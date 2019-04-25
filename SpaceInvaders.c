// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 11/20/2018 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2018

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2018

 Copyright 2018 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "PLL.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer0.h"
#include "Timer1.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds


int main1(void){
  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
  Random_Init(1);

  Output_Init();
  ST7735_FillScreen(0x0000);            // set screen to black
  
  ST7735_DrawBitmap(52, 159, ns, 18,8); // player ship middle bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);

  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);
  ST7735_DrawBitmap(100, 9, SmallEnemy30pointB, 16,10);

  Delay100ms(50);              // delay 5 sec at 80 MHz

  ST7735_FillScreen(0x0000);            // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  LCD_OutDec(1234);
  while(1){
  }

}
static short texture[160];
void drawCastLine(int16_t x, int16_t y, int16_t h, uint16_t colorF, uint16_t colorB) 
{
	
	
	int32_t yBelow = y;
	int32_t yAbove = 160 - (y + h);
	
	if(yBelow > 0)
	{
		ST7735_DrawFastVLine(x, 0, yBelow, colorB);
		
	}
	
	if(yAbove > 0)
	{
		ST7735_DrawFastVLine(x, y+h, yAbove, colorB);
		
	}
	
	if(h > 0)
	{
		ST7735_DrawFastVLine(x, y, h, colorF);
		
	}
	h*=0;
	return;
	
}

void drawCastLineRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t colorF, uint16_t colorB) 
{
	
	
	int32_t yBelow = y;
	int32_t yAbove = 160 - (y + h);
	
	if(yBelow > 0)
	{
		ST7735_FillRect(x, 0, w, yBelow, colorB);
		
	}
	
	if(yAbove > 0)
	{
		ST7735_FillRect(x, y+h, w, yAbove, colorB);
		
	}
	
	if(h > 0)
	{
		ST7735_FillRect(x, y, w, h, colorF);
		
	}
	h*=1;
	return;
	
}

void drawCastTexture(int16_t x, int16_t y, int16_t h, uint16_t colorC, uint16_t colorF)
{
	
	int32_t yBelow = y;
	int32_t yAbove = 160 - (y + h);
	
	if(yBelow > 0)
	{
		ST7735_DrawFastVLine(x, 0, yBelow, colorC);
		
	}
	
	if(yAbove > 0)
	{
		ST7735_DrawFastVLine(x, y+h, yAbove, colorF);
		
	}
	
	if(h > 0)
	{
		ST7735_DrawBitmap(x, y + h, texture, 1, h);
		
		
	}
	
	
}
//static short tempArr[128];
//void scaleTexture(int32_t height, int32_t row, int32_t y_slice)
//{
//	
//	
//	
//	int32_t d;
//	int32_t texY;
//	for(uint32_t i = 0; i < height; i++)
//	{
//		
//		 d = i * 256 - 64 * 128 + height * 128;
//		 texY = ((d * 64) / height) / 256;
//		
//		 texture[i] = dk_wall[64 * texY + row];
//		
//		
//	}
//	
//	
//	
//	
//}


void scaleTexture(int32_t height, int32_t row)
{
  uint32_t i = 0;
	uint32_t j = 0;
	uint32_t inc = (64 << 10) / height;
	uint32_t texY;
	while(j < height)
	{
		texY = i >> 10;
		
		texture[j] = dk_wall[ (texY * 64)+ row];
		
		i+= inc;
		j++;
		
	}
	
	
	
}


