/**
 * @file ch455g.c - CH455G 7-segment display + LED driver
 * @brief Bit-bang I2C on PB10(SCL), PB11(SDA)
 *
 *  CH455G command format: 2 bytes
 *  Byte0: addr (0x40 for display data, 0x48 for LED/key)
 *  Byte1: command/data
 *
 *  Display registers (4 digits, active-low segments):
 *    Digit0: 0x60 + seg_data  (leftmost — channel indicator '1'-'4' or 'H')
 *    Digit1: 0x62 + seg_data  (hundreds)
 *    Digit2: 0x64 + seg_data  (tens)
 *    Digit3: 0x66 + seg_data  (ones)
 *
 *  LED register: 0x48, bit0=LED1, bit1=LED2, bit2=LED3, bit3=LED4
 *  (active low: 0=on, 1=off)
 *
 *  7-segment map (active-low, common-anode):
 *    bit: 6 5 4 3 2 1 0
 *         a b c d e f g
 */
#include "main.h"

/* 7-segment patterns (0=segment ON, 1=segment OFF) for common-anode */
static const uint8_t seg_font[10] = {
    /* 0 */ 0x01,  /* a b c d e f on, g off */
    /* 1 */ 0x4F,  /* b c on */
    /* 2 */ 0x11,  /* a b d e g on */
    /* 3 */ 0x09,  /* a b c d g on */
    /* 4 */ 0x4D,  /* b c f g on */
    /* 5 */ 0x0D,  /* a c d f g on */
    /* 6 */ 0x05,  /* a c d e f g on */
    /* 7 */ 0x49,  /* a b c on */
    /* 8 */ 0x01,  /* all on */
    /* 9 */ 0x09,  /* a b c d f g on */
};
/* Note: CH455G segment mapping may differ. Adjust per actual display wiring.
 * The above is a generic pattern; the actual bit positions depend on the
 * CH455G datasheet and PCB routing. These are placeholders — verify on hardware. */

/* Special characters */
#define SEG_H   0x09  /* H: b c e f g */
#define SEG_DASH 0x10 /* -: g only */
#define SEG_DP  0x80  /* decimal point (bit 7) */
#define SEG_ALL_OFF 0xFF

/* CH455G I2C-like address (7-bit): 0x20 (shifted left = 0x40 write) */
#define CH455_ADDR_W  0x40

/* ---- Bit-bang I2C primitives ---- */
static void SCL_H(void) { HAL_GPIO_WritePin(CH455_GPIO, CH455_SCL_PIN, GPIO_PIN_SET); }
static void SCL_L(void) { HAL_GPIO_WritePin(CH455_GPIO, CH455_SCL_PIN, GPIO_PIN_RESET); }
static void SDA_H(void) { HAL_GPIO_WritePin(CH455_GPIO, CH455_SDA_PIN, GPIO_PIN_SET); }
static void SDA_L(void) { HAL_GPIO_WritePin(CH455_GPIO, CH455_SDA_PIN, GPIO_PIN_RESET); }
static uint8_t SDA_READ(void) { return HAL_GPIO_ReadPin(CH455_GPIO, CH455_SDA_PIN); }

static void delay_us(uint32_t us)
{
    uint32_t ticks = us * (SystemCoreClock / 1000000U) / 4;
    while (ticks--) __NOP();
}

static void i2c_start(void)
{
    SDA_H(); SCL_H(); delay_us(2);
    SDA_L(); delay_us(2);
    SCL_L(); delay_us(2);
}

static void i2c_stop(void)
{
    SDA_L(); SCL_H(); delay_us(2);
    SDA_H(); delay_us(2);
}

static void i2c_write_byte(uint8_t data)
{
    for (int i = 7; i >= 0; i--) {
        if (data & (1 << i)) SDA_H(); else SDA_L();
        delay_us(1);
        SCL_H(); delay_us(2);
        SCL_L(); delay_us(1);
    }
    /* ACK bit (ignore for CH455G) */
    SDA_H(); delay_us(1);
    SCL_H(); delay_us(2);
    SCL_L(); delay_us(1);
}

