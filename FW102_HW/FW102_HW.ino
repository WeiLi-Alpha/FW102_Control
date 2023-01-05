/*
This project converts the ESP32 to the controller of the FW102. It sends pulses 
to FW102 to rotate the wheel and therefore switch the filters on the wheel. 
Each step takes about one second.

/////////////////////////////////////////////////////////////////////////////////
The serial communication uses the format of "[CMD0] [CMD1]" separated by a space.
The protocol is as following:
--------------------------------------------------------------------------------
CMD0  | CMD1 | Description
--------------------------------------------------------------------------------
POS   |      | request the current position, returns the current position number
      |      | between 1 and 6
--------------------------------------------------------------------------------
SET   | NUM  | NUM is a number (or the reminder) between 1 and 6. It sets the 
      |      | position to the position of NUM
--------------------------------------------------------------------------------
RESET | NUM  | NUM is a number (or the reminder) between 1 and 6. It resets the 
      |      | position reading to the NUM without rotating the wheel. This is
      |      | used in the initialization to match the position reading on the
      |      | device.
--------------------------------------------------------------------------------
STOP  |      | Stop the rotation.
--------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////
Wiring:
The external triger cable is connected to two pins: P1 and P2.
In this case, D2 and D4 are used.
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define BAUDRATE 115200
#define CMD_LENGTH 10
#define OBJ_MAX 6
#define PULSE_WIDTH 500

int P1 = 2; 
int P2 = 4; 

char CMD0[CMD_LENGTH]={'0'};// global variables of the received commands 
char CMD1[CMD_LENGTH]={'0'};
char CMD2[CMD_LENGTH]={'0'};


void setup()
{
  Serial.begin(BAUDRATE, SERIAL_8N1, 3, 1);
  //Serial.begin(BAUDRATE);
  int  icount = 0;
/*  
  while (!Serial) {
    Serial.print("waiting for connection for %d seconds");
    delay(10); // wait for serial port to connect. Needed for native USB
    icount = icount + 1;
  }
  Serial.print("Connected...");
*/
  pinMode(P1, OUTPUT);
  pinMode(P2, OUTPUT);  
  digitalWrite(P1,LOW);
  digitalWrite(P2,LOW);
}

//--------------------------------------------
// read commands from the default serial port, i.e. the default UART port.
void readCommand()
{
  int str_in_max=Serial.available();
  //printf("str_in_max:%d \n",str_in_max);
 
  memset(CMD0,'\0',CMD_LENGTH);
  memset(CMD1,'\0',CMD_LENGTH);
  memset(CMD2,'\0',CMD_LENGTH);
 
  char str_in[str_in_max]={'0'};

  int flag_cmd0_read=1;
  int flag_cmd1_read=1;
  int flag_cmd2_read=1;
  
  int icount0 = 0;
  int icount1 = 0;
  int icount2 = 0;
  for(int itemp=0;itemp<str_in_max;itemp++)
  {
    str_in[itemp]=Serial.read();

    // start receiving CMD0
    if(!isspace(str_in[itemp])&&flag_cmd0_read)
    {
      CMD0[icount0]=str_in[itemp];
      icount0 = icount0 + 1;
    }
    else
    {//CMD0 is done, set the flag to 
      flag_cmd0_read=0;
      //icount0 = 0;
    }
    CMD0[icount0] = '\0';

    // start receiving CMD1
    if(flag_cmd0_read==0&&!isspace(str_in[itemp])&&flag_cmd1_read)
    {// start from the first letter after spaces
      if(!isspace(str_in[itemp])&&flag_cmd1_read)
      {
        CMD1[icount1]=str_in[itemp];
        icount1 = icount1 + 1;
      }
      else
      {//CMD1 is done, set the flag to 0
        flag_cmd1_read=0;
        //icount1 = 0;
      }
    }
    else
    { // already read some information and come to the next space
      if(icount1)
      {
        flag_cmd1_read=0;
      }
    }
    CMD1[icount1] = '\0';

    // start receiving CMD2
    if(flag_cmd1_read==0&&!isspace(str_in[itemp])&&flag_cmd2_read)
    {// start from the first letter after spaces
      if(!isspace(str_in[itemp])&&flag_cmd2_read)
      {
        CMD2[icount2]=str_in[itemp];
        icount2 = icount2 + 1;
      }
      else
      {//CMD0 is done, set the flag to 
      flag_cmd2_read=0;
      //icount2 = 0;
      }
    }
    else
    { 
      if(icount2) // already read some information and come to the next space
      {
        flag_cmd2_read=0;
      }
    }
    CMD2[icount2] = '\0';

  } 
  str_in[str_in_max-1] = {'\0'};
  //printf("Read:%s\n",str_in);
  //printf("CMD0:%s\n",CMD0);
  //printf("CMD1:%s\n",CMD1);
  //printf("CMD2:%s\n",CMD2);

  //delay(1000);
}


int POS=0;
int OBJ=0;
int OFFSET = OBJ - POS;

void reSet()
{
  int val = atoi(CMD1)%OBJ_MAX;
  if(val<0)
  {
    val = val + OBJ_MAX;
  }
  if(val==0)
  {
    val=OBJ_MAX;
  }
  POS = val;
  OBJ = val;
}

void setObj()
{
  int val = atoi(CMD1)%OBJ_MAX;
  if(val<0)
  {
    val = val + OBJ_MAX;
  }
  if(val==0)
  {
    val=OBJ_MAX;
  }
  OBJ = val;
}

int calcOffset()
{

  int val = OBJ-POS;
  return val;
}

void stopRotation()
{
  OBJ = POS;
}


void loop()
{
  if(Serial.available()>1)
  {
    //printf("--------------------------------\n");
    readCommand();
    
    /*
    printf("CMD0:%s\n",CMD0);
    printf("CMD1:%s\n",CMD1);
    printf("CMD2:%s\n",CMD2);
    */

    if(strcmp(CMD0,"POS")==0)
    {
      Serial.println(POS);
   }

    if(strcmp(CMD0,"RESET")==0)
    {
      reSet();
      //printf("ASET Done...\n");
      Serial.println("OK");
    }

    if(strcmp(CMD0,"SET")==0)
    {
      setObj();
      Serial.println("OK");
    }
    
    if(strcmp(CMD0,"STOP")==0)
    {
      stopRotation();
      Serial.println("OK");
    }
    
  }
  
  OFFSET= abs(OBJ - POS);

  if(OFFSET!=0)
  {
    digitalWrite(P1,HIGH);
    delay(PULSE_WIDTH);
    digitalWrite(P1,LOW);
    delay(PULSE_WIDTH);
    POS = POS + 1;
  }
  if(POS >OBJ_MAX)
  {
    POS = 1;
  }

  //printf("POS=%d\n",POS);
  //delay(500);
}
