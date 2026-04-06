/**
  ******************************************************************************
  * @file    stm32g0xx.h
  * @brief   CMSIS STM32G0xx Device Peripheral Access Layer Header File
  *
  *          Contains:
  *          - CMSIS core definitions
  *          - Peripheral register structures
  *          - Bit definitions
  *
  *  NOTE: This is a MINIMAL version containing only the registers and bits
  *  actually used by this project. For full HAL, download STM32CubeG0 from:
  *  https://github.com/STMicroelectronics/STM32CubeG0
  ******************************************************************************
  */

#ifndef __STM32G0xx_H
#define __STM32G0xx_H

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================================
 *  CMSIS Core Definitions (Cortex-M0+)
 * ====================================================================== */
#include <stdint.h>
#include "core_cm0plus.h"

/* IO definitions (access restrictions to peripheral registers) */
#define __I  volatile const   /* read only  */
#define __O  volatile         /* write only */
#define __IO volatile         /* read/write */

/* CMSIS compiler specific */
#ifndef __STATIC_INLINE
  #define __STATIC_INLINE static inline
#endif
#ifndef __WEAK
  #define __WEAK __attribute__((weak))
#endif
#ifndef __PACKED
  #define __PACKED __attribute__((packed))
#endif

/* ======================================================================
 *  IRQ Number Definition
 * ====================================================================== */
typedef enum
{
    /* Cortex-M0+ Processor Exceptions Numbers */
    NonMaskableInt_IRQn   = -14,
    HardFault_IRQn        = -13,
    SVCall_IRQn           = -5,
    PendSV_IRQn           = -2,
    SysTick_IRQn          = -1,
    /* STM32G0 specific Interrupt Numbers */
    WWDG_IRQn             = 0,
    RTC_TAMP_IRQn         = 2,
    FLASH_IRQn            = 3,
    RCC_IRQn              = 4,
    EXTI0_1_IRQn          = 5,
    EXTI2_3_IRQn          = 6,
    EXTI4_15_IRQn         = 7,
    DMA1_Channel1_IRQn    = 9,
    DMA1_Channel2_3_IRQn  = 10,
    DMA1_Ch4_7_DMA2_Ch1_5_IRQn = 11,
    ADC1_IRQn             = 12,
    TIM1_BRK_UP_TRG_COM_IRQn = 13,
    TIM1_CC_IRQn          = 14,
    TIM3_IRQn             = 16,
    TIM6_IRQn             = 17,
    TIM7_IRQn             = 18,
    TIM14_IRQn            = 19,
    TIM15_IRQn            = 20,
    TIM16_IRQn            = 21,
    TIM17_IRQn            = 22,
    I2C1_IRQn             = 23,
    I2C2_IRQn             = 24,
    SPI1_IRQn             = 25,
    SPI2_IRQn             = 26,
    USART1_IRQn           = 27,
    USART2_IRQn           = 28,
    USART3_4_LPUART1_IRQn = 29,
} IRQn_Type;

/* ======================================================================
 *  CMSIS Core Register Definitions
 * ====================================================================== */
typedef struct
{
    __IO uint32_t ISER[1U];
         uint32_t RESERVED0[31U];
    __IO uint32_t ICER[1U];
         uint32_t RESERVED1[31U];
    __IO uint32_t ISPR[1U];
         uint32_t RESERVED2[31U];
    __IO uint32_t ICPR[1U];
         uint32_t RESERVED3[31U];
         uint32_t RESERVED4[64U];
    __IO uint32_t IP[8U];
} NVIC_Type;

typedef struct
{
    __I  uint32_t CPUID;
    __IO uint32_t ICSR;
    __IO uint32_t VTOR;
    __IO uint32_t AIRCR;
    __IO uint32_t SCR;
    __IO uint32_t CCR;
         uint32_t RESERVED0;
    __IO uint32_t SHP[2U];
    __IO uint32_t SHCSR;
} SCB_Type;

typedef struct
{
    __IO uint32_t CTRL;
    __IO uint32_t LOAD;
    __IO uint32_t VAL;
    __I  uint32_t CALIB;
} SysTick_Type;

/* Memory mapping */
#define SCS_BASE        (0xE000E000UL)
#define SysTick_BASE    (SCS_BASE + 0x0010UL)
#define NVIC_BASE       (SCS_BASE + 0x0100UL)
#define SCB_BASE        (SCS_BASE + 0x0D00UL)

#define SCB             ((SCB_Type *)SCB_BASE)
#define SysTick         ((SysTick_Type *)SysTick_BASE)
#define NVIC            ((NVIC_Type *)NVIC_BASE)

/* SCB CPUID */
#define SCB_CPUID_IMPLEMENTER_Pos  24U
#define SCB_CPUID_IMPLEMENTER_Msk  (0xFFUL << SCB_CPUID_IMPLEMENTER_Pos)
#define SCB_CPUID_VARIANT_Pos      20U
#define SCB_CPUID_VARIANT_Msk      (0xFUL << SCB_CPUID_VARIANT_Pos)
#define SCB_CPUID_ARCHITECTURE_Pos 16U
#define SCB_CPUID_ARCHITECTURE_Msk (0xFUL << SCB_CPUID_ARCHITECTURE_Pos)
#define SCB_CPUID_PARTNO_Pos       4U
#define SCB_CPUID_PARTNO_Msk       (0xFFFUL << SCB_CPUID_PARTNO_Pos)
#define SCB_CPUID_REVISION_Pos     0U
#define SCB_CPUID_REVISION_Msk     (0xFUL << SCB_CPUID_REVISION_Pos)

