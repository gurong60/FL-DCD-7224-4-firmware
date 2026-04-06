/**
 * @file buttons.c - Button scanning with debounce and long-press
 * @brief KEY_L(PB6)=CH select, KEY_M(PB7)=-, KEY_R(PB8)=+
 *
 *  Short press CH: cycle channel 1→2→3→4→mode_select→1
 *  Long press CH in mode2: toggle pulse-width adjustment
 *  Short press +/-: adjust ±1
 *  Long press +/-: rapid adjust
 *
 *  NOTE: PB8 conflicts with I2C1_SCL in some alternate configurations.
 *        This firmware uses PB10/PB11 bit-bang for CH455G display,
 *        so PB8 is free for KEY_R. Verified from schematic.
 */
#include "main.h"

typedef enum {
    BTN_IDLE = 0,
    BTN_DEBOUNCE,
    BTN_PRESSED,
    BTN_LONG,
} BtnState_t;

typedef struct {
    GPIO_TypeDef *port;
    uint16_t      pin;
    BtnState_t    state;
    uint32_t      press_tick;
    uint32_t      repeat_tick;
} Button_t;

static Button_t btn_l = { KEY_GPIO, KEY_L_PIN, BTN_IDLE, 0, 0 };
static Button_t btn_m = { KEY_GPIO, KEY_M_PIN, BTN_IDLE, 0, 0 };
static Button_t btn_r = { KEY_GPIO, KEY_R_PIN, BTN_IDLE, 0, 0 };

/* Current display state */
static DisplayMode_t disp_mode = DISP_CHANNEL_1;
static uint8_t  cur_channel   = 0;
static bool     in_pulse_mode = false;

/* ---- Read button (active low) ---- */
static inline bool btn_read(Button_t *b)
{
    return (HAL_GPIO_ReadPin(b->port, b->pin) == GPIO_PIN_RESET);
}

/* ---- Shared brightness/mode adjustment ---- */
static void do_adjust(Button_t *b, bool is_positive)
{
    if (disp_mode == DISP_MODE_SELECT) {
        if (is_positive) {
            g_params.work_mode++;
            if (g_params.work_mode > 2) g_params.work_mode = 0;
        } else {
            if (g_params.work_mode == 0) g_params.work_mode = 2;
            else g_params.work_mode--;
        }
        CH455G_DisplayUpdate(DISP_MODE_SELECT, 0, 0);
        Params_RequestSave();
    } else if (disp_mode == DISP_PULSE_WIDTH && in_pulse_mode) {
        if (is_positive) {
            if (g_params.pulse_width[cur_channel] < 999)
                g_params.pulse_width[cur_channel]++;
        } else {
            if (g_params.pulse_width[cur_channel] > 0)
                g_params.pulse_width[cur_channel]--;
        }
        CH455G_DisplayUpdate(DISP_PULSE_WIDTH, cur_channel,
                             g_params.pulse_width[cur_channel]);
        Params_RequestSave();
    } else if (cur_channel < 4) {
        if (is_positive) {
            if (g_params.brightness[cur_channel] < 999)
                g_params.brightness[cur_channel]++;
        } else {
            if (g_params.brightness[cur_channel] > 0)
                g_params.brightness[cur_channel]--;
        }
        Set_PWM_Duty(cur_channel, g_params.brightness[cur_channel]);
        CH455G_DisplayUpdate(disp_mode, cur_channel,
                             g_params.brightness[cur_channel]);
        Params_RequestSave();
    }
}

/* ---- Button state machine ---- */
static void btn_update(Button_t *b)
{
    bool pressed = btn_read(b);

    switch (b->state) {
        case BTN_IDLE:
            if (pressed) {
                b->state = BTN_DEBOUNCE;
                b->press_tick = g_tick_ms;
            }
            break;

        case BTN_DEBOUNCE:
            if (g_tick_ms - b->press_tick >= DEBOUNCE_MS) {
                if (pressed) {
                    b->state = BTN_PRESSED;
                    b->repeat_tick = g_tick_ms;
                } else {
                    b->state = BTN_IDLE;
                }
            }
            break;

        case BTN_PRESSED:
            if (!pressed) {
                /* ---- SHORT PRESS release ---- */
                b->state = BTN_IDLE;
                if (b == &btn_l) {
                    /* CH button: cycle channel */
                    if (in_pulse_mode) {
                        in_pulse_mode = false;
                        disp_mode = (DisplayMode_t)(DISP_CHANNEL_1 + cur_channel);
                    } else {
                        cur_channel++;
                        if (cur_channel > 4) cur_channel = 0;
                        if (cur_channel == 4) {
                            disp_mode = DISP_MODE_SELECT;
                        } else {
                            disp_mode = (DisplayMode_t)(DISP_CHANNEL_1 + cur_channel);
                        }
                    }
                    /* Update display */
                    if (disp_mode == DISP_MODE_SELECT)
                        CH455G_DisplayUpdate(DISP_MODE_SELECT, 0, 0);
                    else if (in_pulse_mode)
                        CH455G_DisplayUpdate(DISP_PULSE_WIDTH, cur_channel,
                                             g_params.pulse_width[cur_channel]);
                    else
                        CH455G_DisplayUpdate(disp_mode, cur_channel,
                                             g_params.brightness[cur_channel]);
                } else {
                    /* +/- buttons: single step */
                    do_adjust(b, (b == &btn_r));
                }
                return;
            }
            /* Transition to long press */
            if (g_tick_ms - b->press_tick >= LONG_PRESS_MS) {
                b->state = BTN_LONG;
            }
            break;

        case BTN_LONG:
            if (!pressed) {
                /* ---- LONG PRESS release ---- */
                b->state = BTN_IDLE;
                if (b == &btn_l) {
                    /* Long CH: toggle pulse-width mode in mode2 */
                    if (g_params.work_mode == MODE_TRIGGER_2) {
                        in_pulse_mode = !in_pulse_mode;
                        if (in_pulse_mode) {
                            disp_mode = DISP_PULSE_WIDTH;
                            CH455G_DisplayUpdate(DISP_PULSE_WIDTH, cur_channel,
                                                 g_params.pulse_width[cur_channel]);
                        } else {
                            disp_mode = (DisplayMode_t)(DISP_CHANNEL_1 + cur_channel);
                            CH455G_DisplayUpdate(disp_mode, cur_channel,
                                                 g_params.brightness[cur_channel]);
                        }
                    }
                }
                return;
            }
            /* Long-press: rapid repeat for +/- buttons */
            if (b == &btn_r || b == &btn_m) {
                if (g_tick_ms - b->repeat_tick >= RAPID_STEP_MS) {
                    b->repeat_tick = g_tick_ms;
                    do_adjust(b, (b == &btn_r));
                }
            }
            break;
    }
}

/* ---- Public ---- */
void Buttons_Init(void)
{
    disp_mode = DISP_CHANNEL_1;
    cur_channel = 0;
    in_pulse_mode = false;
}

void Buttons_Process(void)
{
    btn_update(&btn_l);
    btn_update(&btn_m);
    btn_update(&btn_r);
}
