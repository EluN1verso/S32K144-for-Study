#include "led.h"
#include "delay.h"
#include <Cpu.h>
#include <S32K144.h>

void LED_Flow(void)
{
    LED_ON(LED1);
    Delay_ms(500);
    LED_OFF(LED1);
    Delay_ms(500);
    LED_ON(LED2);
    Delay_ms(500);
    LED_OFF(LED2);
    Delay_ms(500);
    LED_ON(LED3);
    Delay_ms(500);
    LED_OFF(LED3);
    Delay_ms(500);
    LED_ON(LED4);
    Delay_ms(500);
    LED_OFF(LED4);
    Delay_ms(500);
}

void LED_AllOff(void)
{
    LED_OFF(LED1);
    LED_OFF(LED2);
    LED_OFF(LED3);
    LED_OFF(LED4);
}

void LED_AllOn(void)
{
    LED_ON(LED1);
    LED_ON(LED2);
    LED_ON(LED3);
    LED_ON(LED4);
}