/* SysTick CTRL */
#define SysTick_CTRL_COUNTFLAG_Pos  16U
#define SysTick_CTRL_COUNTFLAG_Msk  (1UL << SysTick_CTRL_COUNTFLAG_Pos)
#define SysTick_CTRL_CLKSOURCE_Pos   2U
#define SysTick_CTRL_CLKSOURCE_Msk  (1UL << SysTick_CTRL_CLKSOURCE_Pos)
#define SysTick_CTRL_TICKINT_Pos     1U
#define SysTick_CTRL_TICKINT_Msk    (1UL << SysTick_CTRL_TICKINT_Pos)
#define SysTick_CTRL_ENABLE_Pos      0U
#define SysTick_CTRL_ENABLE_Msk     (1UL << SysTick_CTRL_ENABLE_Pos)
#define SysTick_LOAD_RELOAD_Pos      0U
#define SysTick_LOAD_RELOAD_Msk     (0xFFFFFFUL << SysTick_LOAD_RELOAD_Pos)
#define SysTick_VAL_CURRENT_Pos      0U
#define SysTick_VAL_CURRENT_Msk     (0xFFFFFFUL << SysTick_VAL_CURRENT_Pos)

/* SCB VTOR */
#define SCB_VTOR_TBLOFF_Pos          7U
#define SCB_VTOR_TBLOFF_Msk         (0x1FFFFFFUL << SCB_VTOR_TBLOFF_Pos)

/* NVIC functions */
__STATIC_INLINE void NVIC_EnableIRQ(IRQn_Type IRQn)
{
    NVIC->ISER[0U] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
}

__STATIC_INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
    NVIC->IP[((uint32_t)IRQn)] = (uint8_t)((priority << (8U - 2U)) & (uint32_t)0xFFUL);
}

/* ======================================================================
 *  Peripheral Base Addresses
 * ====================================================================== */
#define PERIPH_BASE           (0x40000000UL)
#define APBPERIPH_BASE        PERIPH_BASE
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)
#define IOPORT_BASE           (PERIPH_BASE + 0x10000000UL)

/* APB peripherals */
#define TIM1_BASE             (APBPERIPH_BASE + 0x12C00UL)
#define USART1_BASE           (APBPERIPH_BASE + 0x3800UL)
#define RCC_BASE              (AHBPERIPH_BASE + 0x0000UL)
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x0020UL)
#define DMA1_BASE             (AHBPERIPH_BASE + 0x0080UL)
#define DMA1_Channel1_BASE    (DMA1_BASE + 0x0008UL)

/* AHB peripherals */
#define GPIOA_BASE            (IOPORT_BASE + 0x0000UL)
#define GPIOB_BASE            (IOPORT_BASE + 0x0400UL)
#define ADC1_BASE             (APBPERIPH_BASE + 0x12400UL)

#define FLASH_BASE            (0x08000000UL)
#define SRAM_BASE             (0x20000000UL)

/* ======================================================================
 *  Peripheral Register Structures
 * ====================================================================== */

/* ---- GPIO ---- */
typedef struct
{
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;
    __IO uint32_t PUPDR;
    __I  uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t LCKR;
    __IO uint32_t AFR[2];
    __IO uint32_t BRR;
} GPIO_TypeDef;

/* ---- RCC ---- */
typedef struct
{
    __IO uint32_t CR;
    __IO uint32_t ICSCR;
    __IO uint32_t CFGR;
    __IO uint32_t PLLCFGR;
    __IO uint32_t RESERVED0;
    __IO uint32_t CRRCR;
    __IO uint32_t CIER;
    __IO uint32_t CIFR;
    __IO uint32_t CICR;
    __IO uint32_t IOPRSTR;
    __IO uint32_t AHBRSTR;
    __IO uint32_t APBRSTR1;
    __IO uint32_t APBRSTR2;
    __IO uint32_t IOPENR;
    __IO uint32_t AHBENR;
    __IO uint32_t APBENR1;
    __IO uint32_t APBENR2;
} RCC_TypeDef;

/* ---- TIM (Basic + Advanced registers we need) ---- */
typedef struct
{
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SMCR;
    __IO uint32_t DIER;
    __IO uint32_t SR;
    __IO uint32_t EGR;
    __IO uint32_t CCMR1;
    __IO uint32_t CCMR2;
    __IO uint32_t CCER;
    __IO uint32_t CNT;
    __IO uint32_t PSC;
    __IO uint32_t ARR;
    __IO uint32_t RCR;
    __IO uint32_t CCR1;
    __IO uint32_t CCR2;
    __IO uint32_t CCR3;
    __IO uint32_t CCR4;
    __IO uint32_t BDTR;
    __IO uint32_t DCR;
    __IO uint32_t DMAR;
} TIM_TypeDef;

/* ---- USART ---- */
typedef struct
{
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t CR3;
    __IO uint32_t BRR;
    __IO uint32_t GTPR;
    __IO uint32_t RTOR;
    __IO uint32_t RQR;
    __I  uint32_t ISR;
    __IO uint32_t ICR;
    __I  uint32_t RDR;
    __IO uint32_t TDR;
} USART_TypeDef;

/* ---- ADC ---- */
typedef struct
{
    __IO uint32_t ISR;
    __IO uint32_t IER;
    __IO uint32_t CR;
    __IO uint32_t CFGR1;
    __IO uint32_t CFGR2;
    __IO uint32_t SMPR;
         uint32_t RESERVED0;
    __IO uint32_t TR;
         uint32_t RESERVED1;
    __IO uint32_t CHSELR;
         uint32_t RESERVED2[5];
    __I  uint32_t DR;
         uint32_t RESERVED3[177];
    __IO uint32_t CCR;
} ADC_TypeDef;

