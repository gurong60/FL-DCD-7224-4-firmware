/**
 * @file main.c - FL-DCD-7224-4 Digital Light Source Controller
 * @brief Main entry, system clock, and super-loop
 *        STM32G030C8T6 @ 64 MHz HSI
 */
#include "main.h"

volatile uint32_t g_tick_ms = 0;

/* ---- SysTick callback (called every 1 ms) ---- */
void HAL_IncTick(void);
void SysTick_Handler(void)
{
    HAL_IncTick();
    g_tick_ms++;
}

/* ---- Clock: HSI 64 MHz, 0-wait-state Flash ---- */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* HSI 16 MHz → PLL × 4 → 64 MHz system clock */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv         = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM       = RCC_PLLM_DIV1;
    RCC_OscInitStruct.PLL.PLLN       = 8;     /* 16/1*8 = 128 MHz VCO */
    RCC_OscInitStruct.PLL.PLLP       = RCC_PLLP_DIV2; /* 64 MHz */
    RCC_OscInitStruct.PLL.PLLQ       = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR       = RCC_PLLR_DIV2; /* 64 MHz SYSCLK */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        Error_Handler();

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                     | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;   /* HCLK=64 MHz */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;     /* PCLK=64 MHz */
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
        Error_Handler();
}

/* ---- Error handler ---- */
void Error_Handler(void)
{
    __disable_irq();
    while (1) { }
}

/* ======================================================================
 *  Main entry
 * ====================================================================== */
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    /* Peripheral init order matters */
    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_USART1_UART_Init();

    CH455G_Init();
    Buttons_Init();
    Trigger_Init();
    Overcurrent_Init();

    /* Load saved parameters from Flash */
    Params_Load();

    /* Apply initial brightness to all channels */
    for (uint8_t ch = 0; ch < 4; ch++)
        Set_PWM_Duty(ch, g_params.brightness[ch]);

    /* Display initial state */
    CH455G_DisplayUpdate(DISP_CHANNEL_1, 0, g_params.brightness[0]);

    /* ---- Super-loop ---- */
    while (1)
    {
        RS232_Process();      /* Parse incoming serial commands */
        Buttons_Process();    /* Scan buttons, adjust brightness/mode */
        Trigger_Process();    /* Handle trigger inputs per work mode */
        Overcurrent_Process();
        Params_SaveTick();    /* Check 30-second delayed save */
    }
}
