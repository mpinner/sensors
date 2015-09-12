#define POWER_PIN_0 5
#define SENSOR_PIN_0 21
#define ADJUSTMENT_PIN_0 20

#define POWER_PIN_1 6
#define SENSOR_PIN_1 19
#define ADJUSTMENT_PIN_1 18

#define LED_PIN 11

#define DEBUG true

int power[] = {
  POWER_PIN_0, POWER_PIN_1};
int reading[] = {
  0, 0};
int adjustment[] = {
  0, 0};
boolean powerState[] = {
  false, false};


void setup() {

  digitalWrite(POWER_PIN_0, LOW);
  digitalWrite(POWER_PIN_1, LOW);

  pinMode(POWER_PIN_0, OUTPUT);
  pinMode(POWER_PIN_1, OUTPUT);


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


  delay(10);

  if (DEBUG)
  {

    for (int i = 0; i < 2; i++) {
      Serial.print("i:");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(reading[i]);
      Serial.print(" , ");
      Serial.print(powerState[i]);
      //Serial.print(" , ");
      //Serial.print(power[i]);
      Serial.println();

    }
          Serial.println();



    delay(250);
  }


  return;  
}

void makeReadings() {

  reading[0] = analogRead(SENSOR_PIN_0);
  reading[1] = analogRead(SENSOR_PIN_1);

  return;
}

void setPowerPins() {
  
  digitalWrite(LED_PIN, LOW);

  for (int i = 0; i < 2; i++) {
    if ( reading[i] > 512) {
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


