/**
  * @file stm32g0xx_hal.h - HAL module driver header
  */
#ifndef __STM32G0xx_HAL_H
#define __STM32G0xx_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32g0xx.h"
#include "stm32g0xx_hal_conf.h"

/* ---- Status ---- */
typedef enum
{
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

typedef enum
{
    HAL_TICK_FREQ_10HZ         = 100U,
    HAL_TICK_FREQ_100HZ        = 10U,
    HAL_TICK_FREQ_1KHZ         = 1U,
    HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;

#define HAL_MAX_DELAY      0xFFFFFFFFU
#define UNUSED(x) ((void)(x))

/* ---- GPIO Types ---- */
typedef struct
{
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Pull;
    uint32_t Speed;
    uint32_t Alternate;
} GPIO_InitTypeDef;

typedef enum
{
    GPIO_PIN_RESET = 0U,
    GPIO_PIN_SET
} GPIO_PinState;

#define GPIO_PIN_0                 ((uint16_t)0x0001)
#define GPIO_PIN_1                 ((uint16_t)0x0002)
#define GPIO_PIN_2                 ((uint16_t)0x0004)
#define GPIO_PIN_3                 ((uint16_t)0x0008)
#define GPIO_PIN_4                 ((uint16_t)0x0010)
#define GPIO_PIN_5                 ((uint16_t)0x0020)
#define GPIO_PIN_6                 ((uint16_t)0x0040)
#define GPIO_PIN_7                 ((uint16_t)0x0080)
#define GPIO_PIN_8                 ((uint16_t)0x0100)
#define GPIO_PIN_9                 ((uint16_t)0x0200)
#define GPIO_PIN_10                ((uint16_t)0x0400)
#define GPIO_PIN_11                ((uint16_t)0x0800)
#define GPIO_PIN_12                ((uint16_t)0x1000)
#define GPIO_PIN_13                ((uint16_t)0x2000)
#define GPIO_PIN_14                ((uint16_t)0x4000)
#define GPIO_PIN_15                ((uint16_t)0x8000)
#define GPIO_PIN_All               ((uint16_t)0xFFFF)

#define GPIO_MODE_INPUT            0x00000000U
#define GPIO_MODE_OUTPUT_PP        0x00000001U
#define GPIO_MODE_OUTPUT_OD        0x00000011U
#define GPIO_MODE_AF_PP            0x00000002U
#define GPIO_MODE_AF_OD            0x00000012U
#define GPIO_MODE_ANALOG           0x00000003U

#define GPIO_NOPULL                0x00000000U
#define GPIO_PULLUP                0x00000001U
#define GPIO_PULLDOWN              0x00000002U

#define GPIO_SPEED_FREQ_LOW        0x00000000U
#define GPIO_SPEED_FREQ_MEDIUM     0x00000001U
#define GPIO_SPEED_FREQ_HIGH       0x00000003U

#define GPIO_AF1_TIM1              ((uint8_t)0x01)
#define GPIO_AF1_USART1            ((uint8_t)0x01)
#define GPIO_AF2_TIM1              ((uint8_t)0x02)

/* ---- RCC Types ---- */
#define RCC_OSCILLATORTYPE_NONE    0x00000000U
#define RCC_OSCILLATORTYPE_HSI     0x00000001U
#define RCC_OSCILLATORTYPE_HSE     0x00000002U

#define RCC_HSI_OFF                0x00000000U
#define RCC_HSI_ON                 RCC_CR_HSION
#define RCC_HSI_DIV1               0x00000000U

#define RCC_PLL_NONE               0x00000000U
#define RCC_PLL_OFF                0x00000001U
#define RCC_PLL_ON                 0x00000002U

#define RCC_PLLSOURCE_HSI          0x00000002U
#define RCC_PLLSOURCE_HSE          0x00000003U

#define RCC_PLLM_DIV1              0x00000000U
#define RCC_PLLM_DIV2              0x00000001U
#define RCC_PLLM_DIV3              0x00000002U
#define RCC_PLLM_DIV4              0x00000003U
#define RCC_PLLM_DIV5              0x00000004U
#define RCC_PLLM_DIV6              0x00000005U
#define RCC_PLLM_DIV7              0x00000006U
#define RCC_PLLM_DIV8              0x00000007U

#define RCC_PLLP_DIV2              0x00000000U
#define RCC_PLLP_DIV3              0x00000001U
#define RCC_PLLP_DIV4              0x00000002U
#define RCC_PLLP_DIV5              0x00000003U
#define RCC_PLLP_DIV6              0x00000004U
#define RCC_PLLP_DIV7              0x00000005U
#define RCC_PLLP_DIV8              0x00000006U
#define RCC_PLLP_DIV9              0x00000007U

#define RCC_PLLQ_DIV2              0x00000000U
#define RCC_PLLQ_DIV3              0x00000001U
#define RCC_PLLQ_DIV4              0x00000002U
#define RCC_PLLQ_DIV5              0x00000003U
#define RCC_PLLQ_DIV6              0x00000004U
#define RCC_PLLQ_DIV7              0x00000005U
#define RCC_PLLQ_DIV8              0x00000006U
#define RCC_PLLQ_DIV9              0x00000007U

#define RCC_PLLR_DIV2              0x00000000U
#define RCC_PLLR_DIV3              0x00000001U
#define RCC_PLLR_DIV4              0x00000002U
#define RCC_PLLR_DIV5              0x00000003U
#define RCC_PLLR_DIV6              0x00000004U
#define RCC_PLLR_DIV7              0x00000005U
#define RCC_PLLR_DIV8              0x00000006U
#define RCC_PLLR_DIV9              0x00000007U

#define RCC_SYSCLKSOURCE_PLLCLK    RCC_CFGR_SW_1
#define RCC_SYSCLKSOURCE_HSI       0x00000000U

#define RCC_CLOCKTYPE_HCLK         0x00000002U
#define RCC_CLOCKTYPE_SYSCLK       0x00000001U
#define RCC_CLOCKTYPE_PCLK1        0x00000004U

#define RCC_SYSCLK_DIV1            0x00000000U
#define RCC_SYSCLK_DIV2            RCC_CFGR_HPRE_3
#define RCC_SYSCLK_DIV4            (RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_0)
#define RCC_HCLK_DIV1              0x00000000U
#define RCC_HCLK_DIV2              RCC_CFGR_PPRE_2
#define RCC_HCLK_DIV4              (RCC_CFGR_PPRE_2 | RCC_CFGR_PPRE_0)

#define RCC_HSICALIBRATION_DEFAULT 0x10U

typedef struct
{
    uint32_t OscillatorType;
    uint32_t HSIState;
    uint32_t HSICalibrationValue;
    uint32_t HSIDiv;
    uint32_t HSEState;
    uint32_t LSIState;
    uint32_t LSEState;
    struct {
        uint32_t PLLState;
        uint32_t PLLSource;
        uint32_t PLLM;
        uint32_t PLLN;
        uint32_t PLLP;
        uint32_t PLLQ;
        uint32_t PLLR;
        uint32_t PLLFRACN;
    } PLL;
} RCC_OscInitTypeDef;

typedef struct
{
    uint32_t ClockType;
    uint32_t SYSCLKSource;
    uint32_t AHBCLKDivider;
    uint32_t APB1CLKDivider;
} RCC_ClkInitTypeDef;

/* ---- TIM Types ---- */
typedef struct
{
    uint32_t Prescaler;
    uint32_t CounterMode;
    uint32_t Period;
    uint32_t ClockDivision;
    uint32_t RepetitionCounter;
    uint32_t AutoReloadPreload;
} TIM_Base_InitTypeDef;

typedef TIM_Base_InitTypeDef TIM_InitTypeDef;

typedef struct
{
    uint32_t OCMode;
    uint32_t Pulse;
    uint32_t OCPolarity;
    uint32_t OCNPolarity;
    uint32_t OCFastMode;
    uint32_t OCIdleState;
    uint32_t OCNIdleState;
} TIM_OC_InitTypeDef;

typedef struct
{
    TIM_TypeDef            *Instance;
    TIM_InitTypeDef         Init;
} TIM_HandleTypeDef;

#define TIM_COUNTERMODE_UP             0x00000000U
#define TIM_CLOCKDIVISION_DIV1         0x00000000U
#define TIM_AUTORELOAD_PRELOAD_DISABLE 0x00000000U
#define TIM_AUTORELOAD_PRELOAD_ENABLE  TIM_CR1_ARPE

#define TIM_OCMODE_PWM1                (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2)
#define TIM_OCPOLARITY_HIGH            0x00000000U
#define TIM_OCPOLARITY_LOW             TIM_CCER_CC1P
#define TIM_OCIDLESTATE_RESET          0x00000000U
#define TIM_OCIDLESTATE_SET            TIM_CR2_OIS1
#define TIM_OCFAST_ENABLE              TIM_CCMR1_OC1PE

#define TIM_CHANNEL_1                  0x00000000U
#define TIM_CHANNEL_2                  0x00000004U
#define TIM_CHANNEL_3                  0x00000008U
#define TIM_CHANNEL_4                  0x0000000CU

#define __HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__) \
    (((__HANDLE__)->Instance->CCR1 + (__CHANNEL__)) = (__COMPARE__))

/* Actually, CCR1 is at offset 0, CCR2 at offset 4, etc. */
#undef __HAL_TIM_SET_COMPARE
#define __HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__) \
    (*(&(__HANDLE__)->Instance->CCR1 + ((__CHANNEL__) >> 2U)) = (__COMPARE__))

#define __HAL_TIM_MOE_ENABLE(__HANDLE__) \
    ((__HANDLE__)->Instance->BDTR |= TIM_BDTR_MOE)

/* ---- UART Types ---- */
typedef struct
{
    uint32_t BaudRate;
    uint32_t WordLength;
    uint32_t StopBits;
    uint32_t Parity;
    uint32_t Mode;
    uint32_t HwFlowCtl;
    uint32_t OverSampling;
} UART_InitTypeDef;

typedef struct
{
    USART_TypeDef         *Instance;
    UART_InitTypeDef       Init;
    uint8_t               *pRxBuffPtr;
    uint16_t               RxXferSize;
    uint16_t               RxXferCount;
} UART_HandleTypeDef;

#define UART_WORDLENGTH_8B             0x00000000U
#define UART_WORDLENGTH_9B             USART_CR1_M0
#define UART_STOPBITS_1                0x00000000U
#define UART_STOPBITS_2                USART_CR2_STOP_1
#define UART_PARITY_NONE               0x00000000U
#define UART_PARITY_EVEN               USART_CR1_PCE
#define UART_PARITY_ODD                (USART_CR1_PCE | USART_CR1_M0)
#define UART_MODE_TX_RX                (USART_CR1_TE | USART_CR1_RE)
#define UART_HWCONTROL_NONE            0x00000000U
#define UART_OVERSAMPLING_16           0x00000000U

/* ---- ADC Types ---- */
typedef struct
{
    uint32_t ClockPrescaler;
    uint32_t Resolution;
    uint32_t DataAlign;
    uint32_t ScanConvMode;
    uint32_t EOCSelection;
    uint32_t LowPowerAutoWait;
    uint32_t LowPowerAutoPowerOff;
    uint32_t ContinuousConvMode;
    uint32_t NbrOfConversion;
    uint32_t DiscontinuousConvMode;
    uint32_t ExternalTrigConv;
    uint32_t ExternalTrigConvEdge;
    uint32_t DMAContinuousRequests;
    uint32_t Overrun;
    uint32_t OversamplingMode;
} ADC_InitTypeDef;

typedef struct
{
    uint32_t Channel;
    uint32_t Rank;
    uint32_t SamplingTime;
    uint32_t SingleDiff;
    uint32_t OffsetNumber;
    uint32_t Offset;
    uint32_t OffsetSign;
    uint32_t OffsetSaturation;
} ADC_ChannelConfTypeDef;

typedef struct
{
    ADC_TypeDef       *Instance;
    ADC_InitTypeDef    Init;
} ADC_HandleTypeDef;

#define ADC_RESOLUTION_12B             0x00000000U
#define ADC_DATAALIGN_RIGHT            0x00000000U
#define ADC_SCAN_DISABLE               0x00000000U
#define ADC_EOC_SINGLE_CONV            0x00000000U
#define ADC_OVR_DATA_PRESERVED         0x00000000U
#define ADC_OVR_DATA_OVERWRITTEN       0x00000001U

#define ADC_SOFTWARE_START             0x00000000U
#define ADC_EXTERNALTRIGCONVEDGE_NONE  0x00000000U
#define ADC_CLOCK_SYNC_PCLK_DIV4       ((uint32_t)0x00000000U)

#define ADC_CHANNEL_0                  0x00000000U
#define ADC_CHANNEL_1                  0x00000001U
#define ADC_REGULAR_RANK_1             0x00000001U

#define ADC_SAMPLETIME_39CYCLES_5      ((uint32_t)0x00000003U)

/* ---- Flash Types ---- */
#define FLASH_TYPEERASE_PAGES          0x00U
#define FLASH_TYPEPROGRAM_DOUBLEWORD   0x03U

typedef struct
{
    uint32_t TypeErase;
    uint32_t Page;
    uint32_t NbPages;
} FLASH_EraseInitTypeDef;

/* ---- Function Prototypes ---- */
HAL_StatusTypeDef HAL_Init(void);
HAL_StatusTypeDef HAL_DeInit(void);
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
void              HAL_IncTick(void);
uint32_t          HAL_GetTick(void);
void              HAL_Delay(uint32_t Delay);
void              HAL_SuspendTick(void);
void              HAL_ResumeTick(void);
uint32_t          HAL_RCC_GetSysClockFreq(void);
uint32_t          HAL_RCC_GetHCLKFreq(void);
uint32_t          HAL_RCC_GetPCLK1Freq(void);
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef *RCC_OscInitStruct);
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *RCC_ClkInitStruct, uint32_t FLatency);

