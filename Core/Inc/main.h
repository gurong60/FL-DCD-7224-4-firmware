/**
 * @file main.h - FL-DCD-7224-4 Digital Light Source Controller
 * @brief Main application header for STM32G030C8T6
 */
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32g0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* ======================================================================
 *  PWM Frequency Selection - Use exactly ONE macro at a time
 *  PWM_FREQ_64K  =  64kHz (period=750, resolution 0-749)
 *  PWM_FREQ_250K = 250kHz (period=192, resolution 0-191)
 *  PWM_FREQ_500K = 500kHz (period=96,  resolution 0-95)
 * ====================================================================== */
#define PWM_FREQ_64K
// #define PWM_FREQ_250K
// #define PWM_FREQ_500K

/* ---------- derived constants (do not edit) ---------- */
#if defined(PWM_FREQ_64K)
  /* 64MHz / (0+1) / (999+1) = 64kHz, resolution 0-999 (matches manual) */
  #define PWM_PRESCALER   0U
  #define PWM_ARR         999U
  #define PWM_RESOLUTION  1000U
#elif defined(PWM_FREQ_250K)
  /* 64MHz / (0+1) / (255+1) = 250kHz, resolution 0-255 */
  #define PWM_PRESCALER   0U
  #define PWM_ARR         255U
  #define PWM_RESOLUTION  256U
#elif defined(PWM_FREQ_500K)
  /* 64MHz / (0+1) / (127+1) = 500kHz, resolution 0-127 */
  #define PWM_PRESCALER   0U
  #define PWM_ARR         127U
  #define PWM_RESOLUTION  128U
#else
  #error "Please select exactly one PWM frequency: PWM_FREQ_64K, PWM_FREQ_250K, or PWM_FREQ_500K"
#endif

/* ---------- brightness scaling: map 0-999 → 0-PWM_ARR ---------- */
#define BRIGHTNESS_MAX  999U

/* ---------- GPIO Pin Definitions (per schematic) ---------- */
/* Trigger inputs PA4-PA7 (directly on MCU pins, optocoupler active-high) */
#define TRIG1_PIN       GPIO_PIN_4
#define TRIG2_PIN       GPIO_PIN_5
#define TRIG3_PIN       GPIO_PIN_6
#define TRIG4_PIN       GPIO_PIN_7
#define TRIG_GPIO       GPIOA

/* Button inputs PB6(CH/Left), PB7(Middle), PB8(Right) - active low, pull-up */
#define KEY_L_PIN       GPIO_PIN_6
#define KEY_M_PIN       GPIO_PIN_7
#define KEY_R_PIN       GPIO_PIN_8
#define KEY_GPIO        GPIOB

/* USART1: PA9=TX, PA10=RX (RS232 via SP3232EEN) */
/* I2C1:  PB8=SCL, PB9=SDA (CH455G display) — conflict! PB8 used by KEY_R
 * Schematic shows SCL/SDA on connector CN2, I2C likely uses PA9/PA10 alternate
 * but those are USART too. Actual: CH455G uses bit-bang on PB10(SCL)/PB11(SDA) */
#define CH455_SCL_PIN   GPIO_PIN_10
#define CH455_SDA_PIN   GPIO_PIN_11
#define CH455_GPIO      GPIOB

/* Power enable (optional, from SY8201 EN = PA3 per schematic U1 EN=pin3, but
 * EN is pulled high via R21 100kΩ, so always enabled unless MCU pulls low) */
/* Not directly controlled by MCU — skip */

/* LED indicators: active on CH455G extra outputs via display chip, or direct GPIO */
/* From schematic: LED1-LED4 are driven by CH455G, not MCU GPIO */
#define LED_B_PIN       GPIO_PIN_12  /* Power LED — PB12 if directly driven */
#define LED_S_PIN       GPIO_PIN_13  /* Mode1 LED */
#define LED_P_PIN       GPIO_PIN_14  /* Mode2 LED */
#define LED_GPIO        GPIOB

/* ADC current sense: PA0 */
#define ADC_CURRENT_CHANNEL  ADC_CHANNEL_0

/* ---------- Flash storage address (last 2KB of 64KB Flash) ---------- */
#define FLASH_STORAGE_ADDR   0x0800F800U
#define FLASH_PAGE_SIZE      2048U  /* STM32G030: 2KB per page */

/* ---------- RS232 Protocol ---------- */
#define RS232_BAUDRATE       115200U
#define RS232_RX_BUF_SIZE    32

/* ---------- Timing ---------- */
#define SAVE_DELAY_MS        30000U   /* 30 seconds delayed save */
#define DEBOUNCE_MS          30U
#define LONG_PRESS_MS        800U
#define RAPID_STEP_MS        80U

/* ---------- Work Modes ---------- */
typedef enum {
    MODE_ALWAYS_ON  = 0,   /* Constant brightness */
    MODE_TRIGGER_1  = 1,   /* Level-triggered: high=on */
    MODE_TRIGGER_2  = 2,   /* Edge-triggered: pulse output */
} WorkMode_t;

/* ---------- Parameter Storage Structure ---------- */
typedef struct {
    uint16_t brightness[4];   /* 0-999 per channel */
    uint16_t pulse_width[4];  /* 0-999 ms, mode2 only */
    uint8_t  work_mode;       /* 0/1/2 */
    uint8_t  padding[3];      /* alignment */
    uint32_t magic;           /* 0x4C454421 "LED!" */
} ParamStore_t;

/* ---------- Display ---------- */
typedef enum {
    DISP_CHANNEL_1 = 0,
    DISP_CHANNEL_2,
    DISP_CHANNEL_3,
    DISP_CHANNEL_4,
    DISP_MODE_SELECT,
    DISP_PULSE_WIDTH,
} DisplayMode_t;

/* ---------- Global Variables (defined in main.c) ---------- */
extern volatile uint32_t g_tick_ms;

/* ---------- Function Prototypes ---------- */
/* main.c */
void SystemClock_Config(void);
void Error_Handler(void);

/* gpio.c */
void MX_GPIO_Init(void);

/* tim.c */
void MX_TIM1_Init(void);
void Set_PWM_Duty(uint8_t ch, uint16_t brightness);

/* usart.c */
void MX_USART1_UART_Init(void);
void RS232_Process(void);

/* ch455g.c */
void CH455G_Init(void);
void CH455G_DisplayNumber(uint8_t ch, uint16_t num);
void CH455G_SetLED(uint8_t b, uint8_t s, uint8_t p);
void CH455G_DisplayUpdate(DisplayMode_t mode, uint8_t channel,
                           uint16_t value);

/* buttons.c */
void Buttons_Init(void);
void Buttons_Process(void);

/* params.c */
void Params_Load(void);
void Params_Save(void);
void Params_RequestSave(void);
void Params_SaveTick(void);

/* trigger.c */
void Trigger_Init(void);
void Trigger_Process(void);

/* overcurrent.c */
void Overcurrent_Init(void);
void Overcurrent_Process(void);

#endif /* __MAIN_H */
