#ifndef MYSTEPPER_H_
#define MYSTEPPER_H_

class MyStepper
{
  public:
    MyStepper(int aStepPin, int aDirPin);
    void setReductionRatio(float gearRatio, int stepsPerRev);
    void setPositionRad(float rad);
    void setPosition(int value);
    void stepToPositionRad(float rad);
    void update(unsigned int flag);
  

  private:
    int stepperStepTargetPosition;
    int stepperStepPosition;

    int stepPin;
    int dirPin;
    int enablePin;

    float radToStepFactor;

    void motorSteps_1(void);
    void motorSteps_2(void);
    void motorSteps_3(void);
};




#endif
