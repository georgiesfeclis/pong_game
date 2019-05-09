/*--------------------------------------------------
 * Pong game system based on UEA LAB SHEET
 * Name:    pong_utils.c
 * Purpose: Pong Prototype
 * 
 *--------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "main.h"
#include "Board_ADC.h"
#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Board_Touch.h"
#include "pong_utils.h"
#include "LED.h"
#include "stm32f7xx_hal_gpio.h"
#include <stdio.h>
#include "keypad.h"

/* Globals */
// TODO: global variables... try to either make the local (within functions) or static (so they can only be used in this file
extern GLCD_FONT     GLCD_Font_16x24;
extern GLCD_FONT     GLCD_customFont_16x24;
extern GameInfo thisGame;
BallInfo init_pstn;
// TODO magic numbers?
char output[2];
uint8_t gamePoints;


/****************************************************
*  game_Init()
*  Initialize some game parameters.
*****************************************************/
void game_Initialize(void) {
	// TODO: magic numbers, what is 1 in this case?
	init_pstn.dirn = 1;
	init_pstn.x = (WIDTH-CHAR_W)/2;
	init_pstn.y = (HEIGHT-CHAR_H)/2;
	thisGame.ball = init_pstn;
	thisGame.p1.x = 0;
	thisGame.p1.y = 0;
	thisGame.num_ticks = T_SHORT;
	initializeMembranePins();
	gamePoints = 3;
}




/***************************************************
*  undate_ball()
*  update the ball pstn - depending on dirn of travel
****************************************************/
unsigned char update_ball(void) {
	switch (thisGame.ball.dirn) {
		case 0: thisGame.ball.x++;
						break;
		case 1: thisGame.ball.x++;
						thisGame.ball.y--;
						break;
		case 2: thisGame.ball.y--;
						break;
		case 3: thisGame.ball.x--;
						thisGame.ball.y--;
						break;
		case 4: thisGame.ball.x--;
						break;
		case 5: thisGame.ball.x--;
						thisGame.ball.y++;
						break;
		case 6: thisGame.ball.y++;
						break;
		case 7: thisGame.ball.x++;
						thisGame.ball.y++;			
						// TODO default case..
	}
	if (thisGame.ball.x<BAR_W) {	 /* reset position */
		gamePoints--;
		if(gamePoints == 0){
			GLCD_DrawString (0, 0*24, "!!!!!!!!!!!GAME OVER!!!!!!!!!!!");
			GLCD_DrawString (0, 1*24, "!!!!!!!!!!!GAME OVER!!!!!!!!!!!");
			GLCD_DrawString (0, 2*24, "!!!!!!!!!!!GAME OVER!!!!!!!!!!!");
			GLCD_DrawString (0, 3*24, "!!!!!!!!!!!GAME OVER!!!!!!!!!!!");
			GLCD_DrawString (0, 4*24, "!!!!!!!!!!!GAME OVER!!!!!!!!!!!");
			GLCD_DrawString (0, 5*24, "!!!!!!!!!!!GAME OVER!!!!!!!!!!!");
			GLCD_DrawString (0, 6*24, "!!!!!!!!!!!GAME OVER!!!!!!!!!!!");
			GLCD_DrawString (0, 7*24, "!!!!!!!!!!!GAME OVER!!!!!!!!!!!");
			GLCD_DrawString (0, 8*24, "!!!!!!!!!!!GAME OVER!!!!!!!!!!!");
			GLCD_DrawString (0, 9*24, "!!!!!!!!!!!GAME OVER!!!!!!!!!!!");
			GLCD_DrawString (0, 10*24, "!!!!!!!!!!!GAME OVER!!!!!!!!!!!");
			HAL_Delay(5000);
			GLCD_ClearScreen();
		}
		
		// TODO: is this delay really needed? it might cause issues.
    wait_delay(T_LONG);
    /* Erase Ball */
    GLCD_DrawChar ( thisGame.ball.x, thisGame.ball.y, ' '); 	
		thisGame.ball = init_pstn;
	
	}
		return 0; //TODO:  why do you need a return value if it's always gonna be 0? make this function a void
}

