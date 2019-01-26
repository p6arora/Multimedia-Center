#include "LPC17xx.h"                        /* LPC17xx definitions */
#include "type.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "usbaudio.h"
#include "GLCD.h"
#include "KBD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lanes10x20.c"
#include "RaceCar30x60.c"
#include "taxi30x60.c"



#define __FI        1                      /* Font index 16x24        */
#define __USE_LCD   0				      /* Uncomment to use the LCD */
#define TIMESTRING static volatile unsigned char code TimeString [] = __TIME__


int lane_y,score, timer; 			//game variables
int val, position, prev_position, prev_val; 						//joystick variables
char score_txt[20], text[20];
int car_x, car_y, speed; 						//car coordinates
int taxi_x, taxi_y; 					// taxi coordinates
int i; 												// miscelaneous variables


// prototype of all functions used
void delay(int i);
void mainMenu();
void mp3();
void PhotoGallery();
void game();
void makeLane();
void makeTaxis();

int position, img_num, joystick;



void delay(int delay){
	while (--delay);
}

void mp3(){
	GLCD_Clear(White);
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(0, 1, __FI, "USB Audio");
	GLCD_DisplayString(1, 1, __FI, "SPEAKERS: ON ");
	GLCD_DisplayString(2, 1, __FI, "Press left for Main Manu ");

	joystick = 0;
	joystick = get_button();
	
	audio();
	while(get_button()!=KBD_DOWN);
	mainMenu();
	
	
	
}

void PhotoGallery(){
	#include "GULKHAN1.c"
	#include "GulKhan2.c"
	#include "GuKhan3.c"
	
	GLCD_Clear(White);
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(0, 1, __FI, "<< Main Menu");
	GLCD_DisplayString(8, 1, __FI, "vv Photos ^^ ");
	delay(10000);
	img_num = 0;
	while(1){

	
		int val = 0;
		val = get_button();

		switch(val){
			case(KBD_DOWN):
				GLCD_Clear(White);
				img_num++;
			GLCD_DisplayString(1, 1, __FI, (unsigned char*)img_num);
				if(img_num == 3){img_num = 0;}
				if(img_num == 0){GLCD_Bitmap(0,0,300,200,GULKHAN1_PIXEL_DATA);}
				if(img_num == 1){GLCD_Bitmap(0,0,320,240,GULKHAN2_PIXEL_DATA);}
				if(img_num == 2){GLCD_Bitmap(0,0,320,240,GUKHAN3_PIXEL_DATA);}
				break;
			case(KBD_UP):
				GLCD_Clear(White);
				img_num--;
				if(img_num == -1){img_num = 2;}
				if(img_num == 0){GLCD_Bitmap(0,0,300,200,GULKHAN1_PIXEL_DATA);}
				if(img_num == 1){GLCD_Bitmap(0,0,320,240,GULKHAN2_PIXEL_DATA);}
				if(img_num == 2){GLCD_Bitmap(0,0,320,240,GUKHAN3_PIXEL_DATA);}
				break;
			case(KBD_LEFT):
				mainMenu();
				break;
			default:
				break;
		}

	
	}
	
	

}

void game(){
	GLCD_Clear(Black);
	GLCD_SetBackColor(Black);
	lane_y = 0;
	position = 1;
	prev_position = -1;
	prev_val = 0;
	score = 0;
	timer = 0;
	taxi_y = 180;
	val = 0;
	car_y = 160;
	car_x = 150;
	speed = 1;
	srand((unsigned int)__TIME__); 	//seeds the random number with current time
	while (1){
	
		val = get_button();
	
	makeLane();
	if (timer == 0){GLCD_Bitmap(car_x,car_y,30,60,RACECAR30X60_PIXEL_DATA);} // starting position of car
	if(val != 0 || val != prev_val){	
		switch(prev_position){
			case(0): GLCD_Bitmap_color(44,car_y,30,60,0x0000); 	
			break;
			case(1): GLCD_Bitmap_color(150,car_y,30,60,0x0000); 	
			break;
			case(2): GLCD_Bitmap_color(240,car_y,30,60,0x0000); 	
			break;
			default: break;
	}
			
		
		
		
		switch(val){
			case(KBD_RIGHT):
				position++;
			if(position == 3){position = 2;}
				break;
			case(KBD_LEFT):
				position--;
			if(position == -1){position = 0;}
				break;
			default:
				break;
		}

			switch(position){
			case(0):
				//car - left position
				car_x = 44;
				GLCD_Bitmap(car_x,car_y,30,60,RACECAR30X60_PIXEL_DATA);
				delay(100000);
				break;
			case(1):
				//car - middle position
				car_x = 150;
				GLCD_Bitmap(car_x,car_y,30,60,RACECAR30X60_PIXEL_DATA);
				delay(100000);
				break;
			case(2):
				//car - right position
				car_x = 240;
				GLCD_Bitmap(car_x,car_y,30,60,RACECAR30X60_PIXEL_DATA);
				delay(100000);
				break;
			default:
				break;
		}
	}	
		timer++;
		if(timer % 50 == 0){score++;}
		if(timer % 70 == 0){speed++;}
		sprintf(score_txt, "Score: %d", score);
		sprintf(text, "Speed: %d", speed);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0,0, __FI,(unsigned char *) score_txt);
		GLCD_DisplayString(1,0, __FI,(unsigned char *) text);
			
		if(timer > 200){makeTaxis();}
		
		//collision detection
		if(taxi_x == car_x && (car_y - taxi_y <= 60))
			for (i = 0; i < 10; i++){
			GLCD_Clear(Red);
			GLCD_SetTextColor(Green);
			GLCD_DisplayString(3, 3, __FI, "You Lose!");
			if(i == 9){mainMenu();}
		}

		
		prev_position = position;
		prev_val = val;
	}
	

}

