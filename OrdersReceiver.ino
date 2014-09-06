/*------------------------------------------------------------------------------*\
 * This source file is subject to the GPLv3 license that is bundled with this   *
 * package in the file COPYING.                                                 *
 * It is also available through the world-wide-web at this URL:                 *
 * http://www.gnu.org/licenses/gpl-3.0.txt                                      *
 * If you did not receive a copy of the license and are unable to obtain it     *
 * through the world-wide-web, please send an email to                          *
 * siempre.aprendiendo@gmail.com so we can send you a copy immediately.         *
 *                                                                              *
 * @category  Robotics                                                          *
 * @copyright Copyright (c) 2011 Jose Cortes (http://www.softwaresouls.com) *
 * @license   http://www.gnu.org/licenses/gpl-3.0.txt GNU v3 Licence            *
 *                                                                              *
\*------------------------------------------------------------------------------*/

//#include <Ultrasonic.h>
#include <OLLO.h>

// enum bool {false, true};
// typedef enum {false, true} bool;

/*
// CM-510 functions

#define F_NO_FUNCTION  0
#define F_GET_SENSOR_VALUE_RAW  1
#define F_GET_TWO_DMS_SENSOR_VALUES 2
#define F_GET_MULTIPLE_SENSOR_VALUE 3
#define F_GET_SENSOR_VALUE_FILTERED  4
#define F_SET_SENSOR_VALUES_TO_FILTER 5
#define F_GET_BATTERY_MILLIVOLTS 6
#define F_SET_BATTERY_LOW_LEVEL_THRESHOLD 20
#define F_SET_BATTERY_VERY_LOW_LEVEL_THRESHOLD 21

#define F_BEEP 8
#define F_DO_SENSOR_SCAN 9
#define F_SET_VALUE_DMS1 11
#define F_SET_VALUE_DMS2 12

#define Max_Parameters_Values 10
*/

#define Max_Parameters_Values 10
#define DID 94

#define InputUSB 1
#define InputDynamixel 2


#define F_NO_FUNCTION 0
#define RawOlloRead 1
#define BLINK 8
#define SensorUSRead 101

#define NEXT_IS_HEADER 0
#define NEXT_IS_ID 1
#define NEXT_IS_LENGTH 2
#define NEXT_IS_COMMAND 3
#define NEXT_IS_PARAM1 4
//#define NEXT_IS_PARAM2 5
#define NEXT_IS_CHECKSUM 20

unsigned char nextIs=NEXT_IS_HEADER;
unsigned char isToMe=0;

const unsigned char PacketHeaderSize = 2;
unsigned char numberOfParameters=0;
unsigned char parametersReceived=0;
unsigned char parameters[Max_Parameters_Values+1];

int values[Max_Parameters_Values+1];

int id;
int position;

bool debugOutputSerial2=false;
bool debugOutputOn=false;
//bool tossModeOn=false;
char charRead;
byte dxDataAvailable=0;

Dynamixel Dxl(1);

//Ports 1,2 for IR
//Ports 3,4 for DMS     

// Port 1 a 4 IR
const int MaximunNumberSensors=4;
OLLO DMSSensors[MaximunNumberSensors];
OLLO IRSensors[MaximunNumberSensors];

//Ultrasonic SensorUS(8,7);

void debugPrint(byte b)
{
  if (debugOutputOn)
  {
    SerialUSB.print(b);
  }
  
  if (debugOutputSerial2)
  {
    Serial2.print(b);
  }
}

void debugPrint(char c)
{
  if (debugOutputOn)
  {
    SerialUSB.print(c);
  }
  
  if (debugOutputSerial2)
  {
    Serial2.print(c);
  }
}

void debugPrintln(char c)
{
  if (debugOutputOn)
  {
    SerialUSB.println(c);
  }
  
  if (debugOutputSerial2)
  {
    Serial2.println(c);
  }
}