//void scaleTexture(int32_t height)
//{
//	uint32_t tCounter = 0;
//  uint32_t j = 0;
//	for(j = 0; j < height; j++)
//		{
//			tempArr[j] = texture[j];
//			
//		}
//	
//	j = 0;
//	if (height == 64)
//	{
//		return;
//	}
//	
//	
//	else if(height > 64)
//	{
//		
//		//short temp[5];
//		uint32_t j = 0;
//		uint32_t div = height / 64;
//		uint32_t mod = height % 64;
//		
//		int modCount = 0;
//		for(uint32_t i = 0; i < 64; i++)
//		{
//			
//			for(j = 0; j < div; j++)
//			{
//				texture[tCounter] = tempArr[i];
//				tCounter++;
//				
//			}
//			
//			if(mod > 0)
//			{
//			texture[tCounter] = tempArr[i];
//			tCounter++;
//			mod-=1;	
//			}
//			
//			
//			
//		}
//		
//		
//		
//		return;
//			
//	}
//	 
//	else
//	{
//	
//		uint32_t div = 64 / height;
//		uint32_t mod = 64 % height;
//		uint32_t j = 0;
//		//short temp[height];
//		//short temp[5];
//		
//		
//		uint32_t arrCount = 0;
//		
//		
//		for(uint32_t i = 0; i < height; i++)
//		{
//			
//			uint32_t sum;
//			for(j=0; j<div; j++)
//			{
//				sum+=tempArr[arrCount + j];
//				arrCount++;
//			}
//			
//			if(mod > 0)
//			{
//				
//			  sum+=tempArr[j + arrCount + 1];
//				arrCount++;
//				texture[i] = sum / (div + 1);
//			}
//			else
//			{
//				texture[i] = sum / div;
//				
//			}
//			
//		
//			
//		}
//		
//		
//		return;
//		
//	}
//	
//	return;
//	
//}
	
	
	


	
void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 0;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x0007;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;
	NVIC_ST_RELOAD_R = 0xFFFFFF;
}


void SysTick_Handler(void)
{
	
	NVIC_ST_RELOAD_R = 0xFFFFFF;
}




// *****ROM TABLES ************************

const int32_t xMoveTable[] = {15, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 13, 13, 13, 13, 13, 13, 13, 13, 12, 12, 12, 12, 12, 12, 12, 11, 11, 11, 11, 11, 11, 10, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -2, -2, -2, -2, -3, -3, -3, -3, -4, -4, -4, -4, -5, -5, -5, -5, -6, -6, -6, -6, -7, -7, -7, -7, -7, -8, -8, -8, -8, -9, -9, -9, -9, -9, -10, -10, -10, -10, -10, -10, -11, -11, -11, -11, -11, -11, -12, -12, -12, -12, -12, -12, -12, -13, -13, -13, -13, -13, -13, -13, -13, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -15, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -13, -13, -13, -13, -13, -13, -13, -13, -12, -12, -12, -12, -12, -12, -12, -11, -11, -11, -11, -11, -11, -10, -10, -10, -10, -10, -10, -9, -9, -9, -9, -9, -8, -8, -8, -8, -7, -7, -7, -7, -7, -6, -6, -6, -6, -5, -5, -5, -5, -4, -4, -4, -4, -3, -3, -3, -3, -2, -2, -2, -2, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15};
const int32_t yMoveTable[] = {0, 0, 0, 0, -1, -1, -1, -1, -2, -2, -2, -2, -3, -3, -3, -3, -4, -4, -4, -4, -5, -5, -5, -5, -6, -6, -6, -6, -7, -7, -7, -7, -7, -8, -8, -8, -8, -9, -9, -9, -9, -9, -10, -10, -10, -10, -10, -10, -11, -11, -11, -11, -11, -11, -12, -12, -12, -12, -12, -12, -12, -13, -13, -13, -13, -13, -13, -13, -13, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -15, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, -13, -13, -13, -13, -13, -13, -13, -13, -12, -12, -12, -12, -12, -12, -12, -11, -11, -11, -11, -11, -11, -10, -10, -10, -10, -10, -10, -9, -9, -9, -9, -9, -8, -8, -8, -8, -7, -7, -7, -7, -7, -6, -6, -6, -6, -5, -5, -5, -5, -4, -4, -4, -4, -3, -3, -3, -3, -2, -2, -2, -2, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 13, 13, 13, 13, 13, 13, 13, 13, 12, 12, 12, 12, 12, 12, 12, 11, 11, 11, 11, 11, 11, 10, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0};
const int32_t cosTableTim8[] = {-1, 255, 255, 255, 255, 255, 254, 254, 253, 252, 252, 251, 250, 249, 248, 247, 246, 244, 243, 242, 240, 238, 237, 235, 233, 232, 230, 228, 226, 223, 221, 219, 217, 214, 212, 209, 207, 204, 201, 198, 196, 193, 190, 187, 184, 181, 177, 174, 171, 167, 164, 161, 157, 154, 150, 146, 143, 139, 135, 131, 128, 124, 120, 116, 112, 108, 104, 100, 95, 91, 87, 83, 79, 74, 70, 66, 61, 57, 53, 48, 44, 40, 35, 31, 26, 22, 17, 13, 8, 4, -1, -4, -8, -13, -17, -22, -26, -31, -35, -40, -44, -48, -53, -57, -61, -66, -70, -74, -79, -83, -87, -91, -95, -100, -104, -108, -112, -116, -120, -124, -127, -131, -135, -139, -143, -146, -150, -154, -157, -161, -164, -167, -171, -174, -177, -181, -184, -187, -190, -193, -196, -198, -201, -204, -207, -209, -212, -214, -217, -219, -221, -223, -226, -228, -230, -232, -233, -235, -237, -238, -240, -242, -243, -244, -246, -247, -248, -249, -250, -251, -252, -252, -253, -254, -254, -255, -255, -255, -255, -255, -1, -255, -255, -255, -255, -255, -254, -254, -253, -252, -252, -251, -250, -249, -248, -247, -246, -244, -243, -242, -240, -238, -237, -235, -233, -232, -230, -228, -226, -223, -221, -219, -217, -214, -212, -209, -207, -204, -201, -198, -196, -193, -190, -187, -184, -181, -177, -174, -171, -167, -164, -161, -157, -154, -150, -146, -143, -139, -135, -131, -128, -124, -120, -116, -112, -108, -104, -100, -95, -91, -87, -83, -79, -74, -70, -66, -61, -57, -53, -48, -44, -40, -35, -31, -26, -22, -17, -13, -8, -4, -1, 4, 8, 13, 17, 22, 26, 31, 35, 40, 44, 48, 53, 57, 61, 66, 70, 74, 79, 83, 87, 91, 95, 100, 104, 108, 112, 116, 120, 124, 128, 131, 135, 139, 143, 146, 150, 154, 157, 161, 164, 167, 171, 174, 177, 181, 184, 187, 190, 193, 196, 198, 201, 204, 207, 209, 212, 214, 217, 219, 221, 223, 226, 228, 230, 232, 233, 235, 237, 238, 240, 242, 243, 244, 246, 247, 248, 249, 250, 251, 252, 252, 253, 254, 254, 255, 255, 255, 255, 255, 256,};

