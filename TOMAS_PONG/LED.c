


// TODO why include these twice? either here OR led.h....
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "LED.h"

	GPIO_InitTypeDef gpio;

//Initilize led D0
	// TODO: put a void inside the brackets!!!!!
void initiliazeLED(){

	__HAL_RCC_GPIOC_CLK_ENABLE();	
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Pin = GPIO_PIN_7;
	
	HAL_GPIO_Init(GPIOC, &gpio);
	
	HAL_GPIO_WritePin(GPIOC, gpio.Pin, GPIO_PIN_SET);
	
}


void switchLed(unsigned char state){
	// TODO Else if?? why have 2 different if blocks? takes more time to process.
	if(state == 0){
		HAL_GPIO_WritePin(GPIOC, gpio.Pin, GPIO_PIN_SET);
	}
	
	if(state == 1){
		HAL_GPIO_WritePin(GPIOC, gpio.Pin, GPIO_PIN_RESET);
	}
	
}