void debugPrint(int i)
{
  if (debugOutputOn)
  {
    SerialUSB.print(i);
  }
  
  if (debugOutputSerial2)
  {
    Serial2.print(i);
  }
}

void debugPrintln(int i)
{
  if (debugOutputOn)
  {
    SerialUSB.println(i);
  }
  
  if (debugOutputSerial2)
  {
    Serial2.println(i);
  }
}

void debugPrint(char *buffer, bool newlineAfterMessage)
{
  if (debugOutputOn)
  {
    if (newlineAfterMessage)
      SerialUSB.println(buffer);
    else
      SerialUSB.print(buffer);
  }
  
  if (debugOutputSerial2)
  {
    if (newlineAfterMessage)
      Serial2.println(buffer);
    else
      Serial2.print(buffer);
  }  
}

void debugPrint(char *buffer, int value)
{
  if (debugOutputOn)
  {
      SerialUSB.println ("");
      SerialUSB.print(buffer);
      SerialUSB.print (" ");
      SerialUSB.println(value);
  }
  
  if (debugOutputSerial2)
  {
      Serial2.println ("");
      Serial2.print(buffer);
      Serial2.print (" ");
      Serial2.println(value);
  }  
}

void debugPrint(char *buffer, int value1, int value2)
{
  if (debugOutputOn)
  {
      SerialUSB.println ("");
      SerialUSB.print(buffer);
      SerialUSB.print (" ");
      SerialUSB.print(value1);
      SerialUSB.print (" ");
      SerialUSB.println(value2);
  }
  
  if (debugOutputSerial2)
  {
      Serial2.println ("");
      Serial2.print(buffer);
      Serial2.print (" ");
      Serial2.print(value1);
      Serial2.print (" ");
      Serial2.println(value2);
  }   
}

void debugPrint(char *buffer1, char *buffer2)
{
  if (debugOutputOn)
  {
      SerialUSB.println ("");
      SerialUSB.print(buffer1);
      SerialUSB.print (" ");
      SerialUSB.println(buffer2);
  }
  
  if (debugOutputSerial2)
  {
      Serial2.println ("");
      Serial2.print(buffer1);
      Serial2.print (" ");
      Serial2.println(buffer2);
  }  
}

void debugOutputChar(char aChar)
{
  debugPrint(aChar);
}

void debugOutput(byte aByte)
{
  debugPrint(aByte);
}

void debugOutput(byte aByte, bool isChar)
{
      if (isChar)
        debugPrint((char)aByte);
      else
        debugPrint(aByte);
}

void debugOutput(char *buffer, bool newlineAfterMessage)
{
  debugPrint(buffer, newlineAfterMessage);
}

void debugOutput(int value, bool newlineAfterMessage)
{
    if (newlineAfterMessage)
      debugPrintln(value);
    else
      debugPrint(value);
}

void debugOutputPair(char *buffer, int value)
{
   debugPrint(buffer, value);
}

void debugOutputTrio(char *buffer, int value1, int value2)
{
  debugPrint(buffer, value1, value2);
}

void debugOutputPair(char *buffer1, char *buffer2)
{
  debugPrint(buffer1, buffer2);
}

void debugOutputPairSerial(char *buffer, int value)
{
  if (debugOutputSerial2)
  {
      Serial2.println ("");
      Serial2.print(buffer);
      Serial2.print (" ");
      Serial2.println(value);
  }
}

/*
void USBDataReceived(byte* buffer, byte nCount)
{
  int i;
  for (i=0;i<nCount;i++)
  {    
    //debugOutputTrio("  @", i, buffer[i]);    
    processData(buffer [i]);
    blinkOnce();
  }
}
*/

char GetOnlyYesOrNo()
{
    char charRead=' ';
    while (charRead!='y' && charRead!='Y' && charRead!='n' && charRead!='N')
      charRead=SerialUSB.read();

    return charRead;
}