void              HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);
void              HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
GPIO_PinState     HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void              HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void              HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef *sConfig, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);

HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
void              HAL_UART_IRQHandler(UART_HandleTypeDef *huart);
void              HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void              HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);

HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef *hadc);
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef *hadc, ADC_ChannelConfTypeDef *sConfig);
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef *hadc);
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef *hadc, uint32_t Timeout);
uint32_t          HAL_ADC_GetValue(ADC_HandleTypeDef *hadc);
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef *hadc);

HAL_StatusTypeDef HAL_FLASH_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_Lock(void);
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError);

void              HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
void              HAL_NVIC_EnableIRQ(IRQn_Type IRQn);
void              HAL_NVIC_DisableIRQ(IRQn_Type IRQn);

/* Misc macros */
#define MODIFY_REG(REG, CLEARMASK, SETMASK) \
    WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))
#define READ_REG(REG)         ((REG))

/* ICSCFGR HSITRIM field (in ICSCR for G0) */
#define RCC_ICSCFGR_HSITRIM_Pos   (11U)
#define RCC_ICSCFGR_HSITRIM_Msk   (0x7FUL << RCC_ICSCFGR_HSITRIM_Pos)
#define RCC_ICSCFGR_HSITRIM       RCC_ICSCFGR_HSITRIM_Msk

#ifdef __cplusplus
}
#endif

#endif /* __STM32G0xx_HAL_H */
