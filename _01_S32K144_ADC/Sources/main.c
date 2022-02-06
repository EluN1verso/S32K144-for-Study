/* ###################################################################
**     Filename    : main.c
**     Processor   : S32K1xx
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */

/* Including necessary module. Cpu.h contains other modules needed for
 * compiling.*/
#include "Cpu.h"
#include "clockMan1.h"
#include "delay.h"

volatile int exit_code = 0;

/* User includes (#include below this line is not maintained by Processor
 * Expert) */

#define ADC_VREFH 3.3f
#define ADC_VREFL 0.0f

uint16_t adcRawValue;
uint16_t adcMax;
float    adcVoltage;

/**
 * @brief ADC0_DataReader
 * Interrupt handler for ADC0
 * @param none
 * @return adcVolts
 * @note  This interrupt handler is called when ADC0 conversion is complete
 *       and stores the result in adcRawValue
 */
float adc0_ch2_read(void)
{
    // Initialize adcChannel
    // 0U representing software trigger
    ADC_DRV_ConfigChan(INST_ADCONV1, 0U, &adConv1_ChnConfig0);
    // System Into Suspend to Wait for ADC Conversion to Complete
    ADC_DRV_WaitConvDone(INST_ADCONV1);
    // Read ADC Conversion Result into adcRawValue
    ADC_DRV_GetChanResult(INST_ADCONV1, 0U, &adcRawValue);
    // Convert adcRawValue to Voltage
    adcVoltage = ((float)adcRawValue / adcMax) * (ADC_VREFH - ADC_VREFL);
    return adcVoltage;
}

/**
 * @brief ADC0_DataReader
 * Interrupt handler for ADC0
 * @param none
 * @return adcVolts
 * @note  This interrupt handler is called when ADC0 conversion is complete
 *       and stores the result in adcRawValue
 */
float adc0_ch3_read(void)
{
    ADC_DRV_ConfigChan(INST_ADCONV1, 0U, &adConv1_ChnConfig1);
    ADC_DRV_WaitConvDone(INST_ADCONV1);
    ADC_DRV_GetChanResult(INST_ADCONV1, 1U, &adcRawValue);
    adcVoltage = ((float)adcRawValue / adcMax) * (ADC_VREFH - ADC_VREFL);
    return adcVoltage;
}

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
    /* Write your local variable definition here */
    int   MCU_Freq;
    float adc0_ch2_value = 0;
    float adc0_ch3_value = 0;
/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by
                        the RTOS component. */
#endif
    /*** End of Processor Expert internal initialization.                    ***/
    // Initialize Clock
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
    MCU_Freq = Delay_init();
    // Initialize GPIO
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
    // Choose adcMax
    if (adConv1_ConvConfig0.resolution == ADC_RESOLUTION_8BIT) {
        adcMax = (uint16_t)(1 << 8);
    }
    else if (adConv1_ConvConfig0.resolution == ADC_RESOLUTION_10BIT) {
        adcMax = (uint16_t)(1 << 10);
    }
    else if (adConv1_ConvConfig0.resolution == ADC_RESOLUTION_12BIT) {
        adcMax = (uint16_t)(1 << 12);
    }
    // Initialize ADC
    ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvConfig0);
    // Calibrate ADC
    ADC_DRV_AutoCalibration(INST_ADCONV1);

    while (1) {
        // Read adc channel data
        adc0_ch2_value = adc0_ch2_read();
        adc0_ch3_value = adc0_ch3_read();

        // Control GPIO LED on/off on PTD15
        if (adc0_ch3_value > 1.65) {
            PINS_DRV_WritePin(PTD, 15, 0);
        }
        else {
            PINS_DRV_WritePin(PTD, 15, 1);
        }
        Delay_ms(10);
    }

    /*** Don't write any code pass this line, or it will be deleted during code
     * generation. ***/
/*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
#ifdef PEX_RTOS_START
    PEX_RTOS_START(); /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
#endif
    /*** End of RTOS startup code.  ***/
    /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
    for (;;) {
        if (exit_code != 0) {
            break;
        }
    }
    return exit_code;
    /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the NXP S32K series of microcontrollers.
**
** ###################################################################
*/
