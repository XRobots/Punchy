#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

unsigned long previousMillis = 0;
const long interval = 20;

int but1;
int but2;
int but3;
int but4;

int sw1;
int sw2;
int sw3;
int sw4;
int sw5;

int axis1;
int axis2;
int axis3;
int axis4;
int axis5;
int axis6;

String count;

struct SEND_DATA_STRUCTURE{
//struct __attribute__((__packed__)) SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO

    int16_t menuDown;  
    int16_t Select;    
    int16_t menuUp;  
    int16_t toggleBottom;  
    int16_t toggleTop; 
    int16_t toggle1;
    int16_t toggle2;
    int16_t mode;  
    int16_t RLR;
    int16_t RFB;
    int16_t RT;
    int16_t LLR;
    int16_t LFB;
    int16_t LT;

};

struct RECEIVE_DATA_STRUCTURE_REMOTE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int16_t mode;
  int16_t count;
};

int remoteFlag = 0;

SEND_DATA_STRUCTURE mydata_send;
RECEIVE_DATA_STRUCTURE_REMOTE mydata_remote;

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

void setup() {  

  pinMode(2, INPUT_PULLUP); 
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  
  radio.begin();
  radio.setChannel (0x20);
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_MIN);

  radio.stopListening();

  Serial.begin(115200);
 
    
}

void loop() {

     unsigned long currentMillis = millis();
         if (remoteFlag == 0 && currentMillis - previousMillis >= 10) { 

              previousMillis = currentMillis;  

              sw3 =  digitalRead(2);
                           
              if (but1 == 0) {
                mydata_send.menuDown = 1;
              }
              else {
                mydata_send.menuDown = 0;
              }

              if (but4 == 0) {
                mydata_send.Select = 1;
              }
              else {
                mydata_send.Select = 0;
              }
                            
              if (but2 == 0) {
                mydata_send.menuUp = 1;
              }
              else {
                mydata_send.menuUp = 0;
              }

              
              
              if (sw2 == 0) {
                mydata_send.toggleBottom = 1;
              }
              else {
                mydata_send.toggleBottom = 0;
              }
              
              if (sw1 == 0) {
                mydata_send.toggleTop = 1;
              }
              else {
                mydata_send.toggleTop = 0;
              }

              //******************

              if (sw3 == 0) {
                mydata_send.toggle1 = 1;
              }
              else {
                mydata_send.toggle1 = 0;
              }
              
              if (but3 == 0) {
                mydata_send.toggle2 = 1;
              }
              else {
                mydata_send.toggle2 = 0;
              } 
                       
              axis1 = analogRead(A1);
              axis2 = analogRead(A0);

              axis1 = map(axis1,0,1023,1023,0);

              // reverse walking & reverse all controls (twist is unaffected because CW and CCW and still the same no matter which way around it is)
        
             
              mydata_send.RFB = axis1;
              mydata_send.RLR = axis2;
              mydata_send.RT = axis3;
              mydata_send.LFB = axis4;
              mydata_send.LLR = axis5;
              mydata_send.LT = axis6;              

              radio.write(&mydata_send, sizeof(SEND_DATA_STRUCTURE));               
              
         }        
        


      



  }  // end of main loop

