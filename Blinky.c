/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher and Graphic Demo
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define __FI 1
#define __USE_LCD 0

//extern unsigned char ClockLEDOn;
//extern unsigned char ClockLEDOff;
//extern unsigned char ClockANI;

// Bit Band Macros used to calculate the alias address at run time
#define ADDRESS(x)    (*((volatile unsigned long *)(x)))
#define BitBand(x, y) 	ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))
volatile unsigned long * bit1;
volatile unsigned long * bit2;
volatile unsigned long * bit3;
volatile unsigned long * bit4;
volatile unsigned long * bit5;
volatile unsigned long * bit6;
volatile unsigned long * bit7;

extern unsigned char Bg_16bpp_t[];
extern unsigned char Bg_16bpp_l[];
extern unsigned char Bg_16bpp_r[];
extern unsigned char Bg_16bpp_b[];
extern unsigned char ARM_Ani_16bpp[];

extern unsigned char img_1[320 * 173 * 2 + 1];
extern unsigned char img_2[320 * 173 * 2 + 1];
extern unsigned char img_3[320 * 171 * 2 + 1];
extern unsigned char game_img[250 * 170 * 2 + 1];

int gallery(void);
int music_player(void);
int speaker_menu(void);
int LCD(int menu_count);

int game_menu(void);
int game(void);
int singleplayer(int score_p1, int score_p2, int roundnum);
int multiplayer(int score_p1, int score_p2, int roundnum);
int determrps(int choice_p1, int choice_p2, int count_p1, int count_p2, int round_num);
int win(int score_p1, int score_p2, int round_num);
int lose(int score_p1, int score_p2, int round_num);
int tie(int score_p1, int score_p2, int round_num);
int win_final_multiplayer(int score_p1, int score_p2, int round_num);
int win_final_singleplayer(int score_p1, int score_p2, int round_num);

int p2_select(int choice_p1, int score_p1, int score_p2, int roundnum);
int multi_determrps(int choice_p1, int choice_p2, int count_p1, int count_p2, int round_num);


extern int audio(void);

int flag = 0;
int round_num = 0;

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {                       /* Main Program                       */
	
	// joystick value variables
	int js = 0;
	int js_last = 0;
	int menu_count = 2;
	
	// turn on LEDS on board (manual)
	LPC_SC->PCONP     |= (1 << 15);  //enable power to GPIO & IOCON
	
	LPC_GPIO1->FIODIR |= 0xB0000000; //P1 Leds
  LPC_GPIO2->FIODIR |= 0x0000007C; //P2 Leds
	
	// initialize LED/LCD once
	if(flag == 0) {
		GLCD_Init(); 
		LED_Init ();
		flag = 1;
	}                          

  GLCD_Clear(White);      
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "COE718 Project      ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "Kacper Burza        ");
  GLCD_DisplayString(2, 0, __FI, "Menu Select:        ");
	GLCD_SetBackColor(White);
  GLCD_SetTextColor(Black);
	GLCD_DisplayString(4, 0, __FI, "Gallery             ");
	GLCD_DisplayString(5, 0, __FI, "Speaker             ");
	GLCD_DisplayString(6, 0, __FI, "Games               ");
	
	 while (1) {
		 
		 js = get_button();
		 
			 if(js_last != js && js == KBD_UP) {
				 if(menu_count == 2){
					 menu_count = 0;
				 }
				 else {
					 menu_count++;
				 }
			 }
			 else	if(js_last != js && js == KBD_DOWN) {
				 if(menu_count == 0){
					 menu_count = 2;
				 }
				 else {
					 menu_count--;
				 }
			 }
			 else if(js_last != js && js == KBD_SELECT) {
				 if(menu_count == 2){
					 gallery();
				 }
				 else if(menu_count == 1) {
					 speaker_menu();
				 }
				 else if(menu_count == 0) {
					 game_menu();
				 }
			 }
			 
			 LCD(menu_count);
			 js_last = js;
			 
		 
	 }
	
	
}

int LCD(menu_count) {
	
			 if(menu_count == 2) {
				GLCD_SetBackColor(Yellow);
				GLCD_SetTextColor(Black);
				GLCD_DisplayString(4, 0, __FI, "Gallery             ");
				GLCD_SetBackColor(White);
				GLCD_SetTextColor(Black);
				GLCD_DisplayString(5, 0, __FI, "Speaker             ");
				GLCD_DisplayString(6, 0, __FI, "Games               ");
		 }
		else if(menu_count == 1) {
			 	GLCD_SetBackColor(White);
				GLCD_SetTextColor(Black);
				GLCD_DisplayString(4, 0, __FI, "Gallery             ");
				GLCD_SetBackColor(Yellow);
				GLCD_SetTextColor(Black);
				GLCD_DisplayString(5, 0, __FI, "Speaker             ");
				GLCD_SetBackColor(White);
				GLCD_SetTextColor(Black);
				GLCD_DisplayString(6, 0, __FI, "Games               ");		
	  }
		else if(menu_count == 0) {
			 	GLCD_SetBackColor(White);
				GLCD_SetTextColor(Black);
				GLCD_DisplayString(4, 0, __FI, "Gallery             ");
				GLCD_SetBackColor(White);
				GLCD_SetTextColor(Black);
				GLCD_DisplayString(5, 0, __FI, "Speaker             ");
				GLCD_SetBackColor(Yellow);
				GLCD_SetTextColor(Black);
				GLCD_DisplayString(6, 0, __FI, "Games               ");
	  }
		else {
				GLCD_SetBackColor(White);
				GLCD_SetTextColor(Black);
				GLCD_DisplayString(4, 0, __FI, "Gallery             ");
				GLCD_DisplayString(5, 0, __FI, "Speaker             ");
				GLCD_DisplayString(6, 0, __FI, "Games               ");
		}
		
		return 0;
	
}