/* ---- FLASH ---- */
typedef struct
{
    __IO uint32_t ACR;
    __IO uint32_t KEYR;
    __IO uint32_t OPTKEYR;
    __IO uint32_t SR;
    __IO uint32_t CR;
    __IO uint32_t ECCR;
         uint32_t RESERVED;
    __IO uint32_t OPTR;
    __IO uint32_t PCROP1ASR;
    __IO uint32_t PCROP1AER;
    __IO uint32_t WRP1AR;
    __IO uint32_t WRP1BR;
    __IO uint32_t PCROP1BSR;
    __IO uint32_t PCROP1BER;
    __IO uint32_t PCROP2ASR;
    __IO uint32_t PCROP2AER;
    __IO uint32_t WRP2AR;
    __IO uint32_t WRP2BR;
} FLASH_TypeDef;

/* ---- PWR (minimal) ---- */
typedef struct
{
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t CR3;
    __IO uint32_t CR4;
    __IO uint32_t SR1;
    __IO uint32_t SR2;
    __IO uint32_t SCR;
         uint32_t RESERVED;
    __IO uint32_t PUCRA;
    __IO uint32_t PDCRA;
    __IO uint32_t PUCRB;
    __IO uint32_t PDCRB;
         uint32_t RESERVED1[4];
    __IO uint32_t CR5;
} PWR_TypeDef;

/* Peripheral instances */
#define GPIOA    ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB    ((GPIO_TypeDef *)GPIOB_BASE)
#define RCC      ((RCC_TypeDef *)RCC_BASE)
#define TIM1     ((TIM_TypeDef *)TIM1_BASE)
#define USART1   ((USART_TypeDef *)USART1_BASE)
#define ADC1     ((ADC_TypeDef *)ADC1_BASE)
#define FLASH    ((FLASH_TypeDef *)FLASH_R_BASE)
#define PWR      ((PWR_TypeDef *)(APBPERIPH_BASE + 0x7000UL))

/* ======================================================================
 *  RCC Bit Definitions (used by HAL)
 * ====================================================================== */

/* RCC_CR */
#define RCC_CR_HSION_Pos              (8U)
#define RCC_CR_HSION_Msk              (0x1UL << RCC_CR_HSION_Pos)
#define RCC_CR_HSION                  RCC_CR_HSION_Msk
#define RCC_CR_HSIRDY_Pos             (10U)
#define RCC_CR_HSIRDY_Msk             (0x1UL << RCC_CR_HSIRDY_Pos)
#define RCC_CR_HSIRDY                 RCC_CR_HSIRDY_Msk
#define RCC_CR_HSIDIV_Pos             (11U)
#define RCC_CR_HSIDIV_Msk             (0x7UL << RCC_CR_HSIDIV_Pos)
#define RCC_CR_HSIDIV                 RCC_CR_HSIDIV_Msk
#define RCC_CR_HSEON_Pos              (16U)
#define RCC_CR_HSEON_Msk              (0x1UL << RCC_CR_HSEON_Pos)
#define RCC_CR_HSEON                  RCC_CR_HSEON_Msk
#define RCC_CR_HSERDY_Pos             (17U)
#define RCC_CR_HSERDY_Msk             (0x1UL << RCC_CR_HSERDY_Pos)
#define RCC_CR_HSERDY                 RCC_CR_HSERDY_Msk
#define RCC_CR_PLLON_Pos              (24U)
#define RCC_CR_PLLON_Msk              (0x1UL << RCC_CR_PLLON_Pos)
#define RCC_CR_PLLON                  RCC_CR_PLLON_Msk
#define RCC_CR_PLLRDY_Pos             (25U)
#define RCC_CR_PLLRDY_Msk             (0x1UL << RCC_CR_PLLRDY_Pos)
#define RCC_CR_PLLRDY                 RCC_CR_PLLRDY_Msk

/* RCC_CFGR */
#define RCC_CFGR_SW_Pos               (0U)
#define RCC_CFGR_SW_Msk               (0x7UL << RCC_CFGR_SW_Pos)
#define RCC_CFGR_SW                   RCC_CFGR_SW_Msk
#define RCC_CFGR_SWS_Pos              (3U)
#define RCC_CFGR_SWS_Msk              (0x7UL << RCC_CFGR_SWS_Pos)
#define RCC_CFGR_SWS                  RCC_CFGR_SWS_Msk
#define RCC_CFGR_HPRE_Pos             (8U)
#define RCC_CFGR_HPRE_Msk             (0xFUL << RCC_CFGR_HPRE_Pos)
#define RCC_CFGR_HPRE                 RCC_CFGR_HPRE_Msk
#define RCC_CFGR_PPRE_Pos             (12U)
#define RCC_CFGR_PPRE_Msk             (0x7UL << RCC_CFGR_PPRE_Pos)
#define RCC_CFGR_PPRE                 RCC_CFGR_PPRE_Msk

