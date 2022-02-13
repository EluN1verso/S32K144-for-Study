#ifndef LED_H_
#define LED_H_

#define LED1 0
#define LED2 1
#define LED3 15
#define LED4 16

#define LED_ON(led) PINS_DRV_WritePin(PTD, led, 1)
#define LED_OFF(led) PINS_DRV_WritePin(PTD, led, 0)

void LED_Flow(void);
void LED_AllOff(void);
void LED_AllOn(void);
#endif /* LED_H_ */