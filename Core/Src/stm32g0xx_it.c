/**
 * @file stm32g0xx_it.c - Interrupt handlers
 */
#include "stm32g0xx_hal.h"
#include "stm32g0xx_it.h"

extern UART_HandleTypeDef huart1;

void NMI_Handler(void) {}

void HardFault_Handler(void)
{
    while (1) {}
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    /* Not used, but handler must exist for TIM1 advanced timer */
}
