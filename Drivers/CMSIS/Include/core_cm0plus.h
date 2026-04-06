/**
  * @file core_cm0plus.h - Minimal CMSIS Core definitions for Cortex-M0+
  */
#ifndef __CORE_CM0PLUS_H
#define __CORE_CM0PLUS_H

#include <stdint.h>

/* SysTick configuration (from CMSIS) */
__attribute__((always_inline)) static inline uint32_t SysTick_Config(uint32_t ticks)
{
    if ((ticks - 1UL) > 0x00FFFFFFUL)
        return 1UL;

    SysTick->LOAD = (uint32_t)(ticks - 1UL);
    NVIC_SetPriority(SysTick_IRQn, (1UL << 2) - 1UL);
    SysTick->VAL  = 0UL;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;
    return 0UL;
}

#endif /* __CORE_CM0PLUS_H */