const int32_t tanRayTableTim12[] = {-1, 71, 143, 214, 286, 358, 430, 502, 575, 648, 722, 796, 870, 945, 1021, 1097, 1174, 1252, 1330, 1410, 1490, 1572, 1654, 1738, 1823, 1909, 1997, 2087, 2177, 2270, 2364, 2461, 2559, 2659, 2762, 2868, 2975, 3086, 3200, 3316, 3436, 3560, 3688, 3819, 3955, 4095, 4241, 4392, 4549, 4711, 4881, 5058, 5242, 5435, 5637, 5849, 6072, 6307, 6554, 6816, 7094, 7389, 7703, 8038, 8398, 8783, 9199, 9649, 10137, 10670, 11253, 11895, 12606, 13397, 14284, 15286, 16428, 17741, 19270, 21072, 23229, 25861, 29144, 33359, 38970, 46817, 58575, 78156, 117294, 234659, -1, -234659, -117294, -78156, -58575, -46817, -38970, -33359, -29144, -25861, -23229, -21072, -19270, -17741, -16428, -15286, -14284, -13397, -12606, -11895, -11253, -10670, -10137, -9649, -9199, -8783, -8398, -8038, -7703, -7389, -7094, -6816, -6554, -6307, -6072, -5849, -5637, -5435, -5242, -5058, -4881, -4711, -4549, -4392, -4241, -4096, -3955, -3819, -3688, -3560, -3436, -3316, -3200, -3086, -2975, -2868, -2762, -2659, -2559, -2461, -2364, -2270, -2177, -2087, -1997, -1909, -1823, -1738, -1654, -1572, -1490, -1410, -1330, -1252, -1174, -1097, -1021, -945, -870, -796, -722, -648, -575, -502, -430, -358, -286, -214, -143, -71, -1, 71, 143, 214, 286, 358, 430, 502, 575, 648, 722, 796, 870, 945, 1021, 1097, 1174, 1252, 1330, 1410, 1490, 1572, 1654, 1738, 1823, 1909, 1997, 2087, 2177, 2270, 2364, 2461, 2559, 2659, 2762, 2868, 2975, 3086, 3200, 3316, 3436, 3560, 3688, 3819, 3955, 4095, 4241, 4392, 4549, 4711, 4881, 5058, 5242, 5435, 5637, 5849, 6072, 6307, 6554, 6816, 7094, 7389, 7703, 8038, 8398, 8783, 9199, 9649, 10137, 10670, 11253, 11895, 12606, 13397, 14284, 15286, 16428, 17741, 19270, 21072, 23229, 25861, 29144, 33359, 38970, 46817, 58575, 78156, 117294, 234659, -1, -234659, -117294, -78156, -58575, -46817, -38970, -33359, -29144, -25861, -23229, -21072, -19270, -17741, -16428, -15286, -14284, -13397, -12606, -11895, -11253, -10670, -10137, -9649, -9199, -8783, -8398, -8038, -7703, -7389, -7094, -6816, -6554, -6307, -6072, -5849, -5637, -5435, -5242, -5058, -4881, -4711, -4549, -4392, -4241, -4096, -3955, -3819, -3688, -3560, -3436, -3316, -3200, -3086, -2975, -2868, -2762, -2659, -2559, -2461, -2364, -2270, -2177, -2087, -1997, -1909, -1823, -1738, -1654, -1572, -1490, -1410, -1330, -1252, -1174, -1097, -1021, -945, -870, -796, -722, -648, -575, -502, -430, -358, -286, -214, -143, -71, 0,};
const int32_t tanRayTableDiv12[] = {-1, 234659, 117294, 78156, 58575, 46817, 38970, 33359, 29144, 25861, 23229, 21072, 19270, 17741, 16428, 15286, 14284, 13397, 12606, 11895, 11253, 10670, 10137, 9649, 9199, 8783, 8398, 8038, 7703, 7389, 7094, 6816, 6554, 6307, 6072, 5849, 5637, 5435, 5242, 5058, 4881, 4711, 4549, 4392, 4241, 4096, 3955, 3819, 3688, 3560, 3436, 3316, 3200, 3086, 2975, 2868, 2762, 2659, 2559, 2461, 2364, 2270, 2177, 2087, 1997, 1909, 1823, 1738, 1654, 1572, 1490, 1410, 1330, 1252, 1174, 1097, 1021, 945, 870, 796, 722, 648, 575, 502, 430, 358, 286, 214, 143, 71, -1, -71, -143, -214, -286, -358, -430, -502, -575, -648, -722, -796, -870, -945, -1021, -1097, -1174, -1252, -1330, -1410, -1490, -1572, -1654, -1738, -1823, -1909, -1997, -2087, -2177, -2270, -2364, -2461, -2559, -2659, -2762, -2868, -2975, -3086, -3200, -3316, -3436, -3560, -3688, -3819, -3955, -4095, -4241, -4392, -4549, -4711, -4881, -5058, -5242, -5435, -5637, -5849, -6072, -6307, -6554, -6816, -7094, -7389, -7703, -8038, -8398, -8783, -9199, -9649, -10137, -10670, -11253, -11895, -12606, -13397, -14284, -15286, -16428, -17741, -19270, -21072, -23229, -25861, -29144, -33359, -38970, -46817, -58575, -78156, -117294, -234659, -1, 234659, 117294, 78156, 58575, 46817, 38970, 33359, 29144, 25861, 23229, 21072, 19270, 17741, 16428, 15286, 14284, 13397, 12606, 11895, 11253, 10670, 10137, 9649, 9199, 8783, 8398, 8038, 7703, 7389, 7094, 6816, 6554, 6307, 6072, 5849, 5637, 5435, 5242, 5058, 4881, 4711, 4549, 4392, 4241, 4096, 3955, 3819, 3688, 3560, 3436, 3316, 3200, 3086, 2975, 2868, 2762, 2659, 2559, 2461, 2364, 2270, 2177, 2087, 1997, 1909, 1823, 1738, 1654, 1572, 1490, 1410, 1330, 1252, 1174, 1097, 1021, 945, 870, 796, 722, 648, 575, 502, 430, 358, 286, 214, 143, 71, -1, -71, -143, -214, -286, -358, -430, -502, -575, -648, -722, -796, -870, -945, -1021, -1097, -1174, -1252, -1330, -1410, -1490, -1572, -1654, -1738, -1823, -1909, -1997, -2087, -2177, -2270, -2364, -2461, -2559, -2659, -2762, -2868, -2975, -3086, -3200, -3316, -3436, -3560, -3688, -3819, -3955, -4095, -4241, -4392, -4549, -4711, -4881, -5058, -5242, -5435, -5637, -5849, -6072, -6307, -6554, -6816, -7094, -7389, -7703, -8038, -8398, -8783, -9199, -9649, -10137, -10670, -11253, -11895, -12606, -13397, -14284, -15286, -16428, -17741, -19270, -21072, -23229, -25861, -29144, -33359, -38970, -46817, -58575, -78156, -117294, -234659, -16723189,};

const int32_t cosDistTableDiv8[] = {-1, -256, -256, -256, -256, -256, -257, -257, -258, -259, -259, -260, -261, -262, -263, -265, -266, -267, -269, -270, -272, -274, -276, -278, -280, -282, -284, -287, -289, -292, -295, -298, -301, -305, -308, -312, -316, -320, -324, -329, -334, -339, -344, -350, -355, -362, -368, -375, -382, -390, -398, -406, -415, -425, -435, -446, -457, -470, -483, -497, -511, -528, -545, -563, -583, -605, -629, -655, -683, -714, -748, -786, -828, -875, -928, -989, -1058, -1138, -1231, -1341, -1474, -1636, -1839, -2100, -2449, -2937, -3669, -4891, -7335, -14668, -1, 14668, 7335, 4891, 3669, 2937, 2449, 2100, 1839, 1636, 1474, 1341, 1231, 1138, 1058, 989, 928, 875, 828, 786, 748, 714, 683, 655, 629, 605, 583, 563, 545, 528, 512, 497, 483, 470, 457, 446, 435, 425, 415, 406, 398, 390, 382, 375, 368, 362, 355, 350, 344, 339, 334, 329, 324, 320, 316, 312, 308, 305, 301, 298, 295, 292, 289, 287, 284, 282, 280, 278, 276, 274, 272, 270, 269, 267, 266, 265, 263, 262, 261, 260, 259, 259, 258, 257, 257, 256, 256, 256, 256, 256, -1, 256, 256, 256, 256, 256, 257, 257, 258, 259, 259, 260, 261, 262, 263, 265, 266, 267, 269, 270, 272, 274, 276, 278, 280, 282, 284, 287, 289, 292, 295, 298, 301, 305, 308, 312, 316, 320, 324, 329, 334, 339, 344, 350, 355, 362, 368, 375, 382, 390, 398, 406, 415, 425, 435, 446, 457, 470, 483, 497, 511, 528, 545, 563, 583, 605, 629, 655, 683, 714, 748, 786, 828, 875, 928, 989, 1058, 1138, 1231, 1341, 1474, 1636, 1839, 2100, 2449, 2937, 3669, 4891, 7335, 14668, -1, -14668, -7335, -4891, -3669, -2937, -2449, -2100, -1839, -1636, -1474, -1341, -1231, -1138, -1058, -989, -928, -875, -828, -786, -748, -714, -683, -655, -629, -605, -583, -563, -545, -528, -511, -497, -483, -470, -457, -446, -435, -425, -415, -406, -398, -390, -382, -375, -368, -362, -355, -350, -344, -339, -334, -329, -324, -320, -316, -312, -308, -305, -301, -298, -295, -292, -289, -287, -284, -282, -280, -278, -276, -274, -272, -270, -269, -267, -266, -265, -263, -262, -261, -260, -259, -259, -258, -257, -257, -256, -256, -256, -256, -256, -256,};
const int32_t sinDistTableDiv8[] = {-1, 14668, 7335, 4891, 3669, 2937, 2449, 2100, 1839, 1636, 1474, 1341, 1231, 1138, 1058, 989, 928, 875, 828, 786, 748, 714, 683, 655, 629, 605, 583, 563, 545, 528, 512, 497, 483, 470, 457, 446, 435, 425, 415, 406, 398, 390, 382, 375, 368, 362, 355, 350, 344, 339, 334, 329, 324, 320, 316, 312, 308, 305, 301, 298, 295, 292, 289, 287, 284, 282, 280, 278, 276, 274, 272, 270, 269, 267, 266, 265, 263, 262, 261, 260, 259, 259, 258, 257, 257, 256, 256, 256, 256, 256, -1, 256, 256, 256, 256, 256, 257, 257, 258, 259, 259, 260, 261, 262, 263, 265, 266, 267, 269, 270, 272, 274, 276, 278, 280, 282, 284, 287, 289, 292, 295, 298, 301, 305, 308, 312, 316, 320, 324, 329, 334, 339, 344, 350, 355, 362, 368, 375, 382, 390, 398, 406, 415, 425, 435, 446, 457, 470, 483, 497, 512, 528, 545, 563, 583, 605, 629, 655, 683, 714, 748, 786, 828, 875, 928, 989, 1058, 1138, 1231, 1341, 1474, 1636, 1839, 2100, 2449, 2937, 3669, 4891, 7335, 14668, -1, -14668, -7335, -4891, -3669, -2937, -2449, -2100, -1839, -1636, -1474, -1341, -1231, -1138, -1058, -989, -928, -875, -828, -786, -748, -714, -683, -655, -629, -605, -583, -563, -545, -528, -511, -497, -483, -470, -457, -446, -435, -425, -415, -406, -398, -390, -382, -375, -368, -362, -355, -350, -344, -339, -334, -329, -324, -320, -316, -312, -308, -305, -301, -298, -295, -292, -289, -287, -284, -282, -280, -278, -276, -274, -272, -270, -269, -267, -266, -265, -263, -262, -261, -260, -259, -259, -258, -257, -257, -256, -256, -256, -256, -256, -1, -256, -256, -256, -256, -256, -257, -257, -258, -259, -259, -260, -261, -262, -263, -265, -266, -267, -269, -270, -272, -274, -276, -278, -280, -282, -284, -287, -289, -292, -295, -298, -301, -305, -308, -312, -316, -320, -324, -329, -334, -339, -344, -350, -355, -362, -368, -375, -382, -390, -398, -406, -415, -425, -435, -446, -457, -470, -483, -497, -511, -528, -545, -563, -583, -605, -629, -655, -683, -714, -748, -786, -828, -875, -928, -989, -1058, -1138, -1231, -1341, -1474, -1636, -1839, -2100, -2449, -2937, -3669, -4891, -7335, -14668, -10451993,};