void makeLane(){
	
	//clear both lanes
	GLCD_Bitmap_color(106,lane_y,10,20,0x0000); 						//lane 1
	GLCD_Bitmap_color(106,lane_y + 80,10,20,0x0000);
	GLCD_Bitmap_color(106,lane_y + 160,10,20,0x0000);
	GLCD_Bitmap_color(212,lane_y,10,20,0x0000); 						//lane 2
	GLCD_Bitmap_color(212,lane_y + 80,10,20,0x0000);
	GLCD_Bitmap_color(212,lane_y + 160,10,20,0x0000);
	
	lane_y = lane_y + speed;
	//create lanes
	GLCD_Bitmap(106,lane_y,10,20,LANES10X20_PIXEL_DATA);		//lane 1
	GLCD_Bitmap(106,lane_y + 80,10,20,LANES10X20_PIXEL_DATA);
	GLCD_Bitmap(106,lane_y + 160,10,20,LANES10X20_PIXEL_DATA);
	GLCD_Bitmap(212,lane_y,10,20,LANES10X20_PIXEL_DATA);		//lane 2
	GLCD_Bitmap(212,lane_y + 80,10,20,LANES10X20_PIXEL_DATA);
	GLCD_Bitmap(212,lane_y + 160,10,20,LANES10X20_PIXEL_DATA);
	
	//resetting the lane
	if(lane_y == 80){lane_y = 0;}
	
}

void makeTaxis(){
if ( taxi_y > 180){
	GLCD_Bitmap_color(taxi_x,taxi_y,30,60,0x0000);
	taxi_y = 0;
	taxi_x = rand() % 3;
	
	switch(taxi_x){
		case(0): taxi_x = 44;
			break;
		case(1): taxi_x = 150;
			break;
		case(2): taxi_x = 240;
			break;
		default: break;
	}
}
else
	taxi_y = taxi_y + speed;
	GLCD_Bitmap(taxi_x,taxi_y,30,60,TAXI30X60_PIXEL_DATA);

}

void mainMenu(){

		GLCD_Clear(White);
		GLCD_SetBackColor(Blue);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0,0, __FI, "COE718 Project Demo ");
		GLCD_DisplayString(1,0, __FI, "    Paarth Arora    ");		
		GLCD_DisplayString(2,0, __FI, "    Main Menu       ");				
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(5, 0, __FI, ">> Photo Gallery    ");
		GLCD_DisplayString(6, 0, __FI, "USB Audio Player    ");
		GLCD_DisplayString(7, 0, __FI, "Game                ");

position = 0;
	//if(prevVal != currentVal){
	for(;;){
	
		int val = 0;
		val = get_button();

		switch(val){
			case(KBD_DOWN):
				position++;
			if(position == 3){position = 0;}
				break;
			case(KBD_UP):
				position--;
			if(position == -1){position = 2;}
				break;
			default:
				break;
		}
		if (position == 0){
		//GLCD_Clear(White);
		GLCD_SetBackColor(Blue);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0,0, __FI, "COE718 Project Demo ");
		GLCD_DisplayString(1,0, __FI, "    Paarth Arora    ");		
		GLCD_DisplayString(2,0, __FI, "    Main Menu       ");				
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);

		GLCD_DisplayString(5, 0, __FI, ">> Photo Gallery    ");
		GLCD_DisplayString(6, 0, __FI, "USB Audio Player    ");
		GLCD_DisplayString(7, 0, __FI, "Game                ");
		}
		
		if(position == 1){
		//GLCD_Clear(White);
		GLCD_SetBackColor(Blue);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0,0, __FI, "COE718 Project Demo ");
		GLCD_DisplayString(1,0, __FI, "    Paarth Arora    ");		
		GLCD_DisplayString(2,0, __FI, "    Main Menu       ");			
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(5, 0, __FI, "Photo Gallery       ");
		GLCD_DisplayString(6, 0, __FI, ">> USB Audio Player ");
		GLCD_DisplayString(7, 0, __FI, "Game                ");
		}
		
		if(position == 2){
		//GLCD_Clear(White);
		GLCD_SetBackColor(Blue);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(0,0, __FI, "COE718 Project Demo ");
		GLCD_DisplayString(1,0, __FI, "    Paarth Arora    ");		
		GLCD_DisplayString(2,0, __FI, "    Main Menu       ");			
		GLCD_SetBackColor(White);
		GLCD_SetTextColor(Black);
		GLCD_DisplayString(5, 0, __FI, "Photo Gallery    		 ");
		GLCD_DisplayString(6, 0, __FI, "USB Audio Player     ");
		GLCD_DisplayString(7, 0, __FI, ">> Game              ");
		}
		

		
		if(val == KBD_SELECT){
			//Photo Gallery    - Position 0
			//USB Audio Player - Position 1 
			//Game             - Position 2
			switch(position){
				case(0):
					PhotoGallery();
					break;
				case(1):
					mp3();
					break;
				case(2):
					game();
					break;
				default:
					break;
			}
		}
		//prevVal = val;
	}
	
	
	
	
	
}


int main(){
		GLCD_Init();
		KBD_Init();
		mainMenu();

	return 0;
}