/****************************************************
* update_player(unsigned int *)
* Read the ADC and draw the player 1's paddle				
*****************************************************/
void update_player(void) {
	int8_t membraneValue;
  int16_t paddleValue; 
	int8_t bufferedValue;
  static int lastValue = 0;
  // TODO: this variable is defined in multiple paces. why? this one is ok cause it's in a function. what about the rest?
  TOUCH_STATE  tsc_state;
  
  Touch_GetState (&tsc_state); /* Get touch state */   
  if (tsc_state.pressed) {
		if(tsc_state.y != 0){
			paddleValue =  tsc_state.y;
		}
  }

	membraneValue = getInput();
	if(membraneValue != -1){
		GLCD_SetForegroundColor (GLCD_COLOR_BLUE);
		GLCD_SetFont (&GLCD_Font_16x24);
		snprintf(output, 50, "%i", membraneValue);
		
		GLCD_DrawString (0, 5*24, output);
		// TODO magic numbers and default state at the end of switch pleasee :-)
		
		switch(membraneValue){
			case 1:
				paddleValue = (paddleValue + (HEIGHT / 7));
			break;
			case 2:
				paddleValue = (paddleValue + (HEIGHT / 6));
			break;
			case 3:
				paddleValue = (paddleValue + (HEIGHT / 5));
			break;
			case 4:
				paddleValue = (paddleValue + (HEIGHT / 4));
			break;
			case 5:
				paddleValue = (paddleValue + (HEIGHT / 3));
			break;
			case 6:
				paddleValue = (paddleValue + (HEIGHT / 2));
			break;
		}
	}
	paddleValue = paddleValue * (HEIGHT-BAR_H)/HEIGHT;
  /* Erase Paddle */
	GLCD_DrawChar (0, lastValue, ' '); 	
  /* Draw Paddle */
  GLCD_SetFont (&GLCD_customFont_16x24);
	GLCD_DrawChar (0, paddleValue, 0x00 ); 	
  GLCD_SetFont (&GLCD_Font_16x24);
	lastValue = paddleValue;
	thisGame.p1.y = paddleValue;
} 

/****************************************************
*  check_collision(void)
*  check for contact between ball and screen edges
*  and paddle and change direction accordingly
*****************************************************/
void check_collision(void) {
  /* check collision with RH */
  if ((thisGame.ball.x==BAR_W) || 
       thisGame.ball.x==(WIDTH-CHAR_W)) {  
	  // TODO magic numbers and default state pls.. the if statements shouldn't be in the same line with the casee.
	  // TODO the whole switch should be a different functions.. for x and y coordinates.
		switch (thisGame.ball.dirn)   /* vert scrn edge */
		{                             /*   or P1 paddle */
			case 0: thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
							break;
			case 1: thisGame.ball.dirn = (thisGame.ball.dirn+2)%8;
							break;
			case 3: if ( (thisGame.ball.y>=thisGame.p1.y-CHAR_H) && (thisGame.ball.y<=(thisGame.p1.y+BAR_H)) )
								thisGame.ball.dirn = (thisGame.ball.dirn+6)%8;
							else 
					      /* empty statement */; 	
							break;
			case 4: if ( (thisGame.ball.y>=thisGame.p1.y-CHAR_H) && (thisGame.ball.y<=(thisGame.p1.y+BAR_H)) )
						    thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
							else
								/* empty statement */;
					    break;
			case 5: if ( (thisGame.ball.y>=thisGame.p1.y-CHAR_H) && (thisGame.ball.y<=(thisGame.p1.y+BAR_H)) ) 
						    thisGame.ball.dirn = (thisGame.ball.dirn+2)%8;
					    else 
								GLCD_SetForegroundColor (GLCD_COLOR_BLUE);
								GLCD_SetFont (&GLCD_Font_16x24);
								GLCD_DrawString (0, 4*24, "BALL LOST");
								switchLed(1);
								HAL_Delay(3000);
								GLCD_ClearScreen();
								switchLed(0);
						    /* empty statement */;
					    break;
			case 7: thisGame.ball.dirn = (thisGame.ball.dirn+6)%8;
							break;
			}
	}
  /* check collision with horiz scrn edge */
  // TODO why is this not and else if..?
	if ((thisGame.ball.y<0) || 
       thisGame.ball.y>(HEIGHT-CHAR_H)) { 
    switch (thisGame.ball.dirn)     
		{
			case 1: thisGame.ball.dirn = (thisGame.ball.dirn+6)%8;
							thisGame.ball.y++;
							break;
			case 2: thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
							thisGame.ball.y++;
							break;
			case 3: thisGame.ball.dirn = (thisGame.ball.dirn+2)%8;
							thisGame.ball.y++;
							break;
			case 5: thisGame.ball.dirn = (thisGame.ball.dirn+6)%8;
							thisGame.ball.y--;
							break;
			case 6: thisGame.ball.dirn = (thisGame.ball.dirn+4)%8;
							thisGame.ball.y--;
							break;
			case 7: thisGame.ball.dirn = (thisGame.ball.dirn+2)%8;
							thisGame.ball.y--;
							break;
		}
	}
}

