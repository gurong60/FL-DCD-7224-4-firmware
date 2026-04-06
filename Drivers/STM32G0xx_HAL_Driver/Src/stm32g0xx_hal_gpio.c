/**
  * @file stm32g0xx_hal_gpio.c - GPIO HAL driver
  */
#include "stm32g0xx_hal.h"

void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    uint32_t position;
    uint32_t ioposition;
    uint32_t iocurrent;

    for (position = 0; position < 16; position++)
    {
        ioposition = 1UL << position;
        iocurrent = (uint32_t)(GPIO_Init->Pin) & ioposition;

        if (iocurrent == ioposition)
        {
            /* Configure mode */
            MODIFY_REG(GPIOx->MODER, (0x3UL << (position * 2)),
                       ((GPIO_Init->Mode & 0x3UL) << (position * 2)));

            /* Configure output type */
            if ((GPIO_Init->Mode & GPIO_MODE_OUTPUT_PP) == GPIO_MODE_OUTPUT_PP)
                GPIOx->OTYPER &= ~ioposition;
            else if ((GPIO_Init->Mode & GPIO_MODE_OUTPUT_OD) == GPIO_MODE_OUTPUT_OD)
                GPIOx->OTYPER |= ioposition;

            /* Configure speed */
            MODIFY_REG(GPIOx->OSPEEDR, (0x3UL << (position * 2)),
                       (GPIO_Init->Speed << (position * 2)));

            /* Configure pull-up/pull-down */
            MODIFY_REG(GPIOx->PUPDR, (0x3UL << (position * 2)),
                       (GPIO_Init->Pull << (position * 2)));

            /* Configure alternate function */
            if ((GPIO_Init->Mode & GPIO_MODE_AF_PP) == GPIO_MODE_AF_PP ||
                (GPIO_Init->Mode & GPIO_MODE_AF_OD) == GPIO_MODE_AF_OD)
            {
                MODIFY_REG(GPIOx->AFR[position >> 3],
                           (0xFUL << ((position & 0x07UL) * 4)),
                           (GPIO_Init->Alternate << ((position & 0x07UL) * 4)));
            }
        }
    }
}

void HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    uint32_t position;
    uint32_t ioposition;

    for (position = 0; position < 16; position++)
    {
        ioposition = 1UL << position;
        if ((GPIO_Pin & ioposition) == ioposition)
        {
            GPIOx->MODER |= (0x3UL << (position * 2));
            GPIOx->PUPDR &= ~(0x3UL << (position * 2));
            GPIOx->OSPEEDR |= (0x3UL << (position * 2));
            GPIOx->OTYPER &= ~ioposition;
            GPIOx->BRR = ioposition;
            if (position < 8)
                GPIOx->AFR[0] &= ~(0xFUL << (position * 4));
            else
                GPIOx->AFR[1] &= ~(0xFUL << ((position - 8) * 4));
        }
    }
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_PinState bitstatus;

    if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
        bitstatus = GPIO_PIN_SET;
    else
        bitstatus = GPIO_PIN_RESET;

    return bitstatus;
}

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    if (PinState != GPIO_PIN_RESET)
        GPIOx->BSRR = GPIO_Pin;
    else
        GPIOx->BRR = GPIO_Pin;
}

void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;
}
