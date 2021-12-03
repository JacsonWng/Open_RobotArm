#include "mystepper.h"
#include <Arduino.h>

MyStepper::MyStepper(int aStepPin, int aDirPin)
{
  stepPin = aStepPin;
  dirPin = aDirPin;
  stepperStepPosition = 0;
  stepperStepTargetPosition = 0;

  //引脚设置
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
}

void MyStepper::setPositionRad(float rad)
{
  setPosition(rad * radToStepFactor);
}

void MyStepper::setPosition(int value)
{
  stepperStepPosition = value;
  stepperStepTargetPosition = value;
}

void MyStepper::stepToPositionRad(float rad) {
  stepperStepTargetPosition = rad * radToStepFactor;
}


void MyStepper::motorSteps_1(void)
{
  //根据旋转方向分一下两种情况
  while (stepperStepTargetPosition < stepperStepPosition)
  {
    digitalWrite(dirPin, LOW);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
    stepperStepPosition--;
  }

  while (stepperStepTargetPosition > stepperStepPosition)
  {
    digitalWrite(dirPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
    stepperStepPosition++;
  }
  stepperStepPosition = stepperStepTargetPosition;
}

void MyStepper::motorSteps_2(void)
{
  //根据旋转方向分一下两种情况
  while (stepperStepTargetPosition < stepperStepPosition)
  {
    digitalWrite(dirPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
    stepperStepPosition--;
  }

  while (stepperStepTargetPosition > stepperStepPosition)
  {
    digitalWrite(dirPin, LOW);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
    stepperStepPosition++;
  }
  stepperStepPosition = stepperStepTargetPosition;
}

void MyStepper::motorSteps_3(void)
{
  //根据旋转方向分一下两种情况
  while (stepperStepTargetPosition < stepperStepPosition)
  {
    digitalWrite(dirPin, LOW);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
    stepperStepPosition--;
  }

  while (stepperStepTargetPosition > stepperStepPosition)
  {
    digitalWrite(dirPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(100);
    stepperStepPosition++;
  }
  stepperStepPosition = stepperStepTargetPosition;
}



void MyStepper::update(unsigned int flag)
{
  //Serial.print("stepper:");ss
  //Serial.print(stepperStepPosition);
  //Serial.print("\n")
  if(flag == 1){
    motorSteps_1();
  }
  else if(flag == 2){
    motorSteps_2();
  }
  else if(flag == 3){
    motorSteps_3();
  }

}

void MyStepper::setReductionRatio(float gearRatio, int stepsPerRev)
{
  radToStepFactor = (gearRatio * stepsPerRev) / (2 * PI);
};
