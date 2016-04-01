
int speed = 57600;

void setup() {
  // initialize both serial ports:
  Serial.begin(speed);
  Serial1.begin(speed);
  Serial2.begin(speed);
  Serial3.begin(speed);
}

void loop() {
  // read from port 1, send to port 0:

    Serial.println("hi0");
    Serial1.println("hi1");
    Serial2.println("hi2");
    Serial3.println("hi3");

  delay(1000);
}