/*
void checkIfTossModeOn()
{
    Serial.print("v12 Put TossMode ON?, y/n");
    charRead=SerialUSB.read();
    
    if (charRead=='y' || charRead=='Y')
      tossModeOn=true;
    else
      tossModeOn=false;
       
    debugOutputPair ("TossModeOn is:", (tossModeOn?1:0));
}
*/

void checkIfDebugOn()
{  
    SerialUSB.print("Put debug ON?, y/n");
    charRead=SerialUSB.read();
    
    if (charRead=='y' || charRead=='Y')
      debugOutputOn=true;
    else
      debugOutputOn=false;       
    
    debugOutputPair ("DebugPut is:", debugOutputOn);
}

unsigned char headerCounter=0;
int serialPacketByteCounter=0;
int firstHeaderByteCounter=0;

unsigned char checkPacketHeader(byte data)
{
  unsigned char headerDetected=0;

  if (data==0xff)
  {
    debugOutputPair("\n\nheaderCounter:%d pos:%d\n\n", headerCounter);
    debugOutputPair("\n\nserialPacketByteCounter:%d pos:%d\n\n", serialPacketByteCounter);
    if (headerCounter==0)
    {
      headerCounter++;
      firstHeaderByteCounter=serialPacketByteCounter;
    }
    else
    {      
      debugOutputTrio("\n\n2nd header prev curr:\n\n", firstHeaderByteCounter, serialPacketByteCounter);
      if (serialPacketByteCounter==firstHeaderByteCounter+1)
      {
        debugOutput("\n\nheader hit\n\n",0);
	headerDetected=1;
	headerCounter=0;
	if (serialPacketByteCounter>1000)
          serialPacketByteCounter=0;
      }
      else
      {
        headerCounter=0;
        firstHeaderByteCounter=0;
      }
    }
  }
  else
  {
    if (headerCounter>0)
    {
      headerCounter=0;
    }

  }
  serialPacketByteCounter++;
  
  return headerDetected;
}


// [Headers] [DID] [Function code] [Param1] [Param2] [ParamN]

int processInputData(unsigned char data)
{
  int processCM510Function=1;
  debugOutputTrio("\n\nprocessInputData: nextIs, Data:\n\n", nextIs, data);
//  debugOutputPair("\n\nprocessInputData: parametersReceived:\n\n", parametersReceived);
  
  if (nextIs==NEXT_IS_ID)
  {		
    if (data==DID)
    {
      isToMe=1;
      debugOutputPair("Is my DID: ", data);
      nextIs=NEXT_IS_LENGTH;
    }
    else
    {
      isToMe=0;
      debugOutputPair("NOT Is my DID: ", data);
      nextIs=NEXT_IS_HEADER;
    }
  }
  else
  {
        		if (nextIs==NEXT_IS_LENGTH)
        		{
        			numberOfParameters=data-PacketHeaderSize;
        
        			debugOutputTrio("\n\nmessage length:%d number of parameters:%d\n\n", data, numberOfParameters);
        
        			nextIs=NEXT_IS_COMMAND;
        		}
        		else
              {
        	
                  if (nextIs==NEXT_IS_COMMAND)
                  {       
                    nextIs=NEXT_IS_PARAM1;
                  }
                  else
                  {
        				if (nextIs>=NEXT_IS_PARAM1 && nextIs<NEXT_IS_CHECKSUM)
        				{
        					debugOutputPair("\n\nnextIs>=NEXT_IS_PARAM1 && nextIs<NEXT_IS_CHECKSUM:\n\n", isToMe);
        			
        					if (isToMe)
        					{
        						parameters[parametersReceived++]=data; //param1=data;
        
        						debugOutputTrio ("\n\nparametersReceived, parameter:", parametersReceived, parameters[parametersReceived-1]);
        
        						if ((parametersReceived>=numberOfParameters) 
        							|| (parametersReceived>=Max_Parameters_Values))						
        						{
        							nextIs=NEXT_IS_CHECKSUM;
        						}
        					}
        				}
        				else
        				{
        						if (nextIs==NEXT_IS_CHECKSUM)
        						{
        							debugOutput("\n\nnextIs=NEXT_IS_CHECKSUM:isToMe:\n\n", isToMe);
        							
        							
        							if (isToMe)
        								processCM510Function=2;	
        							else
        								processCM510Function=1;
        
        							nextIs=NEXT_IS_HEADER;
        						}
        				}						
                    }
                }
  }

        
      if(checkPacketHeader(data))
      {
        parametersReceived=0;
        isToMe=0;
        nextIs=NEXT_IS_ID;
      }
  
  return processCM510Function;
  //printf( "\n\noutput state:%d data:%X isToMe:%d\n\n", nextIs, data, isToMe);
}

