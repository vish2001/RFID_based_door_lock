
#include<reg51.h>
#include<string.h>
#include<stdio.h>
#define LCDPORT P1

sbit rs=P1^0;
sbit rw=P1^1;
sbit en=P1^2;
sbit Motor1=P2^4;
sbit Motor2=P2^3;
sbit Speaker=P2^6;
char i,rx_data[50];
char rfid[13],ch=0;
int counter1, counter2, counter3;
unsigned char result[1];

 void delay(int itime) //delay funtion
{
    int i,j;
    for(i=0;i<itime;i++)
    for(j=0;j<1275;j++);
}

void daten() // data function
{
    rs=1;
    rw=0;
    en=1;
    delay(5);
    en=0;
}

void lcddata(unsigned char ch) //To send the string to be dislayed on lcd
{
    LCDPORT=ch & 0xf0;
    daten();
    LCDPORT=(ch<<4) & 0xf0;
    daten();
}

void cmden(void)// command function
{
    rs=0;
    en=1;
    delay(5);
    en=0;
}

void lcdcmd(unsigned char ch)// To send the command the lcd has to perform
{
    LCDPORT=ch & 0xf0;
    cmden();
    LCDPORT=(ch<<4) & 0xf0;
    cmden();
}

void lcdstring(char *str) // To get the  string to be dislayed on lcd
{
    while(*str)
    {
        lcddata(*str);
        str++;
    }
}

void lcd_init(void) // To initialise the lcd
{
    lcdcmd(0x02);
    lcdcmd(0x28);
    lcdcmd(0x0e);
    lcdcmd(0x01);
}

void uart_init()//To initialize the serial connection
{
 TMOD=0x20;
 SCON=0x50;
 TH1=0xfd;
 TR1=1;
}
char rxdata() //setting up serial connection for the rfid module
{
  while(!RI);
    ch=SBUF;    
    RI=0;
    return ch;
}

void main()
{
    Speaker=1;
    uart_init();
    lcd_init();
    lcdstring("---RFID Based---");
    lcdcmd(0xc0);
    lcdstring("DoorLock System"); 
    delay(500);
    while(1)
    {
        lcdcmd(1);
        lcdstring("Scan Your Card:");
        lcdcmd(0xc0);
        i=0;
        for(i=0;i<12;i++)
        rfid[i]=rxdata();
        rfid[i]='\0';
        lcdcmd(1);
			lcdstring("Rfid No. is:");
        lcdcmd(0xc0);
        for(i=0;i<12;i++)
        lcddata(rfid[i]);
        delay(100);
        if(strncmp(rfid,"100027A476G2",12)==0)// checking if the RFID tag is same as the authorized ones
        {
            counter1++;//To count the number of members entering with the same ID
            lcdcmd(1);             
            lcdstring(" Authorised");
            delay(200);
            lcdcmd(1);
            lcdstring(" Vishwanath ");
            lcdcmd(0xc0);
            lcdstring("Member: ");
            sprintf(result, "%d", counter1);
            lcdstring(result);
            
            Motor1=1;
            Motor2=0;
            delay(300);//Opening the door
            Motor1=0;
            Motor2=0;
            delay(200);//closing the door
            Motor1=0;
            Motor2=1;
					  delay(300);
            Motor1=0;
            Motor2=0;
        }
        
        else if(strncmp(rfid,"1600ABCD147A",12)==0)// checking if the RFID tag is same as the authorized ones
            {
            counter2++;//To count the number of members entering with the same ID
            lcdcmd(1);
            lcdstring(" Authorised");
            delay(200);
            lcdcmd(1);
            lcdstring("Gaurav");
            lcdcmd(0xc0);
            lcdstring("Member: ");
            sprintf(result, "%d", counter2); 
            lcdstring(result);
            
            Motor1=1;
            Motor2=0;
            delay(300);//Opening the door
            Motor1=0;
            Motor2=0;
            delay(200);//closing the door
            Motor1=0;
            Motor2=1;
            delay(300);
            Motor1=0;
            Motor2=0;
      }
            
       else if(strncmp(rfid,"1600ADC369A1",12)==0)// checking if the RFID tag is same as the authorized ones
            {
            counter3++;//To count the number of members entering with the same ID
            lcdcmd(1);
            lcdstring(" Authorised");
            delay(200);
            lcdcmd(1);
            lcdstring(" Adarsh ");
            lcdcmd(0xc0);
            lcdstring("Member: ");
            sprintf(result, "%d", counter3);
            lcdstring(result);
            
            Motor1=1;
            Motor2=0;
            delay(300);//Opening the door
            Motor1=0;
            Motor2=0;
            delay(200);
            Motor1=0;
            Motor2=1;
            delay(300);//closing the door
            Motor1=0;
            Motor2=0;

            }

        else 
        {
           lcdcmd(1);
           lcdstring("ACCESS DENIED"); //Access denied for any unauthorized person.
					 lcdcmd(0xc0);
           lcdstring("Try Another Card");
           Speaker=0;// Buzzer alarm is turned on to alert
           delay(300);
           Speaker=1;
        }
  }
}