/* RCC_PLLCFGR */
#define RCC_PLLCFGR_PLLSRC_Pos        (0U)
#define RCC_PLLCFGR_PLLSRC_Msk        (0x3UL << RCC_PLLCFGR_PLLSRC_Pos)
#define RCC_PLLCFGR_PLLSRC            RCC_PLLCFGR_PLLSRC_Msk
#define RCC_PLLCFGR_PLLM_Pos          (4U)
#define RCC_PLLCFGR_PLLM_Msk          (0x7UL << RCC_PLLCFGR_PLLM_Pos)
#define RCC_PLLCFGR_PLLM              RCC_PLLCFGR_PLLM_Msk
#define RCC_PLLCFGR_PLLN_Pos          (8U)
#define RCC_PLLCFGR_PLLN_Msk          (0x7FUL << RCC_PLLCFGR_PLLN_Pos)
#define RCC_PLLCFGR_PLLN              RCC_PLLCFGR_PLLN_Msk
#define RCC_PLLCFGR_PLLPEN_Pos        (16U)
#define RCC_PLLCFGR_PLLPEN_Msk        (0x1UL << RCC_PLLCFGR_PLLPEN_Pos)
#define RCC_PLLCFGR_PLLPEN            RCC_PLLCFGR_PLLPEN_Msk
#define RCC_PLLCFGR_PLLP_Pos          (17U)
#define RCC_PLLCFGR_PLLP_Msk          (0x7UL << RCC_PLLCFGR_PLLP_Pos)
#define RCC_PLLCFGR_PLLP              RCC_PLLCFGR_PLLP_Msk
#define RCC_PLLCFGR_PLLQEN_Pos        (20U)
#define RCC_PLLCFGR_PLLQEN_Msk        (0x1UL << RCC_PLLCFGR_PLLQEN_Pos)
#define RCC_PLLCFGR_PLLQEN            RCC_PLLCFGR_PLLQEN_Msk
#define RCC_PLLCFGR_PLLQ_Pos          (21U)
#define RCC_PLLCFGR_PLLQ_Msk          (0x7UL << RCC_PLLCFGR_PLLQ_Pos)
#define RCC_PLLCFGR_PLLQ              RCC_PLLCFGR_PLLQ_Msk
#define RCC_PLLCFGR_PLLREN_Pos        (24U)
#define RCC_PLLCFGR_PLLREN_Msk        (0x1UL << RCC_PLLCFGR_PLLREN_Pos)
#define RCC_PLLCFGR_PLLREN            RCC_PLLCFGR_PLLREN_Msk
#define RCC_PLLCFGR_PLLR_Pos          (25U)
#define RCC_PLLCFGR_PLLR_Msk          (0x7UL << RCC_PLLCFGR_PLLR_Pos)
#define RCC_PLLCFGR_PLLR              RCC_PLLCFGR_PLLR_Msk
#define RCC_PLLCFGR_PLLFRACN_Pos      (28U)
#define RCC_PLLCFGR_PLLFRACN_Msk      (0x1FUL << RCC_PLLCFGR_PLLFRACN_Pos)
#define RCC_PLLCFGR_PLLFRACN          RCC_PLLCFGR_PLLFRACN_Msk

/* RCC_IOPENR */
#define RCC_IOPENR_GPIOAEN_Pos        (0U)
#define RCC_IOPENR_GPIOAEN_Msk        (0x1UL << RCC_IOPENR_GPIOAEN_Pos)
#define RCC_IOPENR_GPIOAEN            RCC_IOPENR_GPIOAEN_Msk
#define RCC_IOPENR_GPIOBEN_Pos        (1U)
#define RCC_IOPENR_GPIOBEN_Msk        (0x1UL << RCC_IOPENR_GPIOBEN_Pos)
#define RCC_IOPENR_GPIOBEN            RCC_IOPENR_GPIOBEN_Msk

/* RCC_APBENR1 */
#define RCC_APBENR1_TIM1EN_Pos        (11U)
#define RCC_APBENR1_TIM1EN_Msk        (0x1UL << RCC_APBENR1_TIM1EN_Pos)
#define RCC_APBENR1_TIM1EN            RCC_APBENR1_TIM1EN_Msk
#define RCC_APBENR1_USART1EN_Pos      (14U)
#define RCC_APBENR1_USART1EN_Msk      (0x1UL << RCC_APBENR1_USART1EN_Pos)
#define RCC_APBENR1_USART1EN          RCC_APBENR1_USART1EN_Msk
#define RCC_APBENR1_ADCEN_Pos         (20U)
#define RCC_APBENR1_ADCEN_Msk         (0x1UL << RCC_APBENR1_ADCEN_Pos)
#define RCC_APBENR1_ADCEN             RCC_APBENR1_ADCEN_Msk
#define RCC_APBENR1_PWREN_Pos         (28U)
#define RCC_APBENR1_PWREN_Msk         (0x1UL << RCC_APBENR1_PWREN_Pos)
#define RCC_APBENR1_PWREN             RCC_APBENR1_PWREN_Msk

/* RCC_AHBENR */
#define RCC_AHBENR_FLASHEN_Pos        (8U)
#define RCC_AHBENR_FLASHEN_Msk        (0x1UL << RCC_AHBENR_FLASHEN_Pos)
#define RCC_AHBENR_FLASHEN            RCC_AHBENR_FLASHEN_Msk

/* ======================================================================
 *  TIM Bit Definitions
 * ====================================================================== */
#define TIM_CR1_CEN_Pos               (0U)
#define TIM_CR1_CEN_Msk               (0x1UL << TIM_CR1_CEN_Pos)
#define TIM_CR1_CEN                   TIM_CR1_CEN_Msk
#define TIM_CR1_UDIS_Pos              (1U)
#define TIM_CR1_UDIS_Msk              (0x1UL << TIM_CR1_UDIS_Pos)
#define TIM_CR1_UDIS                  TIM_CR1_UDIS_Msk
#define TIM_CR1_DIR_Pos               (4U)
#define TIM_CR1_DIR_Msk               (0x1UL << TIM_CR1_DIR_Pos)
#define TIM_CR1_DIR                   TIM_CR1_DIR_Msk
#define TIM_CR1_CMS_Pos               (5U)
#define TIM_CR1_CMS_Msk               (0x3UL << TIM_CR1_CMS_Pos)
#define TIM_CR1_CMS                   TIM_CR1_CMS_Msk
#define TIM_CR1_ARPE_Pos              (7U)
#define TIM_CR1_ARPE_Msk              (0x1UL << TIM_CR1_ARPE_Pos)
#define TIM_CR1_ARPE                  TIM_CR1_ARPE_Msk
#define TIM_CR1_CKD_Pos               (8U)
#define TIM_CR1_CKD_Msk               (0x3UL << TIM_CR1_CKD_Pos)
#define TIM_CR1_CKD                   TIM_CR1_CKD_Msk

