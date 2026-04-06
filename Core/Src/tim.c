/**
 * @file tim.c - TIM1 PWM configuration (4 channels)
 * @brief PA8=CH1, PB3=CH2, PB4=CH3, PB5=CH4
 *        Timer clock = 64 MHz (APB1 ×1, no prescaler on timer clock)
 *        PWM frequency = 64MHz / (PSC+1) / (ARR+1)
 *        Brightness 0-999 maps to 0-(PWM_ARR) duty cycle
 */
#include "main.h"

TIM_HandleTypeDef htim1;

/* ================================================================
 *  TIM1 Init — 4 independent PWM outputs
 *  Channel 1: PA8  (AF2)
 *  Channel 2: PB3  (AF1)  — note alternate AF
 *  Channel 3: PB4  (AF1)
 *  Channel 4: PB5  (AF1)
 * ================================================================ */
void MX_TIM1_Init(void)
{
    TIM_OC_InitTypeDef sConfigOC = {0};
    GPIO_InitTypeDef   GPIO_InitStruct = {0};

    /* ---- GPIO Clock ---- */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* PA8: TIM1_CH1 AF2 */
    GPIO_InitStruct.Pin       = GPIO_PIN_8;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PB3/PB4/PB5: TIM1_CH2/3/4 AF1 */
    GPIO_InitStruct.Pin       = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* ---- Timer Clock ---- */
    __HAL_RCC_TIM1_CLK_ENABLE();

    htim1.Instance               = TIM1;
    htim1.Init.Prescaler         = PWM_PRESCALER;
    htim1.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim1.Init.Period            = PWM_ARR;
    htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.RepetitionCounter = 0;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
        Error_Handler();

    /* ---- PWM Channel Config ---- */
    sConfigOC.OCMode       = TIM_OCMODE_PWM1;
    sConfigOC.Pulse        = 0;
    sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode   = TIM_OCFAST_ENABLE;
    sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;

    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
        Error_Handler();
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
        Error_Handler();
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
        Error_Handler();
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
        Error_Handler();

    /* ---- Master Output Enable (MOE bit for advanced timer) ---- */
    __HAL_TIM_MOE_ENABLE(&htim1);

    /* ---- Start all 4 PWM channels ---- */
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}

/**
 * @brief Set PWM duty cycle for a channel
 * @param ch: 0-3 (channel 1-4)
 * @param brightness: 0-BRIGHTNESS_MAX (maps to 0-PWM_ARR)
 */
void Set_PWM_Duty(uint8_t ch, uint16_t brightness)
{
    if (ch > 3 || brightness > BRIGHTNESS_MAX)
        return;

    /* Linear mapping: brightness 0-999 → CCR 0-PWM_ARR */
    uint32_t ccr = (uint32_t)brightness * PWM_ARR / BRIGHTNESS_MAX;

    switch (ch) {
        case 0: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ccr); break;
        case 1: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, ccr); break;
        case 2: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, ccr); break;
        case 3: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, ccr); break;
    }
}
