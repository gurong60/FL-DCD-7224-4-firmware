/**
  * @file stm32g0xx_hal_flash.c - Flash HAL driver
  */
#include "stm32g0xx_hal.h"

static uint32_t FLASH_WaitForLastOperation(uint32_t Timeout)
{
    uint32_t tickstart = HAL_GetTick();

    while (FLASH->SR & FLASH_SR_BSY)
    {
        if ((HAL_GetTick() - tickstart) > Timeout)
            return HAL_TIMEOUT;
    }

    /* Check errors */
    if (FLASH->SR & (FLASH_SR_WRPERR | FLASH_SR_PROGERR))
    {
        FLASH->SR = FLASH_SR_WRPERR | FLASH_SR_PROGERR;  /* Clear flags */
        return HAL_ERROR;
    }

    if (FLASH->SR & FLASH_SR_EOP)
        FLASH->SR = FLASH_SR_EOP;  /* Clear EOP flag */

    return HAL_OK;
}

HAL_StatusTypeDef HAL_FLASH_Unlock(void)
{
    if (!(FLASH->CR & FLASH_CR_LOCK))
        return HAL_ERROR;

    FLASH->KEYR = FLASH_KEYR_KEY1;
    FLASH->KEYR = FLASH_KEYR_KEY2;

    return HAL_OK;
}

HAL_StatusTypeDef HAL_FLASH_Lock(void)
{
    FLASH->CR |= FLASH_CR_LOCK;
    return HAL_OK;
}

HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data)
{
    HAL_StatusTypeDef status = HAL_OK;

    /* Wait for last operation */
    status = FLASH_WaitForLastOperation(5000);
    if (status != HAL_OK)
        return status;

    if (TypeProgram == FLASH_TYPEPROGRAM_DOUBLEWORD)
    {
        /* Set PG bit */
        FLASH->CR |= FLASH_CR_PG;

        /* Program first word */
        *(__IO uint32_t *)Address = (uint32_t)Data;

        /* Program second word */
        *(__IO uint32_t *)(Address + 4) = (uint32_t)(Data >> 32);

        /* Wait */
        status = FLASH_WaitForLastOperation(5000);

        /* Clear PG bit */
        FLASH->CR &= ~FLASH_CR_PG;
    }

    return status;
}

HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError)
{
    HAL_StatusTypeDef status = HAL_OK;

    *PageError = 0xFFFFFFFF;

    for (uint32_t i = 0; i < pEraseInit->NbPages; i++)
    {
        /* Wait for last operation */
        status = FLASH_WaitForLastOperation(5000);
        if (status != HAL_OK)
        {
            *PageError = pEraseInit->Page + i;
            break;
        }

        /* Set PER bit */
        FLASH->CR |= FLASH_CR_PER;

        /* Set page number */
        MODIFY_REG(FLASH->CR, FLASH_CR_PNB,
                   (pEraseInit->Page + i) << FLASH_CR_PNB_Pos);

        /* Start erase */
        FLASH->CR |= FLASH_CR_STRT;

        /* Wait */
        status = FLASH_WaitForLastOperation(5000);

        /* Clear PER bit */
        FLASH->CR &= ~FLASH_CR_PER;

        if (status != HAL_OK)
        {
            *PageError = pEraseInit->Page + i;
            break;
        }
    }

    return status;
}