void blinkOnce()
{
    digitalWrite(BOARD_LED_PIN, LOW);
    delay(5);
    digitalWrite(BOARD_LED_PIN, HIGH);
//    delay(100);
}

int readOlloIRSensor(int port, int times=1)
{
  int maxRead=0;
  
  int sensorValue=0;
  debugOutputPair("readOlloSensor port", port);
  for (int i=1;i<=times;i++) {
    sensorValue = IRSensors[port].read(port, IR_SENSOR);
    //SerialUSB.println(sensorValue);
    if (sensorValue>maxRead)
     maxRead=sensorValue;
  }
  
  debugOutputPair("readOlloSensor value", maxRead);
  
////  SerialUSB.print("Max: ");
//  SerialUSB.println(maxRead);
  
  return maxRead;
}

int readOlloDMSSensor(int port, int times=1)
{
  int maxRead=0;
  
  int sensorValue=0;
  debugOutputPair("readOlloSensor port", port);
  for (int i=1;i<=times;i++) {
    sensorValue = DMSSensors[port].read(port);//analogRead(analogInPin);
    //SerialUSB.println(sensorValue);
    if (sensorValue>maxRead)
     maxRead=sensorValue;
  }
  
  debugOutputPair("readOlloSensor value", maxRead);
  
////  SerialUSB.print("Max: ");
//  SerialUSB.println(maxRead);
  
  return maxRead;
}

/*
// http://softwaresouls.com/softwaresouls/2013/06/17/robotis-cm-900-in-toss-mode-and-querying-connected-sensors/
Example for blinking twice: { 0xFF, 0xFF, 90, 1, 1, 2 }

Headers: 0xFF 0xFF

ID: 90

Command code: 1

Number of parameters: 1

Parameter 1: 2

Example for reading ultrasonic sensor (HC-SR04) : { 0xFF, 0xFF, 90, 2, 0 }
*/

int executeFunction()
{
  int function=parameters[0];
  int ret=0;
  
  debugPrint("executeFunction() function:", function);
  switch(function)
  {
    case RawOlloRead:
      debugOutputPairSerial("RawOlloDMSRead Function", function);
      
      if (parametersReceived>0)
      {
        int port=parameters[1];
        
        //Ports 1,2 for IR
        //Ports 3,4 for DMS
        
        if (port<=MaximunNumberSensors)
          ret=readOlloIRSensor(port);
        else
          ret=readOlloDMSSensor(port);
          
        debugOutputPair("Read", ret);    
      }
      break;
      
    case BLINK:
      debugOutputPair("Blink Function", function);
      if (parametersReceived>0)
      {
        for (int i=0;i<parameters[1];i++)
          blinkOnce();
      }
      break;
                                        
    case SensorUSRead:
      debugOutputPairSerial("SensorUSRead Function", function);
      ret=0;//SensorUS.readDistance();
      debugOutputPair("Read", ret);
      break;
   }
   
   return ret;
}

void toHexHLConversion(int pos, unsigned char *hexH, unsigned char *hexL)
{    
    *hexH = (unsigned char)(pos >> 8);
    *hexL = (unsigned char)pos;
	//printf( "\n\nHL conversion value:%X h:%X l:%X\n\n", pos, *hexH, *hexL);
}

