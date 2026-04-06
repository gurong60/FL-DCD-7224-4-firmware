/**
 * @file stm32g0xx_it.h
 */
#ifndef __STM32G0XX_IT_H
#define __STM32G0XX_IT_H
#ifdef __cplusplus
extern "C" {
#endif
void NMI_Handler(void);
void HardFault_Handler(void);
void SysTick_Handler(void);
void USART1_IRQHandler(void);
void TIM1_BRK_UP_TRG_COM_IRQHandler(void);
#ifdef __cplusplus
}
#endif
#endif
