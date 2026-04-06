/**
 * @file usart.c - RS232 serial communication (SP3232EEN)
 * @brief PA9=TX, PA10=RX, 115200 8N1
 *        Protocol: ASCII commands terminated by '#'
 */
#include "main.h"

UART_HandleTypeDef huart1;
static uint8_t  rx_buf[RS232_RX_BUF_SIZE];
static uint8_t  rx_idx = 0;
static uint8_t  rx_byte;

/* ---- UART Init ---- */
void MX_USART1_UART_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* PA9: USART1_TX AF1, PA10: USART1_RX AF1 */
    GPIO_InitStruct.Pin       = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    huart1.Instance          = USART1;
    huart1.Init.BaudRate     = RS232_BAUDRATE;
    huart1.Init.WordLength   = UART_WORDLENGTH_8B;
    huart1.Init.StopBits     = UART_STOPBITS_1;
    huart1.Init.Parity       = UART_PARITY_NONE;
    huart1.Init.Mode         = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
        Error_Handler();

    /* Enable RX interrupt */
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
}

/* ---- RX interrupt callback ---- */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        if (rx_byte == '#') {
            rx_buf[rx_idx] = '#';
            rx_buf[rx_idx + 1] = '\0';
            rx_idx = 0;  /* ready for next command */
        } else if (rx_idx < RS232_RX_BUF_SIZE - 2) {
            rx_buf[rx_idx++] = rx_byte;
        } else {
            rx_idx = 0;  /* overflow, reset */
        }
        HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
    }
}

/* ---- Helper: send string ---- */
static void UART_Send(const char *str)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), 100);
}

/* ---- Helper: format 3-digit zero-padded ---- */
static void fmt3(char *buf, uint16_t val)
{
    if (val > 999) val = 999;
    buf[0] = '0' + (val / 100) % 10;
    buf[1] = '0' + (val / 10) % 10;
    buf[2] = '0' + val % 10;
    buf[3] = '\0';
}

