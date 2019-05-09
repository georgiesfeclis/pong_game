/**
  ******************************************************************************
  * @file    main.c 
  * @author  Tomas Ambrakaitis, Anastasia Bendikow-Kokoryna, based on UEA LAB Sheet
  ******************************************************************************
  */

#include "stm32f7xx_hal.h"
#include "stm32746g_discovery_sdram.h"
#include "RTE_Components.h"
#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Board_Touch.h"
#include "Board_Buttons.h"
#include "main.h"
#include "LED.h"
#include "Tomas_GUI.h"




/* Globals */

extern GLCD_FONT     GLCD_Font_16x24; // TODO: put this variable in a .h file OR make it static and have a getter and a setter so it does not get overwritten from otther places
GameInfo thisGame;
unsigned char menuScreen; // TODO make variable static and initialise it where it is defined instead :-)
TOUCH_STATE  tsc_state; // TODO: it is declared twice... which one are you using??

#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif


static void SystemClock_Config (void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Activate the OverDrive to reach the 200 MHz Frequency */
  HAL_PWREx_EnableOverDrive();
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
}

/**
  * CPU L1-Cache enable
  */
static void CPU_CACHE_Enable (void) {

  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}



/*********************************************************************
*
*       Main
*/
int main (void) {
  CPU_CACHE_Enable();                       /* Enable the CPU Cache           */
  HAL_Init();                               /* Initialize the HAL Library     */
  BSP_SDRAM_Init();                         /* Initialize BSP SDRAM           */
  SystemClock_Config();                     /* Configure the System Clock     */
	menuScreen = 0; //  TODO: initialise up there

	
	initiliazeLED();
  
	// TODO: ALLIGN THE BLODDY FUNCTIONS!!!!!!!
	//ADC_Initialize();
  Touch_Initialize();                            /* Touchscrn Controller Init */
	GLCD_Initialize ();
  GLCD_SetBackgroundColor (GLCD_COLOR_WHITE);
  GLCD_ClearScreen (); 

	
	GUI_DrawOptionsMenu();
	
	// TODO: use while(1) instead, it's better practice.
	for (;;) {					/* loop forever */
		
		// TODO: Magic numbers? what does 0 mean? create some macros in a header file
		if(menuScreen == 0){
		Touch_GetState (&tsc_state);
		
			if(tsc_state.pressed){
				menuScreen = GUI_MainMenuTouch(&tsc_state);
			}
		
		}
		// TODO: MAGIC NUMBERS
		if(menuScreen == 1){
			GLCD_ClearScreen ();
			game_Initialize();
			GLCD_SetForegroundColor (GLCD_COLOR_BLUE);
			GLCD_SetFont (&GLCD_Font_16x24);
			GLCD_DrawChar ( thisGame.ball.x, thisGame.ball.y, 0x81); 	/* Draw Ball */
			
			// TODO: because you have another loop here , in theory you will always get stuck in this state... get rid of it...
			for(;;){
				// TODO: Delays are quite bad...
				wait_delay(thisGame.num_ticks);	
				
				/* update ball position */
				update_ball();
				update_player();
				check_collision();
				/* Draw Ball */
				GLCD_DrawChar ( thisGame.ball.x, thisGame.ball.y, 0x81); 
			}
			// TODO: MAGIC NUMBERS
		if(menuScreen == 2){
			
			GUI_DrawSettingsMenu();


			// TODO: INstead of using if statements (that are slightly wrong anyway because you should have else if instead of another conditional block) use a SWITCH
		}
		}
		
		
	
  }
}

/*************************** End of file ****************************/