unsigned char checkSumatory(unsigned char *data, int length)
{
    int cs = 0;
    for (int i = 2; i < length; i++)
    {
        cs += data[i];
    }            
    return (unsigned char)~cs;
}

int getResponseValueCommand(unsigned char *buffer, unsigned char id, int value)
{
    //OXFF 0XFF ID LENGTH INSTRUCTION PARAMETER1 â€¦PARAMETER N CHECK SUM
    int pos = 0;

	// header
    buffer[pos++] = 0xff;
    buffer[pos++] = 0xff;

	// id
    buffer[pos++] = id;

	// length
    buffer[pos++] = 4;

	//error
    buffer[pos++] = 0; 

    debugPrint ("value:",value);

    unsigned char hi=0, lo=0;
    toHexHLConversion(value, &hi, &lo);
    
    // low
    buffer[pos++] = lo;

    // hi
    buffer[pos++] = hi;

    unsigned char checksum = checkSumatory(buffer, pos);
    buffer[pos++] = checksum;
    
    return pos;
}

// Create and write response taking the values from the executeCM510Function function
void response(int function, int input, int value)
{
  unsigned char length=0;
  unsigned char buffer[128];

  debugPrint("response mode and value: ", input, value);

  switch (function)
  {	
    case SensorUSRead:
      values[0]=function;
      length=getResponseValueCommand(buffer, DID, value);
      break;
      
      default:
        length=getResponseValueCommand(buffer, DID, value);
	break;
  }

  if (input==InputUSB)
    SerialUSB.write(buffer, length);
  else
  {
    for (int i=0;i<length;i++)
      Dxl.writeRaw(buffer [i]);
  }    
}

void process(byte data, int input)
{
   debugOutputPair("process (byte), data:", data);      
   
   toggleLED();
   
   int processTossModeORFuntion=processInputData(data);
   int function=F_NO_FUNCTION;
   
   switch (processTossModeORFuntion)
   {
     case 1:   
/*     
       debugPrint("To Dxl:", data);
       if (input==InputUSB)
       {         
         Dxl.writeRaw(data);
         //blinkOnce();
       }
       else
       {
         if (input==InputDynamixel)
           SerialUSB.write(data);
       }
*/       
       break;
     case 2:        
       function=parameters[0];    
       response(function, input, executeFunction());
       break;       
   }  
}

void setup()
{  
  pinMode(BOARD_LED_PIN, OUTPUT);
//  pinMode(16,OUTPUT); // US sensor

//  onlyOnceHappened=false;
//  counter=0;
//  debugOutputOn=false;

  //USB Serial initialize
//  Serial2.begin(1);

  for (int i=0;i<MaximunNumberSensors;i++)
  {
    //DMSSensors[i].begin(i+1);// all Ollo ports initialized
    IRSensors[i].begin(i+1, IR_SENSOR);
  }
    
  Dxl.begin(1);
  
  SerialUSB.begin();
  //Serial2.begin(57600);
//  Serial.attachInterrupt(USBDataReceived);
//  delay (3000);
  
  debugOutput("Setuped", true);
  
//    checkIfDebugOn();
//    checkIfTossModeOn();        

    char* salutation="v3.4 Orders receiver started";
    delay(2000);
    SerialUSB.println (" ");
    SerialUSB.println (salutation); 
    SerialUSB.print("My ID is: ");
    SerialUSB.println(DID);
    
    //Serial2.println (" ");
    //Serial2.println (salutation);
}

byte data=0;
void loop() 
{	   
  if (SerialUSB.available())
  {
    data=SerialUSB.read();
    process(data, InputUSB);
    Dxl.writeRaw(data);
  }
  
  if ( Dxl.available() )
  {
    data=Dxl.readRaw();
    process(data, InputDynamixel); 
    SerialUSB.write(data);
  }
}


