/**
  * @file stm32g0xx_hal_cortex.c - Cortex-M0+ HAL driver
  */
#include "stm32g0xx_hal.h"

void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)
{
    /* STM32G030 uses 2-bit priority */
    uint32_t priority = (PreemptPriority << (8U - 2U));
    NVIC_SetPriority(IRQn, priority);
}

void HAL_NVIC_EnableIRQ(IRQn_Type IRQn)
{
    NVIC_EnableIRQ(IRQn);
}

void HAL_NVIC_DisableIRQ(IRQn_Type IRQn)
{
    NVIC->ICER[0U] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
}