#define TIM_CR2_CCPC_Pos              (0U)
#define TIM_CR2_CCPC_Msk              (0x1UL << TIM_CR2_CCPC_Pos)
#define TIM_CR2_CCPC                  TIM_CR2_CCPC_Msk
#define TIM_CR2_CCUS_Pos              (2U)
#define TIM_CR2_CCUS_Msk              (0x1UL << TIM_CR2_CCUS_Pos)
#define TIM_CR2_CCUS                  TIM_CR2_CCUS_Msk
#define TIM_CR2_OIS1_Pos              (8U)
#define TIM_CR2_OIS1_Msk              (0x1UL << TIM_CR2_OIS1_Pos)
#define TIM_CR2_OIS1                  TIM_CR2_OIS1_Msk

#define TIM_CCMR1_OC1M_Pos            (4U)
#define TIM_CCMR1_OC1M_Msk            (0x7UL << TIM_CCMR1_OC1M_Pos)
#define TIM_CCMR1_OC1M                TIM_CCMR1_OC1M_Msk
#define TIM_CCMR1_OC1M_1              (0x1UL << TIM_CCMR1_OC1M_Pos)
#define TIM_CCMR1_OC1M_2              (0x2UL << TIM_CCMR1_OC1M_Pos)
#define TIM_CCMR1_OC2M_Pos            (12U)
#define TIM_CCMR1_OC2M_Msk            (0x7UL << TIM_CCMR1_OC2M_Pos)
#define TIM_CCMR1_OC2M                TIM_CCMR1_OC2M_Msk
#define TIM_CCMR1_OC2M_1              (0x1UL << TIM_CCMR1_OC2M_Pos)
#define TIM_CCMR1_OC2M_2              (0x2UL << TIM_CCMR1_OC2M_Pos)
#define TIM_CCMR1_OC1PE_Pos           (3U)
#define TIM_CCMR1_OC1PE_Msk           (0x1UL << TIM_CCMR1_OC1PE_Pos)
#define TIM_CCMR1_OC1PE               TIM_CCMR1_OC1PE_Msk
#define TIM_CCMR1_OC2PE_Pos           (11U)
#define TIM_CCMR1_OC2PE_Msk           (0x1UL << TIM_CCMR1_OC2PE_Pos)
#define TIM_CCMR1_OC2PE               TIM_CCMR1_OC2PE_Msk
#define TIM_CCMR1_CC1S_Pos            (0U)
#define TIM_CCMR1_CC1S_Msk            (0x3UL << TIM_CCMR1_CC1S_Pos)
#define TIM_CCMR1_CC1S                TIM_CCMR1_CC1S_Msk
#define TIM_CCMR1_CC2S_Pos            (8U)
#define TIM_CCMR1_CC2S_Msk            (0x3UL << TIM_CCMR1_CC2S_Pos)
#define TIM_CCMR1_CC2S                TIM_CCMR1_CC2S_Msk

#define TIM_CCMR2_OC3M_Pos            (4U)
#define TIM_CCMR2_OC3M_Msk            (0x7UL << TIM_CCMR2_OC3M_Pos)
#define TIM_CCMR2_OC3M                TIM_CCMR2_OC3M_Msk
#define TIM_CCMR2_OC3M_1              (0x1UL << TIM_CCMR2_OC3M_Pos)
#define TIM_CCMR2_OC3M_2              (0x2UL << TIM_CCMR2_OC3M_Pos)
#define TIM_CCMR2_OC4M_Pos            (12U)
#define TIM_CCMR2_OC4M_Msk            (0x7UL << TIM_CCMR2_OC4M_Pos)
#define TIM_CCMR2_OC4M                TIM_CCMR2_OC4M_Msk
#define TIM_CCMR2_OC4M_1              (0x1UL << TIM_CCMR2_OC4M_Pos)
#define TIM_CCMR2_OC4M_2              (0x2UL << TIM_CCMR2_OC4M_Pos)
#define TIM_CCMR2_OC3PE_Pos           (3U)
#define TIM_CCMR2_OC3PE_Msk           (0x1UL << TIM_CCMR2_OC3PE_Pos)
#define TIM_CCMR2_OC3PE               TIM_CCMR2_OC3PE_Msk
#define TIM_CCMR2_OC4PE_Pos           (11U)
#define TIM_CCMR2_OC4PE_Msk           (0x1UL << TIM_CCMR2_OC4PE_Pos)
#define TIM_CCMR2_OC4PE               TIM_CCMR2_OC4PE_Msk

#define TIM_CCER_CC1E_Pos             (0U)
#define TIM_CCER_CC1E_Msk             (0x1UL << TIM_CCER_CC1E_Pos)
#define TIM_CCER_CC1E                 TIM_CCER_CC1E_Msk
#define TIM_CCER_CC1P_Pos             (1U)
#define TIM_CCER_CC1P_Msk             (0x1UL << TIM_CCER_CC1P_Pos)
#define TIM_CCER_CC1P                 TIM_CCER_CC1P_Msk
#define TIM_CCER_CC2E_Pos             (4U)
#define TIM_CCER_CC2E_Msk             (0x1UL << TIM_CCER_CC2E_Pos)
#define TIM_CCER_CC2E                 TIM_CCER_CC2E_Msk
#define TIM_CCER_CC3E_Pos             (8U)
#define TIM_CCER_CC3E_Msk             (0x1UL << TIM_CCER_CC3E_Pos)
#define TIM_CCER_CC3E                 TIM_CCER_CC3E_Msk
#define TIM_CCER_CC4E_Pos             (12U)
#define TIM_CCER_CC4E_Msk             (0x1UL << TIM_CCER_CC4E_Pos)
#define TIM_CCER_CC4E                 TIM_CCER_CC4E_Msk

