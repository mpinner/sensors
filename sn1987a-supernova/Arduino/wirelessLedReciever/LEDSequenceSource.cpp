
#include "LEDSequence.h"


// constructor
LEDSequence::LEDSequence(LEDController* ledsIn,
int stepIn[],
int stepCountIn)
{
  setAttributes(ledsIn, stepIn, stepCountIn);
  return;
}

//Strip attribute setter
void LEDSequence::setAttributes(LEDController* ledsIn,
int stepIn[],
int stepCountIn
)
{
  leds = ledsIn;
  step = stepIn;
  stepCount = stepCountIn;
  return;
}


void LEDSequence::takeAction(){


    if (isExpired()) {
        incrAction();
    }
    
  leds->takeAction();

}

void LEDSequence::setLEDBrightness(int ledValue){
  leds->setLEDBrightness(ledValue);
}


void LEDSequence::incrAction() {
    stepStartTime = millis();
    stepCounter++;
    stepCounter %= stepCount;
    stepEndTime = stepStartTime + getCurrentDuration();
    initStepMetaData();
    leds->action = getCurrentAction();
    return;
}

boolean LEDSequence::isExpired() {
    return millis() > stepEndTime;
}

long LEDSequence::getCurrentDuration () {
    return step[stepCounter*stepValues];
}

int LEDSequence::getCurrentAction () {
    int action = step[stepCounter*stepValues+1];
    return action ;
}

void LEDSequence::initStepMetaData() {
  
    int metadata = step[stepCounter*stepValues+2];
    
    if(metadata == 0) return;
    
    int duration = getCurrentDuration();
    leds->blinkTimeHigh = duration /2/metadata;
    leds->blinkTimeLow = duration /2/metadata;
    int rampDelays = duration / leds->analogHighValue / metadata;
    leds->rampDownDelay = rampDelays;
    leds->rampUpDelay = rampDelays;
    return;
    
}


