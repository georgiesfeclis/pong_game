/*--------------------------------------------------
 * Name:    main.h
 * Purpose: pong function prototypes and defs
 *--------------------------------------------------
 *
 * Modification History
 * 06.02.14 Created
 * 09.12.15 Updated (uVision5.17 + DFP2.6.0)
 *
 * Dr Mark Fisher, CMP, UEA, Norwich, UK
 *--------------------------------------------------*/
#ifndef _MAIN_H
#define _MAIN_H

#define wait_delay HAL_Delay // TODO: what's the point of this? why would you have it as a macro? it doesn't make any sense..
#define WIDTH		GLCD_WIDTH
#define HEIGHT	GLCD_HEIGHT
#define CHAR_H  GLCD_Font_16x24.height                  /* Character Height (in pixels) */
#define CHAR_W  GLCD_Font_16x24.width                  /* Character Width (in pixels)  */
// TODO: allign stuff please. .... makes stuff easier to read.
#define BAR_W   6									  /* Bar Width (in pixels) */
#define BAR_H		24				          /* Bar Height (in pixels) */
#define T_LONG	1000                /* Long delay */ // TODO: put some units, 1000 micro seconds? nano secons? minutes? what are they?
#define T_SHORT 5                   /* Short delay */

typedef struct {
	  int dirn;
	  int x; 
	  int y; 
	} BallInfo;

typedef struct {
	  int x;
	  int y;
	} PaddleInfo;

typedef struct {
	unsigned int num_ticks;
	BallInfo ball;
	PaddleInfo p1;
  } GameInfo;

/* Function Prototypes */

  // TODO: Why are these functions declared here? they are defined in pong utils. put them in pong_utils.h????
  // TODO: technically you shouldn't have a main.h at all.... but some IDEs create them anyway..
  // it's your choice whether to keep it or not but put the declarations below where they belong.
void game_Initialize(void);
unsigned char update_ball (void);
void update_player (void);
void check_collision (void);	

#endif /* _MAIN_H */
