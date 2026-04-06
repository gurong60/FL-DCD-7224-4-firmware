/**
 * @file gpio.c - GPIO initialization
 * @brief Buttons (PB6/7/8 pull-up input), Trigger (PA4-7 input),
 *        LED indicators (PB12-14 output), CH455G bit-bang (PB10/11)
 */
#include "main.h"

void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* ---- Trigger Inputs: PA4-PA7 (floating input, optocoupler drives them) ---- */
    GPIO_InitStruct.Pin  = TRIG1_PIN | TRIG2_PIN | TRIG3_PIN | TRIG4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;  /* default low when no trigger */
    HAL_GPIO_Init(TRIG_GPIO, &GPIO_InitStruct);

    /* ---- Button Inputs: PB6/7/8 — active low, internal pull-up ---- */
    GPIO_InitStruct.Pin  = KEY_L_PIN | KEY_M_PIN | KEY_R_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY_GPIO, &GPIO_InitStruct);

    /* ---- CH455G Bit-Bang Pins: PB10(SCL), PB11(SDA) ---- */
    /* SCL: push-pull output */
    GPIO_InitStruct.Pin   = CH455_SCL_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CH455_GPIO, &GPIO_InitStruct);

    /* SDA: open-drust output (bidirectional for ACK read) */
    GPIO_InitStruct.Pin  = CH455_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(CH455_GPIO, &GPIO_InitStruct);

    /* ---- LED Indicator Outputs: PB12/13/14 ---- */
    GPIO_InitStruct.Pin  = LED_B_PIN | LED_S_PIN | LED_P_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO, &GPIO_InitStruct);

    /* Default: power LED on, others off */
    HAL_GPIO_WritePin(LED_GPIO, LED_B_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED_GPIO, LED_S_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GPIO, LED_P_PIN, GPIO_PIN_RESET);
}
