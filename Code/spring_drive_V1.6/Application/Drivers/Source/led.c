#include  "led.h"


void led_init(void)
{
    GPIO_Init(LED_PART, LED_PIN,DIR_OUTPUR,PULL_UP_DISALBE,PULL_DOWN_DISALBE,PUSH_PULL);			//Êä³ö£¬ ½ÓLED
    LED_CLOSE();

}

