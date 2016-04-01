/**
 * derived from:
 * Simple Write example. 
 * oscP5multicast by andreas schlegel
 */


import processing.serial.*;

import oscP5.*;
import netP5.*;

OscP5 oscP5;

int cursorX = 0;
int cursorY = 0;

PImage dot;


Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port

void setup() 
{
  size(200, 200);

//load image
  dot = loadImage("images.jpeg");


// list and open serial connection
  for (int i=0; i< Serial.list().length; i++ ) {
    print(i + ":");
    println(Serial.list()[i]);
  }

  String portName = Serial.list()[9];
  myPort = new Serial(this, portName, 57600);

// open multicast
  oscP5 = new OscP5(this, "239.255.0.85", 10085);
}

void draw() {
  background(255);

  float dotSize = height * 1.5;
  image(dot, cursorX - dotSize/2, cursorY - dotSize/2, dotSize, dotSize);


sendCursor();

  delay(50);
}

void sendCursor() {
  
  myPort.write('x');   
  myPort.write((byte)map(cursorX, 0, width, 0, 255));
  myPort.write('\n');             

  myPort.write('y');   
  myPort.write((byte)map(cursorY, 0, height, 0, 255));  
  myPort.write('\n');    
  

  return;
}
boolean mouseOverRect() { // Test if mouse is over square
  return ((mouseX >= 50) && (mouseX <= 150) && (mouseY >= 50) && (mouseY <= 150));
}


void oscEvent(OscMessage msg) {


  if (msg.addrPattern().startsWith("/cursor")) {
    float x = msg.get(0).floatValue();
    float y = msg.get(1).floatValue();
    cursorX = (int) map (x, -0.3, 0.3, 0, width); 
    cursorY = (int) map (y, -0.3, 0.3, 0, height);
    println( cursorX + "," + cursorY);
}
  


}