// Radio
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

//**************remote control****************
struct RECEIVE_DATA_STRUCTURE{
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

RECEIVE_DATA_STRUCTURE mydata_remote;

int RLR = 0;
int RFB = 0;
int RFBa = 0;

int wheel1;
int wheel1a;
int wheel2;
int wheel2a;

unsigned long currentMillis;
long previousMillis = 0;    // set up timers
long interval = 10;        // time constant for timer

unsigned long remoteMillis;   // safety timer
int remoteState;

void setup() {

    // initialize serial communication
    Serial.begin(115200);
    
    radio.begin();
    radio.setChannel (0x20);
    radio.openWritingPipe(addresses[0]); // 00002
    radio.openReadingPipe(1, addresses[1]); // 00001
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
  
    pinMode(3, OUTPUT);     // PWMs - wheels
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(9, OUTPUT);

    pinMode(4, OUTPUT);     //Digital - puncher
    
   
}   // end of setup

// ********************* MAIN LOOP *******************************

void loop() {  

      
        currentMillis = millis();
        if (currentMillis - previousMillis >= 10) {  // start timed event
          
            previousMillis = currentMillis;


            // check for radio data
            if (radio.available()) {
                    radio.read(&mydata_remote, sizeof(RECEIVE_DATA_STRUCTURE)); 
                    remoteMillis = currentMillis;  
            }             

            // is the remote disconnected for too long ?
            if (currentMillis - remoteMillis > 500) {
              remoteState = 0;
              mydata_remote.RFB = 512;
              mydata_remote.RLR = 512;
            }
            else {
              remoteState = 1;                            
            } 


            // threshold remote data
            // some are reversed based on stick wiring in remote
            RFB = thresholdStick(mydata_remote.RFB)/2.2;   
            RLR = thresholdStick(mydata_remote.RLR)/2.2; 

            wheel1 = RFB + RLR;
            wheel2 = (RFB*-1) + RLR;

            wheel1 = constrain(wheel1, -100,100);
            wheel2 = constrain(wheel2, -100,100);

            Serial.print(wheel1);
            Serial.print(" , ");
            Serial.println(wheel2);

            // *** wheel1 ***

            if (wheel1 > 0) {
              analogWrite(3, wheel1);
              analogWrite(5, 0);              
            }
            else if (wheel1 < 0) {
              wheel1a = abs(wheel1);
              analogWrite(5, wheel1a);
              analogWrite(3, 0); 
            }
            else {
              analogWrite(5, 0);
              analogWrite(3, 0);
            }

            // *** wheel2 ***

            if (wheel2 > 0) {
              analogWrite(6, wheel2);
              analogWrite(9, 0);              
            }
            else if (wheel2 < 0) {
              wheel2a = abs(wheel2);
              analogWrite(9, wheel2a);
              analogWrite(6, 0); 
            }
            else {
              analogWrite(6, 0);
              analogWrite(9, 0);
            }

            // *** puncher ***

            if (mydata_remote.toggle1 == 1) {
              digitalWrite (4, HIGH);
            }
            else {
              digitalWrite (4, LOW);
            }
          
        }     // end of timed loop         

   
}       // end  of main loop
