/**
 * @file trigger.c - External trigger input handling
 * @brief PA4-PA7 = TRIG1-TRIG4 (active high via optocoupler)
 *
 *  Work Mode 0 (Always On):  Ignored — LEDs always on at set brightness
 *  Work Mode 1 (Trigger 1):  High = LED on, Low = LED off
 *  Work Mode 2 (Trigger 2):  Rising edge → LED on for pulse_width[ch] ms
 *
 *  Mode 2 pulse timing uses the 1ms SysTick for simplicity.
 */
#include "main.h"

static uint32_t pulse_start[4] = {0};
static bool     pulse_active[4] = {false};
static bool     trig_prev[4]    = {false};  /* for edge detection */

/* ---- Read trigger input (returns true if high = triggered) ---- */
static inline bool trig_read(uint8_t ch)
{
    uint16_t pins[] = {TRIG1_PIN, TRIG2_PIN, TRIG3_PIN, TRIG4_PIN};
    return (HAL_GPIO_ReadPin(TRIG_GPIO, pins[ch]) == GPIO_PIN_SET);
}

void Trigger_Init(void)
{
    for (int i = 0; i < 4; i++) {
        pulse_active[i] = false;
        trig_prev[i]    = false;
    }
}

void Trigger_Process(void)
{
    /* In always-on mode, do nothing */
    if (g_params.work_mode == MODE_ALWAYS_ON)
        return;

    for (uint8_t ch = 0; ch < 4; ch++) {
        bool trig_now = trig_read(ch);

        if (g_params.work_mode == MODE_TRIGGER_1) {
            /* ---- Level triggered: follow trigger level ---- */
            if (trig_now) {
                /* Trigger high → output ON at set brightness */
                Set_PWM_Duty(ch, g_params.brightness[ch]);
            } else {
                /* Trigger low → output OFF */
                Set_PWM_Duty(ch, 0);
            }
        }
        else if (g_params.work_mode == MODE_TRIGGER_2) {
            /* ---- Edge triggered: pulse output ---- */

            /* Rising edge detection */
            bool rising_edge = (trig_now && !trig_prev[ch]);

            if (rising_edge && !pulse_active[ch]) {
                /* Start pulse */
                uint16_t pw = g_params.pulse_width[ch];
                if (pw == 0) {
                    /* Pulse width 0 → instant flash (minimum on-time) */
                    Set_PWM_Duty(ch, g_params.brightness[ch]);
                    /* Will be turned off next iteration */
                    pulse_start[ch] = g_tick_ms;
                    pulse_active[ch] = true;
                } else {
                    Set_PWM_Duty(ch, g_params.brightness[ch]);
                    pulse_start[ch] = g_tick_ms;
                    pulse_active[ch] = true;
                }
            }

            /* Check pulse timeout */
            if (pulse_active[ch]) {
                uint16_t pw = g_params.pulse_width[ch];
                if (pw == 0) {
                    /* Immediate off */
                    Set_PWM_Duty(ch, 0);
                    pulse_active[ch] = false;
                } else if ((g_tick_ms - pulse_start[ch]) >= pw) {
                    Set_PWM_Duty(ch, 0);
                    pulse_active[ch] = false;
                }
            }

            trig_prev[ch] = trig_now;
        }
    }
}
