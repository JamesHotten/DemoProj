/*

 */
 
#ifndef __LED_H_
#define __LED_H_

 
#include "stm32f10x.h"


#define RCC_LED_PC13         	RCC_APB2Periph_GPIOC
#define PORT_LED_PC13         GPIOC
#define GPIO_LED_PC13         GPIO_Pin_13



#define set_led_pc13(x)          	GPIO_WriteBit( PORT_LED_PC13, GPIO_LED_PC13, x?Bit_SET:Bit_RESET)


void led_init(void);

	

#endif