#define TIM_BDTR_MOE_Pos              (15U)
#define TIM_BDTR_MOE_Msk              (0x1UL << TIM_BDTR_MOE_Pos)
#define TIM_BDTR_MOE                  TIM_BDTR_MOE_Msk
#define TIM_BDTR_OSSI_Pos             (10U)
#define TIM_BDTR_OSSI_Msk             (0x1UL << TIM_BDTR_OSSI_Pos)
#define TIM_BDTR_OSSI                 TIM_BDTR_OSSI_Msk
#define TIM_BDTR_OSSR_Pos             (11U)
#define TIM_BDTR_OSSR_Msk             (0x1UL << TIM_BDTR_OSSR_Pos)
#define TIM_BDTR_OSSR                 TIM_BDTR_OSSR_Msk
#define TIM_BDTR_BKE_Pos              (12U)
#define TIM_BDTR_BKE_Msk              (0x1UL << TIM_BDTR_BKE_Pos)
#define TIM_BDTR_BKE                  TIM_BDTR_BKE_Msk
#define TIM_BDTR_BKP_Pos              (13U)
#define TIM_BDTR_BKP_Msk              (0x1UL << TIM_BDTR_BKP_Pos)
#define TIM_BDTR_BKP                  TIM_BDTR_BKP_Msk
#define TIM_BDTR_AOE_Pos              (14U)
#define TIM_BDTR_AOE_Msk              (0x1UL << TIM_BDTR_AOE_Pos)
#define TIM_BDTR_AOE                  TIM_BDTR_AOE_Msk
#define TIM_BDTR_DTG_Pos              (0U)
#define TIM_BDTR_DTG_Msk              (0xFFUL << TIM_BDTR_DTG_Pos)
#define TIM_BDTR_DTG                  TIM_BDTR_DTG_Msk

#define TIM_EGR_UG_Pos                (0U)
#define TIM_EGR_UG_Msk                (0x1UL << TIM_EGR_UG_Pos)
#define TIM_EGR_UG                    TIM_EGR_UG_Msk

/* TIM DIER */
#define TIM_DIER_UIE_Pos              (0U)
#define TIM_DIER_UIE_Msk              (0x1UL << TIM_DIER_UIE_Pos)
#define TIM_DIER_UIE                  TIM_DIER_UIE_Msk
#define TIM_DIER_TIE_Pos              (6U)
#define TIM_DIER_TIE_Msk              (0x1UL << TIM_DIER_TIE_Pos)
#define TIM_DIER_TIE                  TIM_DIER_TIE_Msk

/* TIM SR */
#define TIM_SR_UIF_Pos                (0U)
#define TIM_SR_UIF_Msk                (0x1UL << TIM_SR_UIF_Pos)
#define TIM_SR_UIF                    TIM_SR_UIF_Msk
#define TIM_SR_TIF_Pos                (6U)
#define TIM_SR_TIF_Msk                (0x1UL << TIM_SR_TIF_Pos)
#define TIM_SR_TIF                    TIM_SR_TIF_Msk

/* ======================================================================
 *  USART Bit Definitions
 * ====================================================================== */
#define USART_CR1_UE_Pos              (0U)
#define USART_CR1_UE_Msk              (0x1UL << USART_CR1_UE_Pos)
#define USART_CR1_UE                  USART_CR1_UE_Msk
#define USART_CR1_RE_Pos              (2U)
#define USART_CR1_RE_Msk              (0x1UL << USART_CR1_RE_Pos)
#define USART_CR1_RE                  USART_CR1_RE_Msk
#define USART_CR1_TE_Pos              (3U)
#define USART_CR1_TE_Msk              (0x1UL << USART_CR1_TE_Pos)
#define USART_CR1_TE                  USART_CR1_TE_Msk
#define USART_CR1_RXNEIE_Pos          (5U)
#define USART_CR1_RXNEIE_Msk          (0x1UL << USART_CR1_RXNEIE_Pos)
#define USART_CR1_RXNEIE              USART_CR1_RXNEIE_Msk
#define USART_CR1_M0_Pos              (12U)
#define USART_CR1_M0_Msk              (0x1UL << USART_CR1_M0_Pos)
#define USART_CR1_M0                  USART_CR1_M0_Msk
#define USART_CR1_M1_Pos              (28U)
#define USART_CR1_M1_Msk              (0x1UL << USART_CR1_M1_Pos)
#define USART_CR1_M1                  USART_CR1_M1_Msk
#define USART_CR1_PCE_Pos             (10U)
#define USART_CR1_PCE_Msk             (0x1UL << USART_CR1_PCE_Pos)
#define USART_CR1_PCE                 USART_CR1_PCE_Msk
#define USART_CR1_OVER8_Pos           (15U)
#define USART_CR1_OVER8_Msk           (0x1UL << USART_CR1_OVER8_Pos)
#define USART_CR1_OVER8               USART_CR1_OVER8_Msk

#define USART_CR2_STOP_Pos            (12U)
#define USART_CR2_STOP_Msk            (0x3UL << USART_CR2_STOP_Pos)
#define USART_CR2_STOP                USART_CR2_STOP_Msk

#define USART_ISR_RXNE_Pos            (5U)
#define USART_ISR_RXNE_Msk            (0x1UL << USART_ISR_RXNE_Pos)
#define USART_ISR_RXNE                USART_ISR_RXNE_Msk
#define USART_ISR_TXE_Pos             (7U)
#define USART_ISR_TXE_Msk             (0x1UL << USART_ISR_TXE_Pos)
#define USART_ISR_TXE                 USART_ISR_TXE_Msk
#define USART_ISR_TC_Pos              (6U)
#define USART_ISR_TC_Msk              (0x1UL << USART_ISR_TC_Pos)
#define USART_ISR_TC                  USART_ISR_TC_Msk
#define USART_ISR_ORE_Pos             (3U)
#define USART_ISR_ORE_Msk             (0x1UL << USART_ISR_ORE_Pos)
#define USART_ISR_ORE                 USART_ISR_ORE_Msk