int gallery(){
	
	int js_gal = 0;
	int js_last_gal = 0;
	int menu_count_gal = 2;
	int flag_once = 0;
	
	int count = 0;
	
  GLCD_Clear  (White);
	
	//delay so that js_gal != select from entering gallery()
	while(count < 1000) {
		count++;
	}
	
	while (1) {
		
		   js_gal = get_button();
		 
			 if(js_last_gal != js_gal && js_gal == KBD_LEFT) {
				 if(menu_count_gal == 2){
					 menu_count_gal = 0;
					 flag_once = 0;
				 }
				 else {
					 menu_count_gal++;
					 flag_once = 0;
				 }
			 }
			 else	if(js_last_gal != js_gal && js_gal == KBD_RIGHT) {
				 if(menu_count_gal == 0){
					 menu_count_gal = 2;
					 flag_once = 0;
				 }
				 else {
					 menu_count_gal--;
					 flag_once = 0;
				 }
			 }
			 else if(js_last_gal != js_gal && js_gal == KBD_SELECT) {
				 flag_once = 0;
				 main();
			 }
				 
				if(menu_count_gal == 2) {
					
					if(flag_once == 0) {
							//set background green
							GLCD_SetBackColor(Green);
							GLCD_SetTextColor(Black);
							GLCD_DisplayString(0, 0, __FI, "                    ");
							GLCD_DisplayString(1, 0, __FI, "                    ");
							GLCD_DisplayString(2, 0, __FI, "                    ");
							GLCD_DisplayString(3, 0, __FI, "                    ");
							GLCD_DisplayString(4, 0, __FI, "                    ");
							GLCD_DisplayString(5, 0, __FI, "                    ");
							GLCD_DisplayString(6, 0, __FI, "                    ");
							GLCD_DisplayString(7, 0, __FI, "                    ");
							GLCD_DisplayString(8, 0, __FI, "                    ");
							GLCD_DisplayString(9, 0, __FI, " Select -> Main Men ");
						
							flag_once = 1;
					}
							//GLCD_Bimap(x, y, horizontal pixel count, vertical pixel count)
							GLCD_Bitmap (  0,   35, 320,  173, img_1);
				}
				else if(menu_count_gal == 1) {
					
							if(flag_once == 0) {
							//set background red
							GLCD_SetBackColor(Red);
							GLCD_SetTextColor(White);
							GLCD_DisplayString(0, 0, __FI, "                    ");
							GLCD_DisplayString(1, 0, __FI, "                    ");
							GLCD_DisplayString(2, 0, __FI, "                    ");
							GLCD_DisplayString(3, 0, __FI, "                    ");
							GLCD_DisplayString(4, 0, __FI, "                    ");
							GLCD_DisplayString(5, 0, __FI, "                    ");
							GLCD_DisplayString(6, 0, __FI, "                    ");
							GLCD_DisplayString(7, 0, __FI, "                    ");
							GLCD_DisplayString(8, 0, __FI, "                    ");
							GLCD_DisplayString(9, 0, __FI, " Select -> Main Men ");
							flag_once = 1;
					}
							
							GLCD_Bitmap (  0,   35, 320,  173, img_2);
				}
				else if(menu_count_gal == 0) {
					
							if(flag_once == 0) {
							//set background blue
							GLCD_SetBackColor(Blue);
							GLCD_SetTextColor(White);
							GLCD_DisplayString(0, 0, __FI, "                    ");
							GLCD_DisplayString(1, 0, __FI, "                    ");
							GLCD_DisplayString(2, 0, __FI, "                    ");
							GLCD_DisplayString(3, 0, __FI, "                    ");
							GLCD_DisplayString(4, 0, __FI, "                    ");
							GLCD_DisplayString(5, 0, __FI, "                    ");
							GLCD_DisplayString(6, 0, __FI, "                    ");
							GLCD_DisplayString(7, 0, __FI, "                    ");
							GLCD_DisplayString(8, 0, __FI, "                    ");
							GLCD_DisplayString(9, 0, __FI, " Select -> Main Men ");
							
							flag_once = 1;
					}
							GLCD_Bitmap (  0,   35, 320,  171, img_3);
				}
			 
			 js_last_gal = js_gal;
		 
	 }
	
}

