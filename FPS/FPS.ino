
#define FLEX 5
#define FPS 4
#define POT 3
#define SLIDE 2


#define SENSOR SLIDE


void setup() {
  Serial.begin(9600);     
  return; 
}

void loop() {
  
  
  Serial.print("a1=");
  Serial.print(analogRead(FLEX));
  Serial.print(" a2=");
  Serial.print(analogRead(FPS));
  Serial.print(" a3=");
  Serial.print(analogRead(POT));
  Serial.print(" a4=");
  Serial.print(analogRead(SLIDE));
  Serial.print("\r\n");
  delay(10);
  return; 
}
