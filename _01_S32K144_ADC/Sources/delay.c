/*
 * delay.c
 *
 *  Created on: 2022年2月6日
 *      Author: Kylin
 */

#include "delay.h"
#include "S32K144.h"
#include "S32K144_features.h"
#include <Cpu.h>

static uint32_t fac_us;

/**
 * @brief	Initialize delay routine
 * @param	None
 * @return	None
 * @note	This function must be called before delay_us() or delay_ms()
 *          RVR is set to 0xFFFFFFFF is 16, 777*216hz = 16.7055mhz
 *          16.7055mhz/48mhz≈0.348s=348ms
 */
void SysTick_Init(void)
{
    // SysTick Reload Value Register
    S32_SysTick->RVR = 0xFFFFFFul;
    // SysTick Current Value Register
    S32_SysTick->CVR = 0ul;
    // SysTick Control and Status Register
    S32_SysTick->CSR = 0u;
}

/**
 * @brief  Enable System Tick counter and interrupt
 * @param  None
 * @return None
 * @note   None
 */
void SysTick_Enable(void)
{
    S32_SysTick->CSR = S32_SysTick_CSR_ENABLE(1) | S32_SysTick_CSR_TICKINT(1u);
}

/**
 * @brief  Disable System Tick counter and interrupt
 * @param  None
 * @return None
 * @note   None
 */
void SysTick_Disable(void)
{
    S32_SysTick->CSR = S32_SysTick_CSR_ENABLE(0) | S32_SysTick_CSR_TICKINT(0u);
}

/**
 * @brief  Delay us function
 * @param  us Delay time in us
 * @return None
 * @note   This function must be called after SysTick_Init()
 */
void Delay_us(uint32_t us)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    // Load the counter value that is equal to the number of clock cycles for 1us
    uint32_t reload = S32_SysTick->RVR;
    // Get tick needed to achieve the delay
    ticks = us * fac_us;
    // Get current value of the system tick timer
    told = S32_SysTick->CVR;
    while (1) {
        tnow = S32_SysTick->CVR;
        if (tnow != told) {
            if (tnow < told) {
                // Systick counter is roll over
                tcnt += told - tnow;
            }
            else {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            // If count and tick exceeded or euqal, break the loop
            if (tcnt >= ticks) {
                break;
            }
        }
    };
}

/**
 * @brief  Delay ms function
 * @param  ms Delay time in ms
 * @return None
 * @note   This function must be called after SysTick_Init()
 */
void Delay_ms(uint32_t ms)
{
    uint32_t i;
    for (i = 0; i < ms; i++) {
        delay_us(1000);
    }
}

/**
 * @brief   Initialize delay frequency
 * @param   None
 * @return  fact count in one us
 * @note    This function must be called after SystemCoreClockUpdate()
 *       and before delay_us() or delay_ms()
 */
int Delay_Init(void)
{
    uint32_t frequency;
    CLOCK_SYS_GetFreq(CORE_CLOCK, &frequency);
    // fact count in one us
    fac_us = frequency / 1000000;
    SysTick_Init();
    SysTick_Enable();
    return fac_us;
}