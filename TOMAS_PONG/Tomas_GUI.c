#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Tomas_GUI.h"
#include "Board_Touch.h"
#include <stdio.h>
#include "stm32f7xx_hal.h"

/**
 * @brief Menu button working structure
 */
typedef struct{
	uint16_t xCord;											/*!< Distance measured in centimeters */
	uint8_t yCord;						/*!< GPIO Base for ECHO pin. For private use only*/
	uint16_t width;							/*!< GPIO Pin  for ECHO pin. For private use only */
	uint8_t height;				/*!< GPIO Base for TRIGGER pin. For private use only */
} Button;

//External Fonts
// TODO: why use extern stuff...... it can be ovverwritten without you knowing !!
extern GLCD_FONT     GLCD_Font_6x8;
extern GLCD_FONT     GLCD_Font_16x24;

//Constants 
// TODO if that is always 50 why not make it a #define??
const unsigned char textPadding = 50;
//TODO: none of these are constant.. just saying..
unsigned char options = 0;
unsigned char armed = 0;
unsigned char typeNo;
unsigned char typeMenu;

//Private variables
// TODO: it says private variables but none of them are static == private?
unsigned char buttonPadding;
Button menuButton;
Button settings;


////YUP // todo: wha?
// TODO put a void in those brackets.. my eyes are hurting..
void GUI_InitializeMainMenuButtonParameters(){
	menuButton.height = 50;
	menuButton.width  = 255;
	menuButton.xCord  = 112;
	menuButton.yCord  = 75;
	
	settings.height = 50;
	settings.width  = 50;
	settings.xCord  = GLCD_SIZE_X - 55;
	settings.yCord  = GLCD_SIZE_Y - 55;
	
	buttonPadding = 60;
}

// TODO add void in bracketsss
void GUI_Settings(){
	
	GLCD_SetBackgroundColor (GLCD_COLOR_BLUE);
	GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
	GLCD_SetFont (&GLCD_Font_16x24);
	GLCD_DrawString (0, 0*24, "            SETTINGS            ");
	GLCD_DrawString (0, 1*24, "                                ");
	GLCD_DrawString (0, 2*24, "                                ");
	
	GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	GLCD_SetFont (&GLCD_Font_16x24);
	
	GLCD_DrawString(menuButton.xCord + textPadding, menuButton.yCord + 16, "INPUT GAME COUNT");
}

//TODO  is good practice to call the pointer " pTsc_state" so you know within the function which data is coming from the pointer...
// TODO also,, it should be a (const TOUCH_State const * tsc_state)
unsigned char GUI_MainMenuTouch(TOUCH_STATE  *tsc_state) {

  int xCord = tsc_state->x;
	int yCord = tsc_state->y;
	
	// TODO why not use and else if...
	// TODO single point of return.. add a retVal.
	if(xCord >= menuButton.xCord &&
		xCord  <= menuButton.xCord + menuButton.width  &&
		yCord  >= menuButton.yCord &&
		yCord  <= menuButton.yCord + menuButton.height){
			return 1;
	}
		
	if(xCord >= settings.xCord &&
		xCord  <= settings.xCord + settings.width  &&
		yCord  >= settings.yCord &&
		yCord  <= settings.yCord + settings.height){
					GLCD_DrawString (0, 0*24, "            SETTINGS           			 ");
					GLCD_DrawString (0, 1*24, "            SETTINGS                	 ");
					GLCD_DrawString (0, 2*24, "            SETTINGS                  ");
			return 2;
	}

	return 0;
}

void GUI_DrawOptionsMenu(){
	GLCD_ClearScreen ();
	GUI_InitializeMainMenuButtonParameters();
	GUI_DrawOptionsButtons();
	GUI_DrawOptionsText();
}

void GUI_DrawSettingsMenu(){
	GLCD_ClearScreen ();
	GUI_Settings();
}



void GUI_DrawOptionsButtons(){
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
	// TODO: these coordinates are global variables and can be ovverwritten without your knowledge... hence why your button goes to random places...
	GLCD_DrawRectangle (menuButton.xCord, menuButton.yCord, menuButton.width, menuButton.height);	
	GLCD_DrawRectangle (menuButton.xCord, menuButton.yCord+buttonPadding, menuButton.width, menuButton.height);	
}

void GUI_DrawOptionsText(){
	GLCD_SetBackgroundColor (GLCD_COLOR_BLUE);
	GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
	GLCD_SetFont (&GLCD_Font_16x24);
	GLCD_DrawString (0, 0*24, "          MAIN MENU            ");
	GLCD_DrawString (0, 1*24, "                                ");
	GLCD_DrawString (0, 2*24, "                                ");
	
	GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	GLCD_SetFont (&GLCD_Font_16x24);
	
	GLCD_DrawString(menuButton.xCord + textPadding, menuButton.yCord + 16, "START GAME");
	GLCD_DrawString(menuButton.xCord + textPadding, menuButton.yCord + buttonPadding + 16, "SETTINGS");
}

// TODO VOID in brackets plss
void GUI_DrawTypeMenu(){
	GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
	GLCD_DrawRectangle (0, -1*(50-GLCD_SIZE_Y)-5 , 50, 50);	
}
void GUI_DrawAccordingToType(unsigned char typeNumber){	
	GLCD_ClearScreen();
	GLCD_SetBackgroundColor (GLCD_COLOR_BLUE);
	GLCD_SetForegroundColor (GLCD_COLOR_WHITE);
	typeMenu = 1;
	GUI_DrawTypeMenu();
	switch(typeNumber){
		case 1:
			GLCD_DrawString (0, 0*24, "                                ");
			GLCD_DrawString (0, 0*24, "         START GAME          	");
		break;
		case 2:
			GLCD_DrawString (0, 0*24, "                                ");
			GLCD_DrawString (0, 0*24, "          SETTINGS           ");
		break;
		// TODO: default state..
	}

	
}