int speaker_menu() {
	
		int js_sp = 0;
		int js_last_sp = 0;
		int menu_count_gal = 3;
	
		int count = 0;
	
	  GLCD_Clear(White); 	
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(0, 0, __FI, "                    ");
		GLCD_DisplayString(1, 0, __FI, "        ####        ");
		GLCD_DisplayString(2, 0, __FI, "        ## ##       ");
		GLCD_DisplayString(3, 0, __FI, "        ##  ##      ");
		GLCD_DisplayString(4, 0, __FI, "        ##   ##     ");
		GLCD_DisplayString(5, 0, __FI, "     #####     ##   ");
		GLCD_DisplayString(6, 0, __FI, "    #######         ");
		GLCD_DisplayString(7, 0, __FI, "     #####          ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, " Select -> Main Men ");
	
		// start mp3 speaker through USB access
		audio();
	
		//delay
		while(count < 1000) {
			count++;
		}
		
			while (1) {
		
		  js_sp = get_button();
				
			if(js_sp == KBD_SELECT){
				main();
			}
		 
	 }
	
}

int game_menu() {
	
		int js_gam = 0;
		int js_last_gam = 0;
		int menu_count_gam = 2;
	
		int count = 0;
	
	  GLCD_Clear(White);      
		GLCD_SetBackColor(Blue);
		GLCD_SetTextColor(Yellow);
		GLCD_DisplayString(0, 0, __FI, "Game Menu           ");
		GLCD_SetTextColor(White);
		GLCD_DisplayString(1, 0, __FI, "Rock Paper Scissors ");
		GLCD_DisplayString(2, 0, __FI, "Choose Mode:        ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(4, 0, __FI, "                    ");
		GLCD_DisplayString(5, 0, __FI, "Single-Player       ");
		GLCD_DisplayString(6, 0, __FI, "Multiplayer         ");
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, "Return to Main Menu ");
		

	
		//delay
		while(count < 1000) {
			count++;
		}
		
 while (1) {
		 
		 js_gam = get_button();
		 
			 if(js_last_gam != js_gam && js_gam == KBD_UP) {
				 if(menu_count_gam == 2){
					 menu_count_gam = 0;
				 }
				 else {
					 menu_count_gam++;
				 }
			 }
			 else	if(js_last_gam != js_gam && js_gam == KBD_DOWN) {
				 if(menu_count_gam == 0){
					 menu_count_gam = 2;
				 }
				 else {
					 menu_count_gam--;
				 }
			 }
			 else if(js_last_gam != js_gam && js_gam == KBD_SELECT) {
				 if(menu_count_gam == 2){
					 singleplayer(0, 0, 0);
				 }
				 else if(menu_count_gam == 1) {
					 multiplayer(0, 0, 0);
				 }
				 else if(menu_count_gam == 0) {
					 main();
				 }
			 }
			 
				if(menu_count_gam == 2) {     	
					GLCD_SetBackColor(Yellow);
					GLCD_SetTextColor(Black);
					GLCD_DisplayString(5, 0, __FI, "Single-Player       ");
					GLCD_SetBackColor(White);
					GLCD_SetTextColor(Black);
					GLCD_DisplayString(6, 0, __FI, "Multiplayer         ");
					GLCD_SetBackColor(White);
					GLCD_SetTextColor(Black);
					GLCD_DisplayString(9, 0, __FI, "Return to Main Menu ");
				}
				else if(menu_count_gam == 1) {
					GLCD_SetBackColor(White);
					GLCD_SetTextColor(Black);
					GLCD_DisplayString(5, 0, __FI, "Single-Player       ");
					GLCD_SetBackColor(Yellow);
					GLCD_SetTextColor(Black);
					GLCD_DisplayString(6, 0, __FI, "Multiplayer         ");
					GLCD_SetBackColor(White);
					GLCD_SetTextColor(Black);
					GLCD_DisplayString(9, 0, __FI, "Return to Main Menu ");
				}
				else if(menu_count_gam == 0) {
					GLCD_SetBackColor(White);
					GLCD_SetTextColor(Black);
					GLCD_DisplayString(5, 0, __FI, "Single-Player       ");
					GLCD_SetBackColor(White);
					GLCD_SetTextColor(Black);
					GLCD_DisplayString(6, 0, __FI, "Multiplayer         ");
					GLCD_SetBackColor(Yellow);
					GLCD_SetTextColor(Black);
					GLCD_DisplayString(9, 0, __FI, "Return to Main Menu ");
				}
			 
			 js_last_gam = js_gam;
			 
	 }
	
	
}

int singleplayer(int score_p1, int score_p2, int round_num) {
	
		int js_sp = 0;
		int js_last_sp = 0;
		int count_sp = 2;
	  int choice_p1 = 0;
	  int choice_p2 = 0;
		int count = 0;
	
	  // rock = 3
		// paper = 2
		// scisssors = 1
	
	  GLCD_Clear(White);      
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(0, 0, __FI, " Single-Player Mode ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
	  GLCD_DisplayString(1, 0, __FI, "    Use Joystick:   ");
		GLCD_DisplayString(2, 0, __FI, "                    ");
	  GLCD_DisplayString(3, 0, __FI, "                    ");
		GLCD_DisplayString(4, 0, __FI, "                    ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Blue);
		//GLCD_DisplayString(5, 0, __FI, " Rock <-   -> Paper ");
		//GLCD_DisplayString(6, 0, __FI, "         |          ");
		//GLCD_DisplayString(7, 0, __FI, "         V          ");
		//GLCD_DisplayString(8, 0, __FI, "      Scissors      ");
		//GLCD_DisplayString(9, 0, __FI, "                    ");
		
		GLCD_Bitmap( 35, 70, 250, 170, game_img);
		
	  while (1) {
			
			// keep score on LED for P1 (P1.28-P1.31)
			if(score_p1 == 0) {
				 bit1 = &BitBand(&LPC_GPIO1->FIOPIN, 28);
	       bit2 = &BitBand(&LPC_GPIO1->FIOPIN, 29);
				*bit1 = 0;
				*bit2 = 0;
			}
			else if(score_p1 == 1) {
				 bit1 = &BitBand(&LPC_GPIO1->FIOPIN, 28);
				*bit1 = 1;
			}
			else if(score_p1 == 2) {
				 bit2 = &BitBand(&LPC_GPIO1->FIOPIN, 29);
				*bit2 = 1;
				win_final_singleplayer(score_p1, score_p2, round_num);
			}
			
			// keep score on LED for P2 (P2.2-P2.4)
			if(score_p2 == 0) {
				 bit4 = &BitBand(&LPC_GPIO2->FIOPIN, 2);
	       bit5 = &BitBand(&LPC_GPIO2->FIOPIN, 3);
				*bit4 = 0;
				*bit5 = 0;
			}
			else if(score_p2 == 1) {
				 bit4 = &BitBand(&LPC_GPIO2->FIOPIN, 2);
				*bit4 = 1;
			}
			else if(score_p2 == 2) {
	       bit5 = &BitBand(&LPC_GPIO2->FIOPIN, 3);
				*bit5 = 1;
				 win_final_singleplayer(score_p1, score_p2, round_num);
			}
			
			 // check for yostick input
		   js_sp = get_button();
			 choice_p2 = rand() % 3; // Generate random value from 1-3 for computer
			
			 if(js_sp == KBD_LEFT) { // ROCK = 3
				  choice_p1 = 3;
				 	determrps(choice_p1, choice_p2, score_p1, score_p2, round_num);
			 }

			 else	if(js_sp == KBD_RIGHT) { // PAPER = 2
				  choice_p1 = 2;
				 	determrps(choice_p1, choice_p2, score_p1, score_p2, round_num);
			 }
			 
			 else	if(js_sp == KBD_DOWN) { // SCISSORS = 1
				  choice_p1 = 1;
				 	determrps(choice_p1, choice_p2, score_p1, score_p2, round_num);
			 }
			 
	 }
	
}

int determrps(int choice_p1, int choice_p2, int score_p1, int score_p2, int round_num) { // from P1's perspective
		
	  round_num++;
	
		if(choice_p1 == 3) { // ROCK
				if(choice_p2 == 3) {  //rock vs rock
					tie(score_p1, score_p2, round_num);
				}
				else if(choice_p2 == 2) { // rock vs paper
					score_p2++;
					lose(score_p1, score_p2, round_num);
				}
				else if(choice_p2 == 1) { // rock vs scissors
					score_p1++;
					win(score_p1, score_p2, round_num);
				}
		}
		else if(choice_p1 == 2) { // PAPER
				if(choice_p2 == 3) {  //paper vs rock
					score_p1++;
					win(score_p1, score_p2, round_num);
				}
				else if(choice_p2 == 2) { // paper vs paper
					tie(score_p1, score_p2, round_num);
				}
				else if(choice_p2 == 1) { // paper vs scissors
					score_p2++;
					lose(score_p1, score_p2, round_num);
				}
		}
		else if(choice_p1 == 1) { // SCISSORS
				if(choice_p2 == 3) {  //scissors vs rock
					score_p2++;
					lose(score_p1, score_p2, round_num);
				}
				else if(choice_p2 == 2) { // scissors vs paper
					score_p1++;
					win(score_p1, score_p2, round_num);
				}
				else if(choice_p2 == 1) { // scissors vs scissors
					tie(score_p1, score_p2, round_num);
				}
		}
	
}

int win(int score_p1, int score_p2, int round_num) {
	
		int js = 0;
		int count = 0;
	
		GLCD_Clear(White);      
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0, 0, __FI, " Single-Player Mode ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(1, 0, __FI, "                    ");
	  GLCD_DisplayString(2, 0, __FI, "                    ");
		GLCD_DisplayString(3, 0, __FI, "                    ");
		GLCD_SetBackColor(Green);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(4, 0, __FI, "      YOU  WIN      ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(5, 0, __FI, "                    ");	
	  GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(6, 0, __FI, "   COMPUTER LOSES   ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, " Select -> Continue ");
		
		//delay
		while(count < 1000) {
			count++;
		}
		
			while (1) {
		
		  js = get_button();
				
			if(js == KBD_SELECT){
				singleplayer(score_p1, score_p2, round_num);
			}
		 
	 }
	
}

int lose(int score_p1, int score_p2, int round_num) {
	
	
		int js = 0;
		int count = 0;
	
		GLCD_Clear(White);      
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0, 0, __FI, " Single-Player Mode ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(1, 0, __FI, "                    ");
	  GLCD_DisplayString(2, 0, __FI, "                    ");
		GLCD_DisplayString(3, 0, __FI, "                    ");
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(4, 0, __FI, "      YOU LOSE      ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(5, 0, __FI, "                    ");	
	  GLCD_SetBackColor(Green);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(6, 0, __FI, "   COMPUTER  WINS   ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, " Select -> Continue ");
		
		//delay
		while(count < 1000) {
			count++;
		}
		
			while (1) {
		
		  js = get_button();
				
			if(js == KBD_SELECT){
				singleplayer(score_p1, score_p2, round_num);
			}
		 
	 }
	
	
}

int tie(int score_p1, int score_p2, int round_num){
	
		int js = 0;
		int count = 0;
	
		GLCD_Clear(White);      
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0, 0, __FI, " Single-Player Mode ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(1, 0, __FI, "                    ");
	  GLCD_DisplayString(2, 0, __FI, "                    ");
		GLCD_DisplayString(3, 0, __FI, "                    ");
		GLCD_SetBackColor(Yellow);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(4, 0, __FI, "        TIE         ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(5, 0, __FI, "                    ");	
		GLCD_DisplayString(6, 0, __FI, "                    ");
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, " Select -> Continue ");

		//delay
		while(count < 1000) {
			count++;
		}
		
			while (1) {
		
		  js = get_button();
				
			if(js == KBD_SELECT){
				singleplayer(score_p1, score_p2, round_num);
			}
		 
	 }
	
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MULTIPLAYER IMPLEMENTATION
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int multiplayer(int score_p1, int score_p2, int round_num) {
	
		int js_sp = 0;
		int js_last_sp = 0;
		int count_sp = 2;
	  int choice_p1 = 0;
		int count = 0;
	
	  // rock = 3
		// paper = 2
		// scisssors = 1
	
	  GLCD_Clear(White);      
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0, 0, __FI, "  Multiplayer Mode  ");
		GLCD_SetBackColor(Green);
		GLCD_SetTextColor(Black);
	  GLCD_DisplayString(1, 0, __FI, "  P1 Use Joystick:  ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		//GLCD_DisplayString(2, 0, __FI, "                    ");
	  //GLCD_DisplayString(3, 0, __FI, "                    ");
		//GLCD_DisplayString(4, 0, __FI, "                    ");
		//GLCD_SetBackColor(White);
		//GLCD_SetTextColor(Blue);
		//GLCD_DisplayString(5, 0, __FI, " Rock <-   -> Paper ");
		//GLCD_DisplayString(6, 0, __FI, "         |          ");
		//GLCD_DisplayString(7, 0, __FI, "         V          ");
		//GLCD_DisplayString(8, 0, __FI, "      Scissors      ");
		//GLCD_DisplayString(9, 0, __FI, "                    ");
		
		GLCD_Bitmap( 35, 70, 250, 170, game_img);
		
	  while (1) {
			
			// keep score on LED for P1 (P1.28-P1.31)
			if(score_p1 == 0) {
				 bit1 = &BitBand(&LPC_GPIO1->FIOPIN, 28);
	       bit2 = &BitBand(&LPC_GPIO1->FIOPIN, 29);
				*bit1 = 0;
				*bit2 = 0;
			}
			else if(score_p1 == 1) {
				 bit1 = &BitBand(&LPC_GPIO1->FIOPIN, 28);
				*bit1 = 1;
			}
			else if(score_p1 == 2) {
				 bit2 = &BitBand(&LPC_GPIO1->FIOPIN, 29);
				*bit2 = 1;
				win_final_multiplayer(score_p1, score_p2, round_num);
			}
			
			// keep score on LED for P2 (P2.2-P2.4)
			if(score_p2 == 0) {
				 bit4 = &BitBand(&LPC_GPIO2->FIOPIN, 2);
	       bit5 = &BitBand(&LPC_GPIO2->FIOPIN, 3);
				*bit4 = 0;
				*bit5 = 0;
			}
			else if(score_p2 == 1) {
				 bit4 = &BitBand(&LPC_GPIO2->FIOPIN, 2);
				*bit4 = 1;
			}
			else if(score_p2 == 2) {
	       bit5 = &BitBand(&LPC_GPIO2->FIOPIN, 3);
				*bit5 = 1;
				 win_final_multiplayer(score_p1, score_p2, round_num);
			}
			
			 js_sp = get_button();
			
			 if(js_sp == KBD_LEFT) { // ROCK = 3
				  choice_p1 = 3;
				 	p2_select(choice_p1, score_p1, score_p2, round_num);
			 }

			 else	if(js_sp == KBD_RIGHT) { // PAPER = 2
				 	choice_p1 = 2;
				 	p2_select(choice_p1, score_p1, score_p2, round_num);
			 }
			 
			 else	if(js_sp == KBD_DOWN) { // SCISSORS = 1
				  choice_p1 = 1;	
				 	p2_select(choice_p1, score_p1, score_p2, round_num);
			 }
			 
	 }
	
}

int p2_select(int choice_p1, int score_p1, int score_p2, int round_num){
	
		int js_sp = 0;
		int choice_p2 = 0;
	
		GLCD_Clear(White);      
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0, 0, __FI, "  Multiplayer Mode  ");
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(1, 0, __FI, "  P2 Use Joystick:  ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		//GLCD_DisplayString(2, 0, __FI, "    Turn Joystick   ");
	  //GLCD_DisplayString(3, 0, __FI, "   to Select Move:  ");
		//GLCD_DisplayString(4, 0, __FI, "                    ");
		//GLCD_SetBackColor(White);
		//GLCD_SetTextColor(Blue);
		//GLCD_DisplayString(5, 0, __FI, " Rock <-   -> Paper ");
		//GLCD_DisplayString(6, 0, __FI, "         |          ");
		//GLCD_DisplayString(7, 0, __FI, "         V          ");
		//GLCD_DisplayString(8, 0, __FI, "      Scissors      ");
		//GLCD_DisplayString(9, 0, __FI, "                    ");
	
		GLCD_Bitmap( 35, 70, 250, 170, game_img);
	
		 while(1) {
			
			 js_sp = get_button();
			 
			 if(js_sp == KBD_LEFT) { // ROCK = 3
				  choice_p2 = 3;
				 	multi_determrps(choice_p1, choice_p2, score_p1, score_p2, round_num);
			 }

			 else	if(js_sp == KBD_RIGHT) { // PAPER = 2
				 	choice_p2 = 2;
				 	multi_determrps(choice_p1, choice_p2, score_p1, score_p2, round_num);
			 }
			 
			 else	if(js_sp == KBD_DOWN) { // SCISSORS = 1
				  choice_p2 = 1;
				 	multi_determrps(choice_p1, choice_p2, score_p1, score_p2, round_num);
			 }
			 
		}
			 
	
}

int multi_determrps(int choice_p1, int choice_p2, int score_p1, int score_p2, int round_num) { // from P1's perspective
		
	  round_num++;
	
		if(choice_p1 == 3) { // ROCK
				if(choice_p2 == 3) {  //rock vs rock
					multi_tie(score_p1, score_p2, round_num);
				}
				else if(choice_p2 == 2) { // rock vs paper
					score_p2++;
					multi_lose(score_p1, score_p2, round_num);
				}
				else if(choice_p2 == 1) { // rock vs scissors
					score_p1++;
					multi_win(score_p1, score_p2, round_num);
				}
		}
		else if(choice_p1 == 2) { // PAPER
				if(choice_p2 == 3) {  //paper vs rock
					score_p1++;
					multi_win(score_p1, score_p2, round_num);
				}
				else if(choice_p2 == 2) { // paper vs paper
					multi_tie(score_p1, score_p2, round_num);
				}
				else if(choice_p2 == 1) { // paper vs scissors
					score_p2++;
					multi_lose(score_p1, score_p2, round_num);
				}
		}
		else if(choice_p1 == 1) { // SCISSORS
				if(choice_p2 == 3) {  //scissors vs rock
					score_p2++;
					multi_lose(score_p1, score_p2, round_num);
				}
				else if(choice_p2 == 2) { // scissors vs paper
					score_p1++;
					multi_win(score_p1, score_p2, round_num);
				}
				else if(choice_p2 == 1) { // scissors vs scissors
					multi_tie(score_p1, score_p2, round_num);
				}
		}
	
}

int multi_win(int score_p1, int score_p2, int round_num) {
	
		int js = 0;
		int count = 0;
	
		GLCD_Clear(White);      
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0, 0, __FI, "  Multiplayer Mode  ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(1, 0, __FI, "                    ");
	  GLCD_DisplayString(2, 0, __FI, "                    ");
		GLCD_DisplayString(3, 0, __FI, "                    ");
		GLCD_SetBackColor(Green);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(4, 0, __FI, "      P1 WINS       ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(5, 0, __FI, "                    ");	
	  GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(6, 0, __FI, "      P2 LOSES      ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, " Select -> Continue ");
		
		//delay
		while(count < 1000) {
			count++;
		}
		
			while (1) {
		
		  js = get_button();
				
			if(js == KBD_SELECT){
				multiplayer(score_p1, score_p2, round_num);
			}
		 
	 }
	
}

int multi_lose(int score_p1, int score_p2, int round_num) {
	
	
		int js = 0;
		int count = 0;
	
		GLCD_Clear(White);      
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0, 0, __FI, "  Multiplayer Mode  ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(1, 0, __FI, "                    ");
	  GLCD_DisplayString(2, 0, __FI, "                    ");
		GLCD_DisplayString(3, 0, __FI, "                    ");
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(4, 0, __FI, "      P1 LOSES      ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(5, 0, __FI, "                    ");	
	  GLCD_SetBackColor(Green);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(6, 0, __FI, "      P2 WINS       ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, " Select -> Continue ");
		
		//delay
		while(count < 1000) {
			count++;
		}
		
			while (1) {
		
		  js = get_button();
				
			if(js == KBD_SELECT){
				multiplayer(score_p1, score_p2, round_num);
			}
		 
	 }
	
	
}

int multi_tie(int score_p1, int score_p2, int round_num){
	
		int js = 0;
		int count = 0;
	
		GLCD_Clear(White);      
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0, 0, __FI, " Single-Player Mode ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(1, 0, __FI, "                    ");
	  GLCD_DisplayString(2, 0, __FI, "                    ");
		GLCD_DisplayString(3, 0, __FI, "                    ");
		GLCD_SetBackColor(Yellow);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(4, 0, __FI, "        TIE         ");
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(5, 0, __FI, "                    ");	
		GLCD_DisplayString(6, 0, __FI, "                    ");
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, " Select -> Continue ");

		//delay
		while(count < 1000) {
			count++;
		}
		
			while (1) {
		
		  js = get_button();
				
			if(js == KBD_SELECT){
				multiplayer(score_p1, score_p2, round_num);
			}
		 
	 }
	
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UNIVERSAL WIN DETERMINATION METHOD
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int win_final_multiplayer(int score_p1, int score_p2, int round_num) {
	
		int js = 0;
		int count = 0;
		int anim = 0;
	
		while(anim < 4) {
		//set background Yellow
		
		while(count < 2000) {
			count++;
		}
		count = 0;
			
		GLCD_SetBackColor(Yellow);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(0, 0, __FI, "                    ");
		GLCD_DisplayString(1, 0, __FI, "                    ");
		GLCD_DisplayString(2, 0, __FI, "                    ");
		GLCD_DisplayString(3, 0, __FI, "       WINNER       ");
		GLCD_DisplayString(4, 0, __FI, "       WINNER       ");
		GLCD_DisplayString(5, 0, __FI, "       WINNER       ");
		GLCD_DisplayString(6, 0, __FI, "                    ");
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, "                    ");
	
	
		while(count < 2000) {
			count++;
		}
		count = 0;
		
			//set background Red
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0, 0, __FI, "                    ");
		GLCD_DisplayString(1, 0, __FI, "                    ");
		GLCD_DisplayString(2, 0, __FI, "                    ");
		GLCD_DisplayString(3, 0, __FI, "       WINNER       ");
		GLCD_DisplayString(4, 0, __FI, "       WINNER       ");
		GLCD_DisplayString(5, 0, __FI, "       WINNER       ");
		GLCD_DisplayString(6, 0, __FI, "                    ");
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, "                    ");
	
		anim++;
		
		}
		
		anim = 0;
	
		while(count < 2000) {
			count++;
		}
		count = 0;
		
		if(score_p1 > score_p2) {
			
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(1, 0, __FI, "                    ");
			GLCD_DisplayString(2, 0, __FI, "                    ");
			GLCD_DisplayString(3, 0, __FI, "                    ");
			GLCD_SetBackColor(Green);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(4, 0, __FI, " P1 IS THE WINNER!! ");
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(5, 0, __FI, "                    ");	
			GLCD_DisplayString(6, 0, __FI, "                    ");
			GLCD_DisplayString(7, 0, __FI, "                    ");
			GLCD_DisplayString(8, 0, __FI, "                    ");
			GLCD_DisplayString(9, 0, __FI, " Select -> Game Men ");
			
		}
		
		else {
			

			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(1, 0, __FI, "                    ");
			GLCD_DisplayString(2, 0, __FI, "                    ");
			GLCD_DisplayString(3, 0, __FI, "                    ");
			GLCD_SetBackColor(Red);
			GLCD_SetTextColor(White);
			GLCD_DisplayString(4, 0, __FI, " P2 IS THE WINNER!! ");
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(5, 0, __FI, "                    ");	
			GLCD_DisplayString(6, 0, __FI, "                    ");
			GLCD_DisplayString(7, 0, __FI, "                    ");
			GLCD_DisplayString(8, 0, __FI, "                    ");
			GLCD_DisplayString(9, 0, __FI, " Select -> Game Men ");
			
		}
	
		
		//delay
		while(count < 1000) {
			count++;
		}
		
			while (1) {
		
		  js = get_button();
				
			if(js == KBD_SELECT){
					// clear LEDs
				
	     *bit1 = 0;
	     *bit2 = 0;
	     *bit4 = 0;
	     *bit5 = 0;
				game_menu();
			}
		 
	 }
 }

 int win_final_singleplayer(int score_p1, int score_p2, int round_num) {
	
		int js = 0;
		int count = 0;
	 	int anim = 0;
		
		if(score_p1 > score_p2) {
			
	
		while(anim < 4) {
		//set background Yellow
		
		while(count < 2000) {
			count++;
		}
		count = 0;
			
		GLCD_SetBackColor(Yellow);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(0, 0, __FI, "                    ");
		GLCD_DisplayString(1, 0, __FI, "                    ");
		GLCD_DisplayString(2, 0, __FI, "                    ");
		GLCD_DisplayString(3, 0, __FI, "       WINNER       ");
		GLCD_DisplayString(4, 0, __FI, "       WINNER       ");
		GLCD_DisplayString(5, 0, __FI, "       WINNER       ");
		GLCD_DisplayString(6, 0, __FI, "                    ");
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, "                    ");
	
	
		while(count < 2000) {
			count++;
		}
		count = 0;
		
			//set background Red
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0, 0, __FI, "                    ");
		GLCD_DisplayString(1, 0, __FI, "                    ");
		GLCD_DisplayString(2, 0, __FI, "                    ");
		GLCD_DisplayString(3, 0, __FI, "       WINNER       ");
		GLCD_DisplayString(4, 0, __FI, "       WINNER       ");
		GLCD_DisplayString(5, 0, __FI, "       WINNER       ");
		GLCD_DisplayString(6, 0, __FI, "                    ");
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, "                    ");
	
		anim++;
		
		}
		anim = 0;
	
		while(count < 2000) {
			count++;
		}
		count = 0;
			
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(1, 0, __FI, "                    ");
			GLCD_DisplayString(2, 0, __FI, "                    ");
			GLCD_DisplayString(3, 0, __FI, "                    ");
			GLCD_SetBackColor(Green);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(4, 0, __FI, "      YOU WIN!      ");
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(5, 0, __FI, "                    ");	
			GLCD_DisplayString(6, 0, __FI, "                    ");
			GLCD_DisplayString(7, 0, __FI, "                    ");
			GLCD_DisplayString(8, 0, __FI, "                    ");
			GLCD_DisplayString(9, 0, __FI, " Select -> Game Men ");
			
		}
		
		else {
			
		while(anim < 4) {
		//set background Yellow
		
		while(count < 2000) {
			count++;
		}
		count = 0;
			
		GLCD_SetBackColor(Yellow);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(0, 0, __FI, "                    ");
		GLCD_DisplayString(1, 0, __FI, "                    ");
		GLCD_DisplayString(2, 0, __FI, "                    ");
		GLCD_DisplayString(3, 0, __FI, "       LOSER        ");
		GLCD_DisplayString(4, 0, __FI, "       LOSER        ");
		GLCD_DisplayString(5, 0, __FI, "       LOSER        ");
		GLCD_DisplayString(6, 0, __FI, "                    ");
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, "                    ");
	
	
		while(count < 2000) {
			count++;
		}
		count = 0;
		
			//set background Red
		GLCD_SetBackColor(Red);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0, 0, __FI, "                    ");
		GLCD_DisplayString(1, 0, __FI, "                    ");
		GLCD_DisplayString(2, 0, __FI, "                    ");
		GLCD_DisplayString(3, 0, __FI, "       LOSER        ");
		GLCD_DisplayString(4, 0, __FI, "       LOSER        ");
		GLCD_DisplayString(5, 0, __FI, "       LOSER        ");
		GLCD_DisplayString(6, 0, __FI, "                    ");
		GLCD_DisplayString(7, 0, __FI, "                    ");
		GLCD_DisplayString(8, 0, __FI, "                    ");
		GLCD_DisplayString(9, 0, __FI, "                    ");
	
		anim++;
		
		}		
	
		anim = 0;
		
		while(count < 2000) {
			count++;
		}
		count = 0;

			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(1, 0, __FI, "                    ");
			GLCD_DisplayString(2, 0, __FI, "                    ");
			GLCD_DisplayString(3, 0, __FI, "                    ");
			GLCD_SetBackColor(Red);
			GLCD_SetTextColor(White);
			GLCD_DisplayString(4, 0, __FI, "   COMPUTER WINS!   ");
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString(5, 0, __FI, "                    ");	
			GLCD_DisplayString(6, 0, __FI, "                    ");
			GLCD_DisplayString(7, 0, __FI, "                    ");
			GLCD_DisplayString(8, 0, __FI, "                    ");
			GLCD_DisplayString(9, 0, __FI, " Select -> Game Men ");
			
		}
	
		
		//delay
		while(count < 1000) {
			count++;
		}
		
			while (1) {
		
		  js = get_button();
				
			if(js == KBD_SELECT){
					// clear LEDs
				
	     *bit1 = 0;
	     *bit2 = 0;
	     *bit4 = 0;
	     *bit5 = 0;
				game_menu();
			}
		 
	 }
 }
 