#include <reg51.h> 
#include "lcd.h"  	 
#include "delay.h"
#include"stdutils.h"
#include<stdio.h>
sbit READ_ADC=P3^0; //Read(RD) pin of ADC0804
sbit WRITE_ADC=P3^1; //Write(WR) pin of ADC0804
sbit INTR_ADC=P3^2; //Interrupt(INTR) pin of ADC0804 
unsigned char ADC_VALUE;
char Tempr[] = " 00.0 C"; //set Temp display format in Celsius
int C;    

/* main program */
void main() 
{ 
	  P1=0xFF; //make Port 1 as input port
  	INTR_ADC=1; //make INTR pin as input
    READ_ADC=1; //set RD pin HIGH
    WRITE_ADC=1; //set WR pin LOW
	  LCD_Init();
    lcd_DisplayString(" MICRODIGISOFT ");
    lcd_Line_posnY();
    lcd_DisplayString("LM35 - ADC0804 ");
	  delay_ms(1000);
	  lcd_Clear();
    while(1)
    {
	 WRITE_ADC=0; //send LOW to HIGH pulse on WR pin
   delay_ms(1);
   WRITE_ADC=1;
   while(INTR_ADC==1); //wait for End of Conversion
   READ_ADC=0; //make RD = 0 to read the data from ADC0804
   ADC_VALUE=P1; //copy ADC data
  // convert_display(ADC_VALUE); //function call to convert ADC data into temperature and display it on     16*2 lcd display
   
		C = ADC_VALUE*1.95;               // // Read analog voltage and convert it to degree Celsius 
    if (C > 99)
    Tempr[0]  = 1 + 48;              // Put 1 (of hundred)
    else
    Tempr[0]  = ' ';                 // Put space
    Tempr[1]  = (C / 10) % 10 + 48;
    Tempr[2]  =  C % 10  + 48;
    Tempr[5] = 223;                    // Put degree symbol ( ° )	

	 lcd_DisplayString("TEMPERATURE");
	 lcd_Line_posnY();
   lcd_DisplayString(Tempr);
	 delay_ms(10);  //interval between every cycles
   READ_ADC=1;   //make RD = 1 for the next cycle
	delay_ms(1000);  //interval between every cycles
   lcd_Clear();		

     }
}
