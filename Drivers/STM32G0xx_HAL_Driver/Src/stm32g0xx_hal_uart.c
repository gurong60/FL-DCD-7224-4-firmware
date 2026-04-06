/**
  * @file stm32g0xx_hal_uart.c - UART HAL driver
  */
#include "stm32g0xx_hal.h"

extern UART_HandleTypeDef huart1;

HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart)
{
    if (huart == NULL) return HAL_ERROR;

    /* Enable USART1 clock */
    __HAL_RCC_USART1_CLK_ENABLE();

    /* Disable UART */
    huart->Instance->CR1 &= ~USART_CR1_UE;

    /* Set baud rate */
    huart->Instance->BRR = (SystemCoreClock + huart->Init.BaudRate / 2) / huart->Init.BaudRate;

    /* Configure word length, parity, stop bits */
    huart->Instance->CR1 &= ~(USART_CR1_M0 | USART_CR1_M1 | USART_CR1_PCE);
    if (huart->Init.WordLength == UART_WORDLENGTH_9B)
        huart->Instance->CR1 |= USART_CR1_M0;
    if (huart->Init.Parity == UART_PARITY_EVEN || huart->Init.Parity == UART_PARITY_ODD)
        huart->Instance->CR1 |= USART_CR1_PCE;

    MODIFY_REG(huart->Instance->CR2, USART_CR2_STOP, huart->Init.StopBits);

    /* Enable TX, RX, and UART */
    huart->Instance->CR1 |= (USART_CR1_TE | USART_CR1_RE);
    huart->Instance->CR1 |= USART_CR1_UE;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData,
                                     uint16_t Size, uint32_t Timeout)
{
    uint32_t tickstart = HAL_GetTick();

    for (uint16_t i = 0; i < Size; i++)
    {
        while (!(huart->Instance->ISR & USART_ISR_TXE))
        {
            if ((HAL_GetTick() - tickstart) > Timeout)
                return HAL_TIMEOUT;
        }
        huart->Instance->TDR = pData[i];
    }

    /* Wait for transmission complete */
    while (!(huart->Instance->ISR & USART_ISR_TC))
    {
        if ((HAL_GetTick() - tickstart) > Timeout)
            return HAL_TIMEOUT;
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    huart->pRxBuffPtr = pData;
    huart->RxXferSize = Size;
    huart->RxXferCount = Size;

    /* Enable RXNE interrupt */
    huart->Instance->CR1 |= USART_CR1_RXNEIE;

    return HAL_OK;
}

void HAL_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    uint32_t isr = huart->Instance->ISR;

    /* RXNE interrupt */
    if ((isr & USART_ISR_RXNE) && (huart->Instance->CR1 & USART_CR1_RXNEIE))
    {
        if (huart->RxXferCount > 0)
        {
            *huart->pRxBuffPtr = (uint8_t)(huart->Instance->RDR);
            huart->pRxBuffPtr++;
            huart->RxXferCount--;

            if (huart->RxXferCount == 0)
            {
                huart->Instance->CR1 &= ~USART_CR1_RXNEIE;
                HAL_UART_RxCpltCallback(huart);
            }
        }
    }

    /* ORE (overrun) */
    if (isr & USART_ISR_ORE)
    {
        huart->Instance->ICR = USART_ICR_ORECF;
    }
}

__WEAK void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}

__WEAK void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
}
