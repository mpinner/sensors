#include <EWMA.h>

#define POWER_PIN_0 5
#define SENSOR_PIN_0 21
#define ADJUSTMENT_PIN_0 20

#define POWER_PIN_1 6
#define SENSOR_PIN_1 19
#define ADJUSTMENT_PIN_1 18

#define LED_PIN 11

#define DEBUG false
#define DEBUG_DELAY 50


int power[] = {
  POWER_PIN_0, POWER_PIN_1};
EWMA* reading[] = {
  new EWMA(), new EWMA()};
int adjustment[] = {
  350, 500};
boolean powerState[] = {
  false, false};


EWMA ewma;



void setup() {

  digitalWrite(POWER_PIN_0, LOW);
  digitalWrite(POWER_PIN_1, LOW);

  pinMode(POWER_PIN_0, OUTPUT);
  pinMode(POWER_PIN_1, OUTPUT);

 for (int i = 0; i < 2; i++) {
  reading[i]->init(ewma.periods(1));  // 0.693147 / 40.0);
 }

  if (DEBUG)
  {
    Serial.begin(9600);
    Serial.println("hello");

  }

  return;
}



void loop() {

  makeReadings();

  setPowerPins();



  if (DEBUG)
  {

    for (int i = 0; i < 2; i++) {
      Serial.print("i:");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(reading[i]->value());
      Serial.print(" , ");
      Serial.print(powerState[i]);
      //Serial.print(" , ");
      //Serial.print(power[i]);
      Serial.println();

    }
    Serial.println();



    delay(DEBUG_DELAY);
  } // end if DEBUG

  delay(0);


  return;  
}

void makeReadings() {

  reading[0]->record(analogRead(SENSOR_PIN_0));
  reading[1]->record(analogRead(SENSOR_PIN_1));

  return;
}

void setPowerPins() {

  digitalWrite(LED_PIN, LOW);

  for (int i = 0; i < 2; i++) {
    if ( reading[i]->value() > adjustment[0]) {
      powerState[i] = true;
      digitalWrite(LED_PIN, HIGH);

    } 
    else {
      powerState[i] = false;
    }

    digitalWrite(power[i], powerState[i]);
  }


  return;
}