static void ch455g_write_cmd(uint8_t cmd_high, uint8_t cmd_low)
{
    i2c_start();
    i2c_write_byte(cmd_high);
    i2c_write_byte(cmd_low);
    i2c_stop();
    delay_us(50);
}

/* ---- Public Functions ---- */

void CH455G_Init(void)
{
    /* BCD mode, normal operation */
    ch455g_write_cmd(0x48, 0x01);  /* Wake up */
    /* Clear all digits */
    ch455g_write_cmd(0x60, SEG_ALL_OFF);
    ch455g_write_cmd(0x62, SEG_ALL_OFF);
    ch455g_write_cmd(0x64, SEG_ALL_OFF);
    ch455g_write_cmd(0x66, SEG_ALL_OFF);
    /* All LEDs off */
    ch455g_write_cmd(0x48, 0x0F);
}

/**
 * @brief Display 3-digit number on digit1-3
 * @param ch: 0-3 display channel number on digit0
 * @param num: 0-999
 */
void CH455G_DisplayNumber(uint8_t ch, uint16_t num)
{
    if (num > 999) num = 999;

    /* Digit 0: channel indicator */
    uint8_t d0;
    if (ch < 4)
        d0 = ~(0x40 | seg_font[ch + 1]);  /* Show '1'-'4' (active low) */
    else
        d0 = ~(0x40 | SEG_H);             /* Show 'H' */

    uint8_t d1 = ~seg_font[(num / 100) % 10];
    uint8_t d2 = ~seg_font[(num / 10) % 10];
    uint8_t d3 = ~seg_font[num % 10];

    ch455g_write_cmd(0x60, d0);
    ch455g_write_cmd(0x62, d1);
    ch455g_write_cmd(0x64, d2);
    ch455g_write_cmd(0x66, d3);
}

/**
 * @brief Set LED indicator states
 * @param b: power LED (always on)
 * @param s: mode1 LED
 * @param p: mode2 LED
 */
void CH455G_SetLED(uint8_t b, uint8_t s, uint8_t p)
{
    /* LED register: active low, bits 0-3 */
    uint8_t led_val = 0x0F;  /* all off */
    if (b) led_val &= ~0x01;
    if (s) led_val &= ~0x02;
    if (p) led_val &= ~0x04;

    ch455g_write_cmd(0x48, led_val);

    /* Also mirror to GPIO LEDs if connected directly */
    HAL_GPIO_WritePin(LED_GPIO, LED_B_PIN, b ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GPIO, LED_S_PIN, s ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GPIO, LED_P_PIN, p ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief Update display based on current mode
 */
void CH455G_DisplayUpdate(DisplayMode_t mode, uint8_t channel, uint16_t value)
{
    switch (mode) {
        case DISP_CHANNEL_1:
        case DISP_CHANNEL_2:
        case DISP_CHANNEL_3:
        case DISP_CHANNEL_4:
            CH455G_DisplayNumber(mode, value);
            CH455G_SetLED(1, (g_params.work_mode == 1), (g_params.work_mode == 2));
            break;
        case DISP_MODE_SELECT:
            /* Show "H.0", "H.1", or "H.2" */
            {
                uint8_t d0 = ~SEG_H;
                uint8_t d1 = SEG_ALL_OFF;
                uint8_t d2 = SEG_ALL_OFF;
                uint8_t d3 = ~seg_font[g_params.work_mode];
                ch455g_write_cmd(0x60, d0);
                ch455g_write_cmd(0x62, d1);
                ch455g_write_cmd(0x64, d2);
                ch455g_write_cmd(0x66, d3);
                CH455G_SetLED(1, 1, 1);  /* all LEDs on during mode select */
            }
            break;
        case DISP_PULSE_WIDTH:
            CH455G_DisplayNumber(channel, value);
            CH455G_SetLED(1, 1, 1);
            break;
    }
}
