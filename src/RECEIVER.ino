#include <SPI.h>
#include <RF22.h>
#include <RF22Router.h>
#include <LiquidCrystal.h> 

//variables for lcd
int Contrast=75;
 LiquidCrystal lcd(9, 8, 5, 4, 3, 7); 

//comminucation 
#define MY_ADDRESS 7
#define NODE_ADDRESS_1 10
//#define NODE_ADDRESS_2 13
//#define NODE_ADDRESS_3 14
RF22Router rf22(MY_ADDRESS);

//Variables:
int data=-10; //save analog value
int info=-10;
int node=-10;

void setup() {
  //setup for lcd
  analogWrite(6,Contrast);
  lcd.begin(16, 2);
  
  //setup for aloha
   Serial.begin(9600);
  if (!rf22.init())
    Serial.println("RF22 init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
  if (!rf22.setFrequency(425.0))
    Serial.println("setFrequency Fail");
  rf22.setTxPower(RF22_TXPOW_20DBM);
  //1,2,5,8,11,14,17,20 DBM
  rf22.setModemConfig(RF22::FSK_Rb125Fd125);
  //modulation

  // We communicate only with the base station
  rf22.addRouteTo(NODE_ADDRESS_1, NODE_ADDRESS_1);   
}

void loop() {
  // procedure to receive
   uint8_t buf[RF22_ROUTER_MAX_MESSAGE_LEN];
  char incoming[RF22_ROUTER_MAX_MESSAGE_LEN];
  memset(buf, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(incoming, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  uint8_t len = sizeof(buf);
  uint8_t from;
  int received_value = 0; 

  //when we receive
  if (rf22.recvfromAck(buf, &len, &from))
  {
    buf[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
    memcpy(incoming, buf, RF22_ROUTER_MAX_MESSAGE_LEN);
    Serial.print("got request from : ");
    Serial.println(from, DEC);
    received_value = atoi((char*)incoming);
    Serial.println(received_value);
    info=received_value;

    data=info/10;       //Delivered message
    node=abs(info%10);  //Received from 
  
  }

  //print on screen the tx node id
   lcd.setCursor(0, 0);
   lcd.println("MESSAGE FROM: " + String(node)+"  ");
  
   if(data == 1)      //decode  data and print it on the screen
   {
    lcd.setCursor(0, 1);
    lcd.println("A                               ");
   }
   else if(data == 2)
   {
    lcd.setCursor(0, 1);
    lcd.println("B                               ");
   }
   else if(data == 3)
   {
    lcd.setCursor(0, 1);
    lcd.println("C                               ");
   }
   else if(data == 4)
   {
    lcd.setCursor(0, 1);
    lcd.println("D                               ");
   }
   else if(data == 0)
   {
    lcd.setCursor(0, 1);
    lcd.println("IDLE                     ");
   }
   else if(data == -1)
   {
    lcd.setCursor(0, 1);
    lcd.println("INDEX FINGER ERROR");
   }
   else if(data == -2)
   {
    lcd.setCursor(0, 1);
    lcd.println("THUMB FINGER ERROR");
   }
   else if(data == -3)
   {
    lcd.setCursor(0, 1);
    lcd.println("BOTH FINGER ERROR");
   }
   else
   {
    lcd.setCursor(0, 1);
    lcd.println("NO LETTER DETECTED");
   }


  
    delay(10);                          //Small delay
  }
