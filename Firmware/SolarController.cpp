#include "Controller.h"

SolarController::SolarController(){}

void SolarController::adjust()
{
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
