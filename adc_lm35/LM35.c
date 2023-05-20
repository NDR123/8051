#include<reg51.h>

#include<string.h>

 

sbit RS = P3^0; 

sbit RW = P3^1;

sbit EN = P3^3;

sbit ale=P2^3;  

sbit oe=P2^4; 

sbit start=P2^6; 

sbit eoc=P2^5;  

sbit clk=P2^7; 

sbit chc=P2^0;  //Address pins for selecting input channels.

sbit chb=P2^1;

sbit cha=P2^2;

 

void delay(int t); 

void lcd_init(void);

void lcd_command(char c);

void lcd_data(char d);

void str(char a[]);

void print( long float p);

long float k;

unsigned long int q,r,x,y,z;

 

void timer0() interrupt 1  // TIMER 0 interrupt ISR

{

    clk=~clk;

}

 

void main()            // MAIN PROGRAM

{

lcd_init();                                // lcd initialization

str("!!welcome!!");

lcd_command(0x01);  // clear display

str("Temp:");

lcd_command(96); //custom character (°c) display 

lcd_data(0x10);

lcd_data(0x07);

lcd_data(0x08);

lcd_data(0x08);

lcd_data(0x08);

lcd_data(0x08);

lcd_data(0x07);

lcd_command(0x8b);

lcd_data(4);

eoc=1;             // make eoc an input

ale=0;

oe=0;

start=0;

TMOD=0x02;    // timer 0 in mode 2

TH0=0xc2;                   //     15khz

IE=0x82; //  set timer 0 interrupt

TR0=1;   // start timer 0

while(1)

{ 

chc=0;  // select channel 0

chb=0;

cha=0;

ale=1;  // send high to low pulse on start and ale pin

start=1;

delay(1);

ale=0;

start=0;

while(eoc==1);     // wait for conversion

while(eoc==0); 

oe=1;

k=P1;

lcd_command(0x85);

print(k);   // send the digital data to lcd 

oe=0;

}

}

void str(char a[])  // lcd function to display string

{

int j;

for(j=0;a[j]!='\0';j++)

{

lcd_data(a[j]);

}

}

void lcd_init(void)  // lcd initialization

{

lcd_command(0x38);        //8 bit,2 line,5x8 dots 

lcd_command(0x01);        // clear display

lcd_command(0x0f);      // display on, cursor blinking

lcd_command(0x06);     //Entry mode

lcd_command(0x0c); //cursor off

lcd_command(0x80); //// force cursor to beginning of first row

}

void lcd_command(char c) // lcd command function

{

P0=c;

RS=0;

RW=0;    // select command register

EN=1;

delay(5);

EN=0;

delay(5);

}

void lcd_data(char d) // lcd data function

{

P0=d;

RS=1;               //select data register

RW=0;

EN=1;

delay(5);

EN=0;

delay(5);

}

void delay(int t)      // delay function

{

int j;

for(j=0;j<t*1275;j++);

}

void print( long float p)  // number display function

{

x=p*10;

if(x>=1000)

{

q=x/1000;

q=q+48;

y=(x%1000)/100;

y=y+48;

z=((x%1000)%100)/10;

z=z+48;

r=x%10;

r=r+48;

lcd_data(q);

lcd_data(y);

lcd_data(z);

lcd_data(46);      //ascii value of point

lcd_data(r);

}

else

{

q=x/100;

q=q+48;

y=(x%100)/10;

y=y+48;

z=x%10;

z=z+48;

lcd_data(q);

lcd_data(y);

lcd_data(46);                      //ascii value of point

lcd_data(z);

r=0;

lcd_data(r);

}

}