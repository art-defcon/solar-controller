#include <timer.h>

// Include Libraries
#include "Arduino.h"
#include "LDR.h"
#include "Button.h"

// Pin Definitions
#define LDR_1_PIN_SIG	A3
#define LDR_2_PIN_SIG	A1
#define MICROSWITCH_1_PIN_COM	2
#define MICROSWITCH_2_PIN_COM	3
#define RELAYMODULE4CH_PIN_IN1	4
#define RELAYMODULE4CH_PIN_IN2	5
#define RELAYMODULE4CH_PIN_IN3	6
#define RELAYMODULE4CH_PIN_IN4  7
#define LEDY_PIN_VIN  8
#define LEDB_PIN_VIN  9


// Global variables and defines
#define THRESHOLD_ldr_1   100
int ldr_1AverageLight;
#define THRESHOLD_ldr_2   100
int ldr_2AverageLight;
#define THRESHOLD_TURN   40

const double left_cal = 1;
const double right_cal = 1;

//define an array for the 4ch relay module pins
int RelayModule4chPins[] = { RELAYMODULE4CH_PIN_IN1, RELAYMODULE4CH_PIN_IN2, RELAYMODULE4CH_PIN_IN3, RELAYMODULE4CH_PIN_IN4 };
// object initialization
LDR ldr_1(LDR_1_PIN_SIG);
LDR ldr_2(LDR_2_PIN_SIG);
Button microSwitch_1(MICROSWITCH_1_PIN_COM);
Button microSwitch_2(MICROSWITCH_2_PIN_COM);


// define vars for testing menu
const int timeout = 20000;      
char menuOption = 0;
long time0;

bool leftSwitchActive;
bool rightSwitchActive;  

#define ACTIVE_TIME   30000
#define SLEEP_TIME   600000
auto startTimer = timer_create_default();
auto stopTimer = timer_create_default();

bool isAdjusting = true;


void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");

    // close all relay if restarted in operation 
    digitalWrite(RelayModule4chPins[0],LOW);
    digitalWrite(RelayModule4chPins[1],LOW);
    digitalWrite(RelayModule4chPins[2],LOW);
    digitalWrite(RelayModule4chPins[3],LOW);    
    
    ldr_1AverageLight = ldr_1.readAverage();
    ldr_2AverageLight = ldr_2.readAverage();
    microSwitch_1.init();
    microSwitch_2.init();
    pinMode(RELAYMODULE4CH_PIN_IN1, OUTPUT);
    pinMode(RELAYMODULE4CH_PIN_IN2, OUTPUT);
    pinMode(RELAYMODULE4CH_PIN_IN3, OUTPUT);
    pinMode(RELAYMODULE4CH_PIN_IN4, OUTPUT); 

    pinMode(LEDB_PIN_VIN, INPUT);
    pinMode(LEDB_PIN_VIN, INPUT);

    stopTimer.in(ACTIVE_TIME, toggleStop);
}

bool toggleStart(void *) {
  isAdjusting = true;
  Serial.println("### START Adjusting ###"); 
  stopTimer.in(ACTIVE_TIME, toggleStop);
  return true; // keep timer active? true
}

bool toggleStop(void *) {
  isAdjusting = false;
  Serial.println("### STOP Adjusting ###");
  startTimer.in(SLEEP_TIME, toggleStart
);
  return true; // keep timer active? true
}
void loop(){
      startTimer.tick();
      stopTimer.tick();
      adjust();
}

void adjust(){
      unsigned long runtime = millis();
      Serial.print(runtime/1000);
      Serial.print("s ");
  
      leftSwitchActive = !microSwitch_1.read();
      digitalWrite(LEDB_PIN_VIN, leftSwitchActive); // sets the digital pin 13 on
      Serial.print(leftSwitchActive);
      Serial.print("L/");
  
      rightSwitchActive = !microSwitch_2.read();    
      digitalWrite(LEDY_PIN_VIN, rightSwitchActive);  // sets the digital pin 13 off
      Serial.print(rightSwitchActive);
      Serial.print("R ");
  
      // Get current light reading, substract the ambient value to detect light changes
      //int leftSample = map(ldr_1.read(), 0, 1023, 0, 100);
      double rightSample = ldr_1.read();
      rightSample = ldr_1.read() * right_cal;
      
      double leftSample =  ldr_2.read();
      leftSample =  ldr_2.read() * left_cal;
     
      //int rightSample =  map(ldr_2.read(), 0, 1023, 0, 100);
      double ldr_1Diff = leftSample - rightSample; 
     
      Serial.print(F("Left: ")); 
      Serial.print(leftSample);
      Serial.print(" - ");    

      Serial.print(F("Right: ")); 
      Serial.print(rightSample);
      Serial.print(" - "); 
      
      Serial.print(F("Diff: "));
      Serial.print(ldr_1Diff);

      if(abs(ldr_1Diff) > THRESHOLD_TURN){
        Serial.print(" (turn"); 
        
        if (leftSample > rightSample){
          // more light from left
          Serial.print(" left");
          digitalWrite(RelayModule4chPins[0],LOW);
          if ( leftSwitchActive)
            digitalWrite(RelayModule4chPins[1],LOW); 
          else{
            if(isAdjusting)
              digitalWrite(RelayModule4chPins[1],HIGH);
          }
        }
        
        if (leftSample < rightSample) 
        {
          // more light from right
          Serial.print(" right"); 
          digitalWrite(RelayModule4chPins[1],LOW);
          
          if (rightSwitchActive)
            digitalWrite(RelayModule4chPins[0],LOW);
          else{
            if(isAdjusting)
              digitalWrite(RelayModule4chPins[0],HIGH);
          }
        }
        
        if(isAdjusting){
          Serial.println(") ");
        }
        else{
          digitalWrite(RelayModule4chPins[1],LOW);
          digitalWrite(RelayModule4chPins[0],LOW);
          Serial.println(" - sleeping)");
        }
          
      } else {
        // turn motor relay off
        digitalWrite(RelayModule4chPins[1],LOW);
        digitalWrite(RelayModule4chPins[0],LOW);

        if(isAdjusting)
          Serial.println(" nothing to do");
            else
          Serial.println(" sleeping");
      }   
}
 
