/**
 * @file params.c - Parameter persistence with 30-second delayed Flash save
 * @brief Stores brightness, pulse_width, work_mode in internal Flash
 *
 *  DIFFERENCE FROM MANUAL: Save is delayed by 30 seconds after the last
 *  parameter change (via button OR RS232). This coalesces rapid changes
 *  into a single Flash write to extend Flash endurance.
 *
 *  STM32G030C8T6: 64KB Flash, 2KB page size
 *  Parameter storage: page 31 at 0x0800F800 (last 2KB)
 *  Flash endurance: ~10,000 cycles → delayed save greatly extends life
 */
#include "main.h"

/* ---- Global parameters ---- */
ParamStore_t g_params = {
    .brightness  = {500, 500, 500, 500},
    .pulse_width = {0, 0, 0, 0},
    .work_mode   = MODE_ALWAYS_ON,
    .padding     = {0, 0, 0},
    .magic       = 0x4C454421,  /* "LED!" */
};

static volatile bool     save_requested = false;
static volatile uint32_t save_tick      = 0;

/* ---- Default parameters ---- */
static const ParamStore_t default_params = {
    .brightness  = {500, 500, 500, 500},
    .pulse_width = {0, 0, 0, 0},
    .work_mode   = MODE_ALWAYS_ON,
    .padding     = {0, 0, 0},
    .magic       = 0x4C454421,
};

/**
 * @brief Load parameters from Flash
 */
void Params_Load(void)
{
    const ParamStore_t *flash = (const ParamStore_t *)FLASH_STORAGE_ADDR;

    if (flash->magic == 0x4C454421) {
        /* Valid data — copy */
        memcpy(&g_params, flash, sizeof(ParamStore_t));
    } else {
        /* First run or corrupted — use defaults */
        memcpy(&g_params, &default_params, sizeof(ParamStore_t));
    }
}

/**
 * @brief Write parameters to Flash (erases page first)
 *        STM32G030: programs in 64-bit (8-byte) double-words
 */
void Params_Save(void)
{
    HAL_StatusTypeDef status;

    HAL_FLASH_Unlock();

    /* Erase the last page (page 31 = 0x0800F800, 2KB) */
    FLASH_EraseInitTypeDef erase = {
        .TypeErase  = FLASH_TYPEERASE_PAGES,
        .Page       = (FLASH_STORAGE_ADDR - FLASH_BASE) / FLASH_PAGE_SIZE,
        .NbPages    = 1,
    };
    uint32_t page_error = 0;
    status = HAL_FLASHEx_Erase(&erase, &page_error);
    if (status != HAL_OK) {
        HAL_FLASH_Lock();
        return;
    }

    /* Program in 64-bit (8-byte) double-words */
    const uint64_t *src  = (const uint64_t *)&g_params;
    uint32_t        addr = FLASH_STORAGE_ADDR;
    uint32_t        words = (sizeof(ParamStore_t) + 7) / 8;  /* round up */

    for (uint32_t i = 0; i < words; i++) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,
                                   addr + i * 8, src[i]);
        if (status != HAL_OK)
            break;
    }

    HAL_FLASH_Lock();
}

/**
 * @brief Called by button/RS232 handlers after parameter change.
 *        Resets the 30-second save timer.
 */
void Params_RequestSave(void)
{
    save_requested = true;
    save_tick = g_tick_ms;
}

/**
 * @brief Called from main loop every iteration.
 *        Checks if 30 seconds have elapsed since last change.
 */
void Params_SaveTick(void)
{
    if (!save_requested)
        return;

    if ((g_tick_ms - save_tick) >= SAVE_DELAY_MS) {
        save_requested = false;
        Params_Save();
    }
}