/* ---- Parse and execute serial commands ---- */
void RS232_Process(void)
{
    if (rx_idx != 0 || rx_buf[0] == '\0')
        return;  /* no complete command yet */

    char resp[24];
    char cmd = 0;

    /* ---- SLV: Set Single Channel Brightness ---- */
    /* Format: SLV<ch><val>#   e.g. SLV1130# */
    if (rx_buf[0] == 'S' && rx_buf[1] == 'L' && rx_buf[2] == 'V') {
        uint8_t ch = rx_buf[3] - '0';
        if (ch >= 1 && ch <= 4) {
            uint16_t val = (rx_buf[4]-'0')*100 + (rx_buf[5]-'0')*10 + (rx_buf[6]-'0');
            if (val <= 999) {
                g_params.brightness[ch-1] = val;
                Set_PWM_Duty(ch-1, val);
                snprintf(resp, sizeof(resp), "SLV%d#", ch);
                UART_Send(resp);
                Params_RequestSave();
            } else {
                UART_Send("ERRSLV#");
            }
        }
        /* SLV0XXXXXXXXXXXX# — set all 4 channels */
        else if (ch == 0) {
            bool ok = true;
            for (int i = 0; i < 4; i++) {
                uint16_t v = (rx_buf[4+i*3]-'0')*100
                           + (rx_buf[5+i*3]-'0')*10
                           + (rx_buf[6+i*3]-'0');
                if (v > 999) { ok = false; break; }
                g_params.brightness[i] = v;
                Set_PWM_Duty(i, v);
            }
            if (ok) {
                UART_Send("SLV0#");
                Params_RequestSave();
            } else {
                UART_Send("ERRSLV#");
            }
        } else {
            UART_Send("ERRSLV#");
        }
        rx_buf[0] = '\0';
        return;
    }

    /* ---- RLV: Read Channel Brightness ---- */
    /* Format: RLV<ch>#   e.g. RLV1# → RLV1XXX# */
    if (rx_buf[0] == 'R' && rx_buf[1] == 'L' && rx_buf[2] == 'V') {
        uint8_t ch = rx_buf[3] - '0';
        char vstr[4];
        if (ch >= 1 && ch <= 4) {
            fmt3(vstr, g_params.brightness[ch-1]);
            snprintf(resp, sizeof(resp), "RLV%d%s#", ch, vstr);
            UART_Send(resp);
        } else if (ch == 0) {
            char all[20];
            char *p = all;
            memcpy(p, "RLV0", 4); p += 4;
            for (int i = 0; i < 4; i++) {
                fmt3(vstr, g_params.brightness[i]);
                memcpy(p, vstr, 3); p += 3;
            }
            *p++ = '#'; *p = '\0';
            UART_Send(all);
        } else {
            UART_Send("ERRRLV#");
        }
        rx_buf[0] = '\0';
        return;
    }

    /* ---- SM: Set Work Mode ---- */
    /* Format: SM<mode>#   e.g. SM1# */
    if (rx_buf[0] == 'S' && rx_buf[1] == 'M') {
        uint8_t mode = rx_buf[2] - '0';
        if (mode <= 2) {
            g_params.work_mode = mode;
            snprintf(resp, sizeof(resp), "SM%d#", mode);
            UART_Send(resp);
            /* Update indicator LEDs */
            CH455G_SetLED(1, (mode == 1), (mode == 2));
            Params_RequestSave();
        } else {
            UART_Send("ERRSLV#");
        }
        rx_buf[0] = '\0';
        return;
    }

    /* ---- RM: Read Work Mode ---- */
    /* Format: RM# → RM<mode># */
    if (rx_buf[0] == 'R' && rx_buf[1] == 'M') {
        snprintf(resp, sizeof(resp), "RM%d#", g_params.work_mode);
        UART_Send(resp);
        rx_buf[0] = '\0';
        return;
    }

    /* ---- SPW: Set Pulse Width (mode 2) ---- */
    /* Format: SPW<ch><val>#   e.g. SPW1120# */
    if (rx_buf[0] == 'S' && rx_buf[1] == 'P' && rx_buf[2] == 'W') {
        uint8_t ch = rx_buf[3] - '0';
        if (ch >= 1 && ch <= 4) {
            uint16_t val = (rx_buf[4]-'0')*100 + (rx_buf[5]-'0')*10 + (rx_buf[6]-'0');
            if (val <= 999) {
                g_params.pulse_width[ch-1] = val;
                snprintf(resp, sizeof(resp), "SPW%d#", ch);
                UART_Send(resp);
                Params_RequestSave();
            } else {
                UART_Send("ERRSLV#");
            }
        } else if (ch == 0) {
            bool ok = true;
            for (int i = 0; i < 4; i++) {
                uint16_t v = (rx_buf[4+i*3]-'0')*100
                           + (rx_buf[5+i*3]-'0')*10
                           + (rx_buf[6+i*3]-'0');
                if (v > 999) { ok = false; break; }
                g_params.pulse_width[i] = v;
            }
            if (ok) {
                UART_Send("SPW0#");
                Params_RequestSave();
            } else {
                UART_Send("ERRSLV#");
            }
        } else {
            UART_Send("ERRSLV#");
        }
        rx_buf[0] = '\0';
        return;
    }

    /* ---- RPW: Read Pulse Width (mode 2) ---- */
    /* Format: RPW<ch># → RPW<ch><val># */
    if (rx_buf[0] == 'R' && rx_buf[1] == 'P' && rx_buf[2] == 'W') {
        uint8_t ch = rx_buf[3] - '0';
        char vstr[4];
        if (ch >= 1 && ch <= 4) {
            fmt3(vstr, g_params.pulse_width[ch-1]);
            snprintf(resp, sizeof(resp), "RPW%d%s#", ch, vstr);
            UART_Send(resp);
        } else if (ch == 0) {
            char all[20];
            char *p = all;
            memcpy(p, "RPW0", 4); p += 4;
            for (int i = 0; i < 4; i++) {
                fmt3(vstr, g_params.pulse_width[i]);
                memcpy(p, vstr, 3); p += 3;
            }
            *p++ = '#'; *p = '\0';
            UART_Send(all);
        } else {
            UART_Send("ERRRLV#");
        }
        rx_buf[0] = '\0';
        return;
    }

    /* Unknown command */
    rx_buf[0] = '\0';
}