#define USART_ICR_ORECF_Pos           (3U)
#define USART_ICR_ORECF_Msk           (0x1UL << USART_ICR_ORECF_Pos)
#define USART_ICR_ORECF               USART_ICR_ORECF_Msk
#define USART_ICR_TCCF_Pos            (6U)
#define USART_ICR_TCCF_Msk            (0x1UL << USART_ICR_TCCF_Pos)
#define USART_ICR_TCCF                USART_ICR_TCCF_Msk

/* ======================================================================
 *  FLASH Bit Definitions
 * ====================================================================== */
#define FLASH_ACR_LATENCY_Pos         (0U)
#define FLASH_ACR_LATENCY_Msk         (0x7UL << FLASH_ACR_LATENCY_Pos)
#define FLASH_ACR_LATENCY             FLASH_ACR_LATENCY_Msk
#define FLASH_ACR_PRFTEN_Pos          (8U)
#define FLASH_ACR_PRFTEN_Msk          (0x1UL << FLASH_ACR_PRFTEN_Pos)
#define FLASH_ACR_PRFTEN              FLASH_ACR_PRFTEN_Msk

#define FLASH_SR_BSY_Pos              (16U)
#define FLASH_SR_BSY_Msk              (0x1UL << FLASH_SR_BSY_Pos)
#define FLASH_SR_BSY                  FLASH_SR_BSY_Msk
#define FLASH_SR_EOP_Pos              (0U)
#define FLASH_SR_EOP_Msk              (0x1UL << FLASH_SR_EOP_Pos)
#define FLASH_SR_EOP                  FLASH_SR_EOP_Msk
#define FLASH_SR_WRPERR_Pos           (17U)
#define FLASH_SR_WRPERR_Msk           (0x1UL << FLASH_SR_WRPERR_Pos)
#define FLASH_SR_WRPERR               FLASH_SR_WRPERR_Msk
#define FLASH_SR_PROGERR_Pos          (3U)
#define FLASH_SR_PROGERR_Msk          (0x1UL << FLASH_SR_PROGERR_Pos)
#define FLASH_SR_PROGERR              FLASH_SR_PROGERR_Msk

#define FLASH_CR_PG_Pos               (0U)
#define FLASH_CR_PG_Msk               (0x1UL << FLASH_CR_PG_Pos)
#define FLASH_CR_PG                   FLASH_CR_PG_Msk
#define FLASH_CR_PER_Pos              (1U)
#define FLASH_CR_PER_Msk              (0x1UL << FLASH_CR_PER_Pos)
#define FLASH_CR_PER                  FLASH_CR_PER_Msk
#define FLASH_CR_MER1_Pos             (2U)
#define FLASH_CR_MER1_Msk             (0x1UL << FLASH_CR_MER1_Pos)
#define FLASH_CR_MER1                 FLASH_CR_MER1_Msk
#define FLASH_CR_PNB_Pos              (3U)
#define FLASH_CR_PNB_Msk              (0xFFUL << FLASH_CR_PNB_Pos)
#define FLASH_CR_PNB                  FLASH_CR_PNB_Msk
#define FLASH_CR_STRT_Pos             (16U)
#define FLASH_CR_STRT_Msk             (0x1UL << FLASH_CR_STRT_Pos)
#define FLASH_CR_STRT                 FLASH_CR_STRT_Msk
#define FLASH_CR_LOCK_Pos             (31U)
#define FLASH_CR_LOCK_Msk             (0x1UL << FLASH_CR_LOCK_Pos)
#define FLASH_CR_LOCK                 FLASH_CR_LOCK_Msk
#define FLASH_CR_FSTPG_Pos            (18U)
#define FLASH_CR_FSTPG_Msk            (0x1UL << FLASH_CR_FSTPG_Pos)
#define FLASH_CR_FSTPG                FLASH_CR_FSTPG_Msk

#define FLASH_KEYR_KEY1               0x45670123UL
#define FLASH_KEYR_KEY2               0xCDEF89ABUL

/* FLASH page size */
#define FLASH_PAGE_SIZE               0x800U  /* 2KB per page for STM32G030 */

/* ======================================================================
 *  ADC Bit Definitions
 * ====================================================================== */
#define ADC_ISR_ADRDY_Pos             (0U)
#define ADC_ISR_ADRDY_Msk             (0x1UL << ADC_ISR_ADRDY_Pos)
#define ADC_ISR_ADRDY                 ADC_ISR_ADRDY_Msk
#define ADC_ISR_EOC_Pos               (2U)
#define ADC_ISR_EOC_Msk               (0x1UL << ADC_ISR_EOC_Pos)
#define ADC_ISR_EOC                   ADC_ISR_EOC_Msk
#define ADC_ISR_EOS_Pos               (3U)
#define ADC_ISR_EOS_Msk               (0x1UL << ADC_ISR_EOS_Pos)
#define ADC_ISR_EOS                   ADC_ISR_EOS_Msk

