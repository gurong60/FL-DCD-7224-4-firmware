/**
  ******************************************************************************
  * @file    stm32g0xx_hal.c
  * @brief   HAL module driver - minimal implementation for FL-DCD-7224-4
  ******************************************************************************
  */
#include "stm32g0xx_hal.h"

__IO uint32_t uwTick;
uint32_t uwTickPrio = 0U;
HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;

/* SysTick reload value for 1ms tick at 64MHz */
#define TICK_FREQ_1KHZ  1000U

HAL_StatusTypeDef HAL_Init(void)
{
    /* Configure Flash prefetch and latency */
    FLASH->ACR |= FLASH_ACR_PRFTEN;
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= (0x2UL << FLASH_ACR_LATENCY_Pos);  /* 2 wait states for 64MHz */

    /* Use SysTick as time base source and configure 1ms tick */
    HAL_InitTick(TICK_INT_PRIORITY);

    /* Enable PWR clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    return HAL_OK;
}

HAL_StatusTypeDef HAL_DeInit(void)
{
    return HAL_OK;
}

__WEAK HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    /* Configure SysTick to generate interrupt every 1ms */
    if (SysTick_Config(SystemCoreClock / TICK_FREQ_1KHZ) > 0U)
    {
        return HAL_ERROR;
    }

    /* Configure SysTick interrupt priority */
    NVIC_SetPriority(SysTick_IRQn, TickPriority);
    uwTickPrio = TickPriority;

    return HAL_OK;
}

void HAL_IncTick(void)
{
    uwTick += uwTickFreq;
}

uint32_t HAL_GetTick(void)
{
    return uwTick;
}

void HAL_Delay(uint32_t Delay)
{
    uint32_t tickstart = HAL_GetTick();
    uint32_t wait = Delay;

    if (wait < HAL_MAX_DELAY)
    {
        wait += (uint32_t)(uwTickFreq);
    }

    while ((HAL_GetTick() - tickstart) < wait) {}
}

void HAL_SuspendTick(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void HAL_ResumeTick(void)
{
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

uint32_t HAL_RCC_GetSysClockFreq(void)
{
    return SystemCoreClock;
}

uint32_t HAL_RCC_GetHCLKFreq(void)
{
    return SystemCoreClock;
}

uint32_t HAL_RCC_GetPCLK1Freq(void)
{
    return SystemCoreClock;  /* No APB prescaler in this config */
}
