/**
  * @file stm32g0xx_hal_tim.c - TIM HAL driver (PWM mode only)
  */
#include "stm32g0xx_hal.h"

TIM_HandleTypeDef htim1;  /* Defined in tim.c but declared here for HAL */

static void TIM_OC1_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)
{
    MODIFY_REG(TIMx->CCMR1, TIM_CCMR1_CC1S, 0);  /* Output mode */
    MODIFY_REG(TIMx->CCMR1, TIM_CCMR1_OC1M,
               ((OC_Config->OCMode >> 0) & 0x7UL) << TIM_CCMR1_OC1M_Pos);
    if (OC_Config->OCFastMode)
        TIMx->CCMR1 |= TIM_CCMR1_OC1PE;  /* Preload enable */

    if (OC_Config->OCIdleState == TIM_OCIDLESTATE_SET)
        TIMx->CR2 |= TIM_CR2_OIS1;
    else
        TIMx->CR2 &= ~TIM_CR2_OIS1;

    TIMx->CCR1 = OC_Config->Pulse;

    if (OC_Config->OCPolarity == TIM_OCPOLARITY_LOW)
        TIMx->CCER |= TIM_CCER_CC1P;
    else
        TIMx->CCER &= ~TIM_CCER_CC1P;

    TIMx->CCER |= TIM_CCER_CC1E;
}

static void TIM_OC2_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)
{
    MODIFY_REG(TIMx->CCMR1, TIM_CCMR1_CC2S, 0);
    MODIFY_REG(TIMx->CCMR1, TIM_CCMR1_OC2M,
               ((OC_Config->OCMode >> 0) & 0x7UL) << TIM_CCMR1_OC2M_Pos);
    if (OC_Config->OCFastMode)
        TIMx->CCMR1 |= TIM_CCMR1_OC2PE;
    TIMx->CCR2 = OC_Config->Pulse;
    if (OC_Config->OCPolarity == TIM_OCPOLARITY_LOW)
        TIMx->CCER |= TIM_CCER_CC1P << 4;
    else
        TIMx->CCER &= ~(TIM_CCER_CC1P << 4);
    TIMx->CCER |= TIM_CCER_CC2E;
}

static void TIM_OC3_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)
{
    MODIFY_REG(TIMx->CCMR2, TIM_CCMR2_CC1S, 0);  /* reuses CC1S field for ch3 */
    MODIFY_REG(TIMx->CCMR2, TIM_CCMR2_OC3M,
               ((OC_Config->OCMode >> 0) & 0x7UL) << TIM_CCMR2_OC3M_Pos);
    if (OC_Config->OCFastMode)
        TIMx->CCMR2 |= TIM_CCMR2_OC3PE;
    TIMx->CCR3 = OC_Config->Pulse;
    if (OC_Config->OCPolarity == TIM_OCPOLARITY_LOW)
        TIMx->CCER |= TIM_CCER_CC1P << 8;
    else
        TIMx->CCER &= ~(TIM_CCER_CC1P << 8);
    TIMx->CCER |= TIM_CCER_CC3E;
}

static void TIM_OC4_SetConfig(TIM_TypeDef *TIMx, TIM_OC_InitTypeDef *OC_Config)
{
    MODIFY_REG(TIMx->CCMR2, TIM_CCMR2_OC4M,
               ((OC_Config->OCMode >> 0) & 0x7UL) << TIM_CCMR2_OC4M_Pos);
    if (OC_Config->OCFastMode)
        TIMx->CCMR2 |= TIM_CCMR2_OC4PE;
    TIMx->CCR4 = OC_Config->Pulse;
    if (OC_Config->OCPolarity == TIM_OCPOLARITY_LOW)
        TIMx->CCER |= TIM_CCER_CC1P << 12;
    else
        TIMx->CCER &= ~(TIM_CCER_CC1P << 12);
    TIMx->CCER |= TIM_CCER_CC4E;
}

HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim)
{
    if (htim == NULL) return HAL_ERROR;

    /* Enable timer clock */
    if (htim->Instance == TIM1)
        __HAL_RCC_TIM1_CLK_ENABLE();

    /* Set counter mode */
    htim->Instance->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
    htim->Instance->CR1 |= htim->Init.CounterMode;

    /* Set clock division */
    MODIFY_REG(htim->Instance->CR1, TIM_CR1_CKD, htim->Init.ClockDivision);

    /* Set auto-reload and prescaler */
    htim->Instance->ARR = htim->Init.Period;
    htim->Instance->PSC = htim->Init.Prescaler;

    /* Set repetition counter */
    htim->Instance->RCR = htim->Init.RepetitionCounter;

    /* Generate update event */
    htim->Instance->EGR = TIM_EGR_UG;

    /* Configure auto-reload preload */
    if (htim->Init.AutoReloadPreload == TIM_AUTORELOAD_PRELOAD_ENABLE)
        htim->Instance->CR1 |= TIM_CR1_ARPE;
    else
        htim->Instance->CR1 &= ~TIM_CR1_ARPE;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim,
                                            TIM_OC_InitTypeDef *sConfig,
                                            uint32_t Channel)
{
    switch (Channel)
    {
        case TIM_CHANNEL_1:
            TIM_OC1_SetConfig(htim->Instance, sConfig);
            break;
        case TIM_CHANNEL_2:
            TIM_OC2_SetConfig(htim->Instance, sConfig);
            break;
        case TIM_CHANNEL_3:
            TIM_OC3_SetConfig(htim->Instance, sConfig);
            break;
        case TIM_CHANNEL_4:
            TIM_OC4_SetConfig(htim->Instance, sConfig);
            break;
        default:
            return HAL_ERROR;
    }
    return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    /* Enable output for advanced timers */
    if (htim->Instance == TIM1)
        htim->Instance->BDTR |= TIM_BDTR_MOE;

    /* Enable the channel */
    htim->Instance->CCER |= (1UL << (Channel & 0xF));

    /* Enable counter */
    htim->Instance->CR1 |= TIM_CR1_CEN;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    htim->Instance->CCER &= ~(1UL << (Channel & 0xF));
    return HAL_OK;
}