#define ADC_CR_ADEN_Pos               (0U)
#define ADC_CR_ADEN_Msk               (0x1UL << ADC_CR_ADEN_Pos)
#define ADC_CR_ADEN                   ADC_CR_ADEN_Msk
#define ADC_CR_ADDIS_Pos              (1U)
#define ADC_CR_ADDIS_Msk              (0x1UL << ADC_CR_ADDIS_Pos)
#define ADC_CR_ADDIS                  ADC_CR_ADDIS_Msk
#define ADC_CR_ADSTART_Pos            (2U)
#define ADC_CR_ADSTART_Msk            (0x1UL << ADC_CR_ADSTART_Pos)
#define ADC_CR_ADSTART                ADC_CR_ADSTART_Msk
#define ADC_CR_ADSTP_Pos              (4U)
#define ADC_CR_ADSTP_Msk              (0x1UL << ADC_CR_ADSTP_Pos)
#define ADC_CR_ADSTP                  ADC_CR_ADSTP_Msk
#define ADC_CR_ADVREGEN_Pos           (28U)
#define ADC_CR_ADVREGEN_Msk           (0x1UL << ADC_CR_ADVREGEN_Pos)
#define ADC_CR_ADVREGEN               ADC_CR_ADVREGEN_Msk

#define ADC_CFGR1_RES_Pos             (3U)
#define ADC_CFGR1_RES_Msk             (0x3UL << ADC_CFGR1_RES_Pos)
#define ADC_CFGR1_RES                 ADC_CFGR1_RES_Msk
#define ADC_CFGR1_CONT_Pos            (13U)
#define ADC_CFGR1_CONT_Msk            (0x1UL << ADC_CFGR1_CONT_Pos)
#define ADC_CFGR1_CONT                ADC_CFGR1_CONT_Msk
#define ADC_CFGR1_OVRMOD_Pos          (12U)
#define ADC_CFGR1_OVRMOD_Msk          (0x1UL << ADC_CFGR1_OVRMOD_Pos)
#define ADC_CFGR1_OVRMOD              ADC_CFGR1_OVRMOD_Msk
#define ADC_CFGR1_SCANDIR_Pos         (2U)
#define ADC_CFGR1_SCANDIR_Msk         (0x1UL << ADC_CFGR1_SCANDIR_Pos)
#define ADC_CFGR1_SCANDIR             ADC_CFGR1_SCANDIR_Msk

#define ADC_SMPR_SMP1_Pos             (0U)
#define ADC_SMPR_SMP1_Msk             (0x7UL << ADC_SMPR_SMP1_Pos)
#define ADC_SMPR_SMP1                 ADC_SMPR_SMP1_Msk
#define ADC_SMPR_SMP2_Pos             (4U)
#define ADC_SMPR_SMP2_Msk             (0x7UL << ADC_SMPR_SMP2_Pos)
#define ADC_SMPR_SMP2                 ADC_SMPR_SMP2_Msk

#define ADC_CHSELR_CHSEL0_Pos         (0U)
#define ADC_CHSELR_CHSEL0_Msk         (0x1UL << ADC_CHSELR_CHSEL0_Pos)
#define ADC_CHSELR_CHSEL0             ADC_CHSELR_CHSEL0_Msk

#define ADC_CCR_VREFEN_Pos            (22U)
#define ADC_CCR_VREFEN_Msk            (0x1UL << ADC_CCR_VREFEN_Pos)
#define ADC_CCR_VREFEN                ADC_CCR_VREFEN_Msk

/* ======================================================================
 *  GPIO Bit Definitions
 * ====================================================================== */
#define GPIO_MODER_MODE0_Pos          (0U)
#define GPIO_MODER_MODE0_Msk          (0x3UL << GPIO_MODER_MODE0_Pos)
#define GPIO_MODER_MODE0              GPIO_MODER_MODE0_Msk
#define GPIO_MODER_MODE0_0            (0x1UL << GPIO_MODER_MODE0_Pos)
#define GPIO_MODER_MODE0_1            (0x2UL << GPIO_MODER_MODE0_Pos)

/* ======================================================================
 *  Misc Utilities
 * ====================================================================== */
#define __NOP()     __asm volatile ("nop")
#define __WFI()     __asm volatile ("wfi")
#define __DSB()     __asm volatile ("dsb 0xF" ::: "memory")
#define __ISB()     __asm volatile ("isb 0xF" ::: "memory")
#define __DMB()     __asm volatile ("dmb 0xF" ::: "memory")

/* Peripheral clock enable macros (for inline use in HAL) */
__STATIC_INLINE void __HAL_RCC_GPIOA_CLK_ENABLE(void)
{
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    __NOP(); __NOP(); __NOP(); __NOP();
}

__STATIC_INLINE void __HAL_RCC_GPIOB_CLK_ENABLE(void)
{
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;
    __NOP(); __NOP(); __NOP(); __NOP();
}

__STATIC_INLINE void __HAL_RCC_TIM1_CLK_ENABLE(void)
{
    RCC->APBENR1 |= RCC_APBENR1_TIM1EN;
}

__STATIC_INLINE void __HAL_RCC_USART1_CLK_ENABLE(void)
{
    RCC->APBENR1 |= RCC_APBENR1_USART1EN;
}

__STATIC_INLINE void __HAL_RCC_ADC_CLK_ENABLE(void)
{
    RCC->APBENR1 |= RCC_APBENR1_ADCEN;
    __NOP(); __NOP(); __NOP(); __NOP();
}

__STATIC_INLINE void __HAL_RCC_PWR_CLK_ENABLE(void)
{
    RCC->APBENR1 |= RCC_APBENR1_PWREN;
}

__STATIC_INLINE void __HAL_RCC_FLASH_CLK_ENABLE(void)
{
    RCC->AHBENR |= RCC_AHBENR_FLASHEN;
}

/* Compiler barrier */
#define __FORCEINLINE __attribute__((always_inline)) static inline

/* SystemCoreClock external declaration */
extern uint32_t SystemCoreClock;

/* HSI default calibration */
#define HSI_CALIBRATIONVALUE_DEFAULT 0x10U

#ifdef __cplusplus
}
#endif

#endif /* __STM32G0xx_H */
