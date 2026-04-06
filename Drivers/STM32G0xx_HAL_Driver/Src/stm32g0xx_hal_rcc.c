/**
  * @file stm32g0xx_hal_rcc.c - RCC HAL driver (minimal)
  */
#include "stm32g0xx_hal.h"

#define HSE_TIMEOUT_VALUE     100U
#define HSI_TIMEOUT_VALUE     2U
#define PLL_TIMEOUT_VALUE     2U

HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInitStruct)
{
    /* HSI Configuration */
    if ((RCC_OscInitStruct->OscillatorType & RCC_OSCILLATORTYPE_HSI) == RCC_OSCILLATORTYPE_HSI)
    {
        if (RCC_OscInitStruct->HSIState != RCC_HSI_OFF)
        {
            RCC->CR |= RCC_CR_HSION;
            /* Wait till HSI is ready */
            uint32_t tickstart = HAL_GetTick();
            while ((RCC->CR & RCC_CR_HSIRDY) == 0U)
            {
                if ((HAL_GetTick() - tickstart) > HSI_TIMEOUT_VALUE)
                    return HAL_TIMEOUT;
            }
            /* HSI Calibration */
            MODIFY_REG(RCC->ICSCR, RCC_ICSCFGR_HSITRIM,
                       (uint32_t)(RCC_OscInitStruct->HSICalibrationValue) << RCC_ICSCFGR_HSITRIM_Pos);
        }
    }

    /* PLL Configuration */
    if (RCC_OscInitStruct->PLL.PLLState != RCC_PLL_NONE)
    {
        if (RCC_OscInitStruct->PLL.PLLState == RCC_PLL_ON)
        {
            /* Disable PLL */
            RCC->CR &= ~RCC_CR_PLLON;
            while ((RCC->CR & RCC_CR_PLLRDY) != 0U) {}

            /* Configure PLL source */
            RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
            RCC->PLLCFGR |= RCC_OscInitStruct->PLL.PLLSource;

            /* Configure PLL M, N, P, Q, R */
            MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLM,
                       ((RCC_OscInitStruct->PLL.PLLM) << RCC_PLLCFGR_PLLM_Pos));
            MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN,
                       ((RCC_OscInitStruct->PLL.PLLN) << RCC_PLLCFGR_PLLN_Pos));
            MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLP,
                       ((RCC_OscInitStruct->PLL.PLLP) << RCC_PLLCFGR_PLLP_Pos));
            MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ,
                       ((RCC_OscInitStruct->PLL.PLLQ) << RCC_PLLCFGR_PLLQ_Pos));
            MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLR,
                       ((RCC_OscInitStruct->PLL.PLLR) << RCC_PLLCFGR_PLLR_Pos));

            /* Enable PLL R output */
            RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

            /* Enable PLL */
            RCC->CR |= RCC_CR_PLLON;
            while ((RCC->CR & RCC_CR_PLLRDY) == 0U) {}
        }
        else
        {
            RCC->CR &= ~RCC_CR_PLLON;
        }
    }

    return HAL_OK;
}

HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t FLatency)
{
    /* Set flash latency */
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLatency);

    /* Set SYSCLK source */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_ClkInitStruct->SYSCLKSource);

    /* Wait till PLL is used as system clock source */
    uint32_t tickstart = HAL_GetTick();
    while ((RCC->CFGR & RCC_CFGR_SWS) != (RCC_ClkInitStruct->SYSCLKSource << 3U))
    {
        if ((HAL_GetTick() - tickstart) > 5000U)
            return HAL_TIMEOUT;
    }

    /* Set AHB and APB prescalers */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_ClkInitStruct->AHBCLKDivider);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE, RCC_ClkInitStruct->APB1CLKDivider);

    /* Update SystemCoreClock */
    SystemCoreClockUpdate();

    return HAL_OK;
}
