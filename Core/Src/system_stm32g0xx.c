/**
  ******************************************************************************
  * @file    system_stm32g0xx.c
  * @brief   CMSIS Cortex-M0+ Device Peripheral Access Layer System Source File
  *
  *          Provides system initialization (SystemInit, SystemCoreClock)
  *          for STM32G030xx devices.
  ******************************************************************************
  */

#include "stm32g0xx.h"

/* This variable is updated in three ways:
   1) by calling CMSIS function SystemCoreClockUpdate()
   2) by calling HAL API function HAL_RCC_GetHCLKFreq()
   3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
*/
uint32_t SystemCoreClock = 16000000UL;  /* Default HSI 16MHz */

const uint32_t AHBPrescTable[16] = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
                                     1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U};
const uint32_t APBPrescTable[8]  = {0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U};

/**
  * @brief  Setup the microcontroller system.
  *         Called early from startup code before main().
  */
void SystemInit(void)
{
    /* Configure the Vector Table location -------------------------------------*/
    SCB->VTOR = FLASH_BASE;  /* Vector Table in Internal Flash */
}

/**
  * @brief  Update SystemCoreClock variable according to Clock Register Values.
  */
void SystemCoreClockUpdate(void)
{
    uint32_t tmp;
    uint32_t pllvco;
    uint32_t pllr;
    uint32_t pllsource;
    uint32_t pllm;
    uint32_t pllfracen;
    uint32_t hsidiv;

    /* Get SYSCLK source -------------------------------------------------------*/
    switch (RCC->CFGR & RCC_CFGR_SWS)
    {
        case RCC_CFGR_SWS_0:                /* HSE used as system clock */
            SystemCoreClock = HSE_VALUE;
            break;

        case (RCC_CFGR_SWS_1 | RCC_CFGR_SWS_0):  /* LSI used as system clock */
            SystemCoreClock = LSI_VALUE;
            break;

        case RCC_CFGR_SWS_2:                /* LSE used as system clock */
            SystemCoreClock = LSE_VALUE;
            break;

        case 0x00000000U:                   /* HSI used as system clock */
        default:
            hsidiv = (1UL << ((RCC->CR & RCC_CR_HSIDIV) >> RCC_CR_HSIDIV_Pos));
            SystemCoreClock = (HSI_VALUE / hsidiv);
            break;

        case RCC_CFGR_SWS_1:                /* PLL used as system clock */
            pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC);
            pllm = ((RCC->PLLCFGR & RCC_PLLCFGR_PLLM) >> RCC_PLLCFGR_PLLM_Pos) + 1U;
            pllfracen = RCC->PLLCFGR & RCC_PLLCFGR_PLLPEN;
            if (pllsource == 0x03UL)        /* HSE used as PLL clock source */
            {
                pllvco = (HSE_VALUE / pllm);
            }
            else if (pllsource == 0x02UL)   /* HSI used as PLL clock source */
            {
                pllvco = (HSI_VALUE / pllm);
            }
            else                            /* No clock source */
            {
                pllvco = 0UL;
            }
            pllvco = pllvco * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> RCC_PLLCFGR_PLLN_Pos);
            if (pllfracen != 0U)
            {
                pllvco += (((RCC->PLLCFGR & RCC_PLLCFGR_PLLFRACN) >> RCC_PLLCFGR_PLLFRACN_Pos)
                           * (pllvco / 8192U));
            }
            pllr = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> RCC_PLLCFGR_PLLR_Pos) + 1U);
            SystemCoreClock = pllvco / pllr;
            break;
    }

    /* Compute HCLK clock frequency --------------------------------------------*/
    /* Get HCLK prescaler */
    tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos)];
    /* HCLK clock frequency */
    SystemCoreClock >>= tmp;
}
