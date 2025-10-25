// #include <SPI.h>
#include <RF22.h>
#include <RF22Router.h>
#define MY_ADDRESS 5
#define DESTINATION_ADDRESS 1
#define SEND_TO 7
RF22Router rf22(MY_ADDRESS);

//variables for my glove
const int flexPin52 = A0;      // Pin connected to voltage divider output for index finger
const int flexPin25 = A1;      // Pin connected to voltage divider output for thumb finger
int data = 0;

//variables for comms
int my_Address = 5;
int send_to = 7;
int info = 0;
boolean successful_packet = false;

//variables for button
const int buttonPin = 8;
int button_state = 0;
int buttonState = 0;


void setup() {
  //setup pins
  pinMode(buttonPin, INPUT);
  pinMode(flexPin52, INPUT);
  pinMode(flexPin25, INPUT);

  //setup communication
  Serial.begin(9600);
  if (!rf22.init())
    Serial.println("RF22 init failed");
  // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
  if (!rf22.setFrequency(425.0))
    Serial.println("setFrequency Fail");
  rf22.setTxPower(RF22_TXPOW_20DBM);
  //1,2,5,8,11,14,17,20 DBM
  //rf22.setModemConfig(RF22::OOK_Rb40Bw335  );
  rf22.setModemConfig(RF22::FSK_Rb125Fd125);
  //modulation

  // we communicate only with the central node
  rf22.addRouteTo(DESTINATION_ADDRESS, DESTINATION_ADDRESS);
 
 
}

void loop() {
  //button 
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH)   //Every time we push button it switches between the different destrination addresses
  {
    if(button_state==0){
    button_state = 1;
    send_to = 8;
    }
    else{
      button_state = 0;
      send_to = 7;
    }
    
  }

  //flex sensors reading
  int ADCflexIn = analogRead(flexPin52);
  int ADCflexTh = analogRead(flexPin25);
  Serial.println("FLEX INDEX: "+ String(ADCflexIn) + "              "  +"FLEX THUMB: "+ String(ADCflexTh) );

  //flex sensors encoding
   if( (ADCflexIn > 450 && ADCflexIn < 500) && (ADCflexTh > 450 && ADCflexTh < 500))
  {
    data = 1;  //A
    Serial.println("STATUS = " + String(data) + " LETTER A");
  }
  else if((ADCflexIn > 550 && ADCflexIn < 600) && (ADCflexTh > 330 && ADCflexTh < 420))
  {
    data = 2;  //B
    Serial.println("STATUS = " + String(data) + " LETTER B");
  }
  else if((ADCflexIn > 500 && ADCflexIn < 550) && (ADCflexTh > 350 && ADCflexTh < 450))
  {
    data = 3;  //C
    Serial.println("STATUS = " + String(data) + " LETTER C");
  }
  else if((ADCflexIn > 600 && ADCflexIn < 650) && (ADCflexTh > 350 && ADCflexTh < 450))
  {
    data = 4;  //D
    Serial.println("STATUS = " + String(data) + " LETTER D");
  }
  else if(ADCflexIn < 100 &&  ADCflexTh < 100)
  {
    data = -3;  //BOTH FINGER ERROR
    Serial.println("STATUS = " + String(data) + " BOTH FINGER ERROR");
  }
  else if(ADCflexIn < 100)
  {
    data = -1;  //INDEX ERROR
    Serial.println("STATUS = " + String(data) + " INDEX ERROR");
  }
  else if(ADCflexTh < 100)
  {
    data = -2;  //THUMB ERROR
    Serial.println("STATUS = " + String(data) + " THUMB ERROR");
  }
   else
  {
    data = 0;  //IDLE
    Serial.println("STATUS = " + String(data) + " IDLE");
  }  


   //creating the packet that will be sent,including the message , my address and the dstination address as a 3 digit integer
   if(data<0)
   {
     info = data*100- my_Address*10 - send_to;
   }
   else
   {
    info = data*100 + my_Address*10 + send_to;
   }
   Serial.println("The packet to be send is: " + String(info));


  //transmit prosedure, we sent the info variable
  char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
  uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];
  memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  sprintf(data_read, "%d", info);
  data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
  memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN);

  successful_packet = false;
  while (!successful_packet)
  {

    if (rf22.sendtoWait(data_send, sizeof(data_send), DESTINATION_ADDRESS) != RF22_ROUTER_ERROR_NONE)
    {
      Serial.println("sendtoWait failed");
     // randNumber=random(200,max_delay);
      //Serial.println(randNumber);
      delay(100);
    }
    else
    {
      successful_packet = true;      
      Serial.println("Send packet to: " + String(send_to));
      Serial.println("sendtoWait Succesful");
      Serial.println("");
    }
  }

  delay(50);
}