//Map for the world. 1 is a wall 0 is empty space
const	int32_t grid[15][15] = {
		
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},


{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},


{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},


{1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},


{1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},


{1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},


{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},


{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1},


{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1},


{1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1},


{1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1},


{1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1},


{1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1},


{1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},


{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		
		
	};	
	
//x1 is current player x
//x2 is the movement in the x direction
//y1 is the current player y
//y2 is the movement in the y direction
// return false if collision
// return true for no collision
uint8_t checkCollision(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
	{
		uint32_t nextX = x1 + x2;
		nextX = nextX >> 6;
		uint32_t nextY = y1+y2;
		nextY = nextY >> 6;
		
		if( grid[nextX] [nextY] == 1)
			return 1;
		return 1;
		
		
	}

int main(void)
{

	//System initalization
	PLL_Init(Bus80MHz);                  // set system clock to 80 MHz
	
	EnableInterrupts();
	SysTick_Init();
	Output_Init();
	
	SYSCTL_RCGCGPIO_R |= 0x02;        // 1) activate clock for Port B
  while((SYSCTL_PRGPIO_R&0x02)==0){}; // allow time for clock to start
                       
	
		
  GPIO_PORTB_DIR_R &= ~0x0F;
	GPIO_PORTB_DEN_R |= 0x0F;
	GPIO_PORTB_PDR_R |= 0x0F;

		
		
	//Setup variables for ray casting	
	uint16_t ceiling= ST7735_Color565(70,180,229);
	uint16_t floor = ST7735_Color565(96,72,5);
  int x = 0;
  int y = 160;
	const int32_t width = 128;
	const int32_t height = 160;

		
	const int32_t xLim = 15;
	const int32_t yLim = 15;
		
	const int32_t resolution = 2;
		
	int32_t wall_hit = 0;
	const int32_t fov = 64;	
		
	const int32_t grid_h = 64;
	const int32_t grid_w = 64;

	//int playerH = 32;
	int32_t playerPos[2] ={244, 94};
	int32_t view_angle = 137;
	
	const int32_t projection_plane[] = {width, height};
	
	const int32_t plane_center = 80;
	//width /2 / tan(fov/2) casted to an int
	int32_t planeDist = 0x66;
	const int32_t angle_inc = 1;
	
	int32_t rayAngle = view_angle + (fov / 2);
	
	
	int32_t x_move = xMoveTable[view_angle];
	int32_t y_move = yMoveTable[view_angle];
	const int32_t rotation_speed = 3;
	
	
	
	
	int32_t i = 0;
	int32_t tx;
	int32_t ty;
	int32_t hWallDist = 100000;
	int32_t vWallDist = 100000;
	int32_t y_side;
	int32_t signed_y;
	int32_t x_side;
	int32_t signed_x;	
	
	int32_t rayPos[2];
	int32_t ray_y;
	int32_t ray_x;
	int32_t x_step;
	int32_t y_step;
	int32_t temp;
	

	int32_t wallDist;
	int32_t texturePos;
	int16_t slice_height = 0;
	int16_t slice_y = 0;
	
	int32_t horWallPos;
	int32_t verWallPos;
	
	
	int32_t cosBeta;
	int32_t tCount;
	int32_t j = 0;
	
	//Start main loop
	while(1)
	{
		NVIC_ST_CURRENT_R = 0;
		
		
		//Handle control input to move player
		if( (GPIO_PORTB_DATA_R & 0x01) != 0)
		{
			
			 if(checkCollision(playerPos[0], playerPos[1], x_move, y_move))
			 {
				 playerPos[0]+=x_move;
				 playerPos[1]+=y_move;
			 }
			
			
		}
		
		else if( (GPIO_PORTB_DATA_R & 0x04) != 0 )
		{
			 if(checkCollision(playerPos[0], playerPos[1], x_move * -1, y_move * -1))
			 {
				 playerPos[0]-=x_move;
				 playerPos[1]-=y_move;
			 }
			
		}
		
		if( (GPIO_PORTB_DATA_R & 0x08) != 0 )
		{
			
			view_angle+= rotation_speed;
			if(view_angle > 359)
				view_angle-=360;
			
			x_move = xMoveTable[view_angle] / 2;
			y_move = yMoveTable[view_angle] / 2;
			
		}
		
		else if( (GPIO_PORTB_DATA_R & 0x02) != 0 )
		{
			
			view_angle -=rotation_speed;
			if(view_angle < 0)
				view_angle+=360;
	
			x_move = xMoveTable[view_angle] / 2;
	    y_move = yMoveTable[view_angle] / 2;
			
		}
		
		
		
		
		rayAngle = view_angle + 32;
		
		//Start raycasting. Loop through width of the screen to calculate and draw each line
		for(i = 0; i < width; i+=resolution)
		{
			
			
			if(rayAngle == 0 || rayAngle == 90 || rayAngle == 180 || rayAngle == 270)
				rayAngle-=1;
			
			if(rayAngle < 0)
				rayAngle+=360;
			if (rayAngle>359)
				rayAngle-=360;
	
			
			
			if(rayAngle<=90)
			{
				tx = 1;
				ty = -1;
				y_side = -1; 
				signed_y = -1;
				x_side = grid_w;
				signed_x = 1;
			}
			else if(rayAngle<=180)
			{
				tx = 1;
				ty = 1;
				y_side = -1; 
				signed_y = -1;
				x_side = -1; 
				signed_x = -1;
			}
			else if(rayAngle<=270)
			{
				tx = -1;
				ty = 1;
				y_side = grid_h;
				signed_y = 1;
				x_side = -1; 
				signed_x = -1;
			}
			else if(rayAngle<=360)
			{
				tx = -1;
				ty = -1;
				y_side = grid_h;
				signed_y = 1;
				x_side = grid_w;
				signed_x = 1;
			}
			
			
			wall_hit = 0;
			hWallDist = 100000;
			vWallDist = 100000;
			
		  ray_y = (playerPos[1] & ~0x3f) + y_side;

			temp = playerPos[1] - ray_y;
			if(temp < 0)
				temp*=-1;
			
			ray_x = (temp * tanRayTableDiv12[rayAngle] * tx) >> 12;
      ray_x += playerPos[0];
		
			x_step = ((grid_h * tanRayTableDiv12[rayAngle]) >> 12) * tx;

      y_step = grid_h * signed_y;
      rayPos[0] = ray_y >> 6; 
			rayPos[1] = ray_x >> 6;
			
			
			
			while(1)
			{
				if( rayPos[0]>=0 && rayPos[0]<yLim &&  rayPos[1]>=0 && rayPos[1]<xLim)
				{
					
					if (grid[rayPos[0]][rayPos[1]]==1)
					{
						hWallDist = ((playerPos[1] - ray_y) * sinDistTableDiv8[rayAngle]) >> 8;
						wall_hit =1;
						break;
								
					}
					else
					{
						ray_x += x_step;
						ray_y += y_step;
						rayPos[0] = ray_y >> 6;
						rayPos[1] = ray_x >> 6;
						
					}
					
				}
				else
				{
					break;
					
				}
			}

			horWallPos = ray_x;
			
      //Done finding horizontal now repeat for vertical

			ray_x = (playerPos[0] & ~0x3f) + x_side;

			temp = playerPos[0] - ray_x;
			if(temp < 0)
				temp*=-1;
			
			ray_y = (temp * tanRayTableTim12[rayAngle] * ty) >> 12;
      ray_y += playerPos[1];
		
			y_step = ((grid_w * tanRayTableTim12[rayAngle]) >> 12) * ty;

      x_step = grid_w * signed_x;
      rayPos[0] = ray_y >> 6; 
			rayPos[1] = ray_x >> 6;
			
			
			
			while(1)
			{
				if( rayPos[0]>=0 && rayPos[0]<yLim &&  rayPos[1]>=0 && rayPos[1]<xLim)
				{
					
					if (grid[rayPos[0]][rayPos[1]]==1)
					{
						vWallDist = ((playerPos[0] - ray_x) * cosDistTableDiv8[rayAngle]) >> 8;
						wall_hit =1;
						break;
								
					}
					else
					{
						ray_x += x_step;
						ray_y += y_step;
						rayPos[0] = ray_y >> 6;
						rayPos[1] = ray_x >> 6;
						
					}
					
				}
				else
				{
					break;
					
				}
			}			
			
		
			verWallPos = ray_y;
			
			
			
	
			
			
			//Now check which wall is the closest
	
			if(wall_hit)
			{
				
				if(vWallDist < hWallDist)
				{
					wallDist = vWallDist;
			    texturePos = verWallPos % grid_w;
					
					
				}
				else
				{
					wallDist = hWallDist;
					texturePos = horWallPos % grid_w;
				}
				
				
				
//				tCount = 0;
//				for(j = texturePos * grid_w; j < (grid_w * (texturePos+ 1)) -1; j++)
//				{
//					texture[tCount] = dk_wall[j];
//					tCount++;
//					
//				}
//				
				
			
				
				cosBeta = view_angle - rayAngle;
				if(cosBeta < 0)
					cosBeta+=360;
				if(cosBeta != 0 && cosBeta != 180)
				{
					cosBeta = cosTableTim8[cosBeta];
					wallDist = (wallDist * cosBeta) >> 8;
				}
			
				
				slice_height = (grid_h * planeDist) / wallDist;
			
		
				
				
				
				slice_y =  plane_center - (slice_height >> 1);
				scaleTexture(slice_height, texturePos);
				
			  drawCastTexture(i,  slice_y, slice_height,ceiling, floor);
				drawCastTexture(i+1,  slice_y, slice_height, ceiling, floor);
				//drawCastLineRect(i, slice_y, resolution, slice_height,, 0x10A0);
			
	
				
				  
				//ST7735_DrawFastVLine(i, slice_y, slice_height, 0xA010);
				//ST7735_DrawFastVLine(i+1, slice_y, slice_height, 0xA010);
				rayAngle-= 1;
				
			}
			
		}
		
		
		 //DelayWait10ms(100);
		//ST7735_FillScreen(0xFFFF);
		
		
	}





}


// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
