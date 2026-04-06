/**
  * @file stm32g0xx_hal_adc.c - ADC HAL driver (minimal)
  */
#include "stm32g0xx_hal.h"

HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef *hadc)
{
    if (hadc == NULL) return HAL_ERROR;

    __HAL_RCC_ADC_CLK_ENABLE();

    /* Enable ADC voltage regulator */
    hadc->Instance->CR |= ADC_CR_ADVREGEN;
    HAL_Delay(1);  /* Wait for regulator startup */

    /* Disable ADC before configuration */
    if (hadc->Instance->CR & ADC_CR_ADEN)
    {
        hadc->Instance->CR |= ADC_CR_ADDIS;
        while (hadc->Instance->CR & ADC_CR_ADDIS) {}
    }

    /* Configure resolution, alignment, scan mode */
    hadc->Instance->CFGR1 &= ~(ADC_CFGR1_RES | ADC_CFGR1_CONT | ADC_CFGR1_OVRMOD);
    hadc->Instance->CFGR1 |= hadc->Init.Resolution;

    if (hadc->Init.ContinuousConvMode == ENABLE)
        hadc->Instance->CFGR1 |= ADC_CFGR1_CONT;

    if (hadc->Init.Overrun == ADC_OVR_DATA_OVERWRITTEN)
        hadc->Instance->CFGR1 |= ADC_CFGR1_OVRMOD;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef *hadc,
                                         ADC_ChannelConfTypeDef *sConfig)
{
    /* Select channel */
    hadc->Instance->CHSELR = (1UL << sConfig->Channel);

    /* Set sampling time */
    MODIFY_REG(hadc->Instance->SMPR, ADC_SMPR_SMP1,
               sConfig->SamplingTime << ADC_SMPR_SMP1_Pos);

    return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef *hadc)
{
    /* Enable ADC */
    if (!(hadc->Instance->CR & ADC_CR_ADEN))
    {
        hadc->Instance->ISR = ADC_ISR_ADRDY;  /* Clear ready flag */
        hadc->Instance->CR |= ADC_CR_ADEN;
        while (!(hadc->Instance->ISR & ADC_ISR_ADRDY)) {}
    }

    /* Start conversion */
    hadc->Instance->CR |= ADC_CR_ADSTART;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef *hadc, uint32_t Timeout)
{
    uint32_t tickstart = HAL_GetTick();

    while (!(hadc->Instance->ISR & ADC_ISR_EOC))
    {
        if ((HAL_GetTick() - tickstart) > Timeout)
            return HAL_TIMEOUT;
    }

    return HAL_OK;
}

uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef *hadc)
{
    return hadc->Instance->DR;
}

HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef *hadc)
{
    hadc->Instance->CR |= ADC_CR_ADSTP;
    while (hadc->Instance->CR & ADC_CR_ADSTP) {}
    return HAL_OK;
}
