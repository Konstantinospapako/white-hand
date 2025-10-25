#include <SPI.h>
#include <RF22.h>
#include <RF22Router.h>

#define MY_ADDRESS 1
#define RECEIVE_ADDRESS 5
#define DESTINATION_ADDRESS_1 7
#define DESTINATION_ADDRESS_2 8

RF22Router rf22(MY_ADDRESS);

//Variables for the leds
const int ledPinW = 4;   //pin 4 for white
const int ledPinB = 5;   //pin 5 for blue
const int ledPinR = 6;   //pin 6 for red
const int ledOn=200;
const int ledOff=0;


//Variables initialization
int data=-10; 
boolean successful_packet = false;
int node_mode=0;
int info=-10;
int send_to = 0;
int fail_counter=0;
int data_leds = 0;

void setup() {
  //setup comms
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

  // This is the base station and communicates with all the others
  rf22.addRouteTo(RECEIVE_ADDRESS,  RECEIVE_ADDRESS);
  rf22.addRouteTo(DESTINATION_ADDRESS_1, DESTINATION_ADDRESS_1);
  rf22.addRouteTo(DESTINATION_ADDRESS_2, DESTINATION_ADDRESS_2);

  //setup for the leds
  pinMode(ledPinW, OUTPUT);  //Set pin 4 as 'output'
  pinMode(ledPinB, OUTPUT);  //Set pin 5 as 'output'
  pinMode(ledPinR, OUTPUT);  //Set pin 6 as 'output'
  Serial.begin(9600);       //Begin serial communication

  analogWrite(ledPinW, ledOff);          
  analogWrite(ledPinR, ledOff);          
  analogWrite(ledPinB, ledOff);
  
}

void loop() {
  /***TRANSCEIVER MODE***/
  if(node_mode==0){
    Serial.println("RECEIVE MODE");         //At this point transeiver is on receiver mode and it's waiting for data
    node_mode=1;
  }
  uint8_t buf[RF22_ROUTER_MAX_MESSAGE_LEN];
  char incoming[RF22_ROUTER_MAX_MESSAGE_LEN];
  memset(buf, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(incoming, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  uint8_t len = sizeof(buf);
  uint8_t from;
  int received_value = 100; 
 
  if (rf22.recvfromAck(buf, &len, &from))
  {
    buf[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
    memcpy(incoming, buf, RF22_ROUTER_MAX_MESSAGE_LEN);
    Serial.print("Got request from : ");
    Serial.println(from, DEC);
    received_value = atoi((char*)incoming);    
    Serial.println("The received value is: " + String(received_value));
    
    info=received_value;        // Info contains the received value
    
    data = info/10;             // Data contains the message to be send to other nodes
    Serial.println("Message to be send " +  String(data) );
    
    send_to = abs(info%10);     // Send_to contains the address of the node that the message is to send.
    Serial.println("Send message to node no. " + String(send_to));
    
    data_leds = info/100;
   if(data_leds>0)          // When i get a letter, blue led turns on
   {                             
    analogWrite(ledPinW, ledOff);          
    analogWrite(ledPinR, ledOff);          
    analogWrite(ledPinB, ledOn); 
    
   }
    else if(data_leds==0)   // When i am at idle state, white led turns on
    {                    
      analogWrite(ledPinW, ledOn);          
      analogWrite(ledPinR, ledOff);          
      analogWrite(ledPinB, ledOff); 
      
    }
    else if(data_leds<0 && data_leds>-4)  // When i have a connection eroor, red led turns on
    {                     
      analogWrite(ledPinW, ledOff);          
      analogWrite(ledPinR, ledOn);          
      analogWrite(ledPinB, ledOff);         
      }
    } 

  if(info == received_value)        //If a message is properly received, the transceiver enters the trasmitter mode
  {
    node_mode=0;
    successful_packet = false;

    Serial.println("TRANSMITTER MODE");
    char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
    uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];
    memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
    memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
    sprintf(data_read, "%d", data);
    data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
    memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN);
  
    
    while (!successful_packet)      // Try to send the received message a maximum of 20 time
  {

    if (rf22.sendtoWait(data_send, sizeof(data_send), send_to) != RF22_ROUTER_ERROR_NONE)
    {
      Serial.println("sendtoWait failed");

      fail_counter=fail_counter+1;

      if (fail_counter==2){
        fail_counter=0;
        break;
        
        }

      delay(30);
    }
    else
    {
      successful_packet = true;
      Serial.println("sendtoWait Succesful");
      Serial.println("");
    }
  }
  }
  
  delay(100);
 
}
