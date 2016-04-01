import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 
import oscP5.*; 
import netP5.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class OscRsSerialWrite_WIN extends PApplet {

/**
 * derived from:
 * Simple Write example. 
 * oscP5multicast by andreas schlegel
 */







OscP5 oscP5;

int cursorX = 0;
int cursorY = 0;

PImage dot;


Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port

public void setup() 
{
  size(200, 200);

//load image
  dot = loadImage("images.jpeg");


// list and open serial connection
  for (int i=0; i< Serial.list().length; i++ ) {
    print(i + ":");
    println(Serial.list()[i]);
  }

  String portName = Serial.list()[0];
 myPort = new Serial(this, portName, 57600);

// open multicast
  oscP5 = new OscP5(this, "239.255.0.85", 10085);
}

public void draw() {
  background(255);

  float dotSize = height * 1.5f;
  image(dot, cursorX - dotSize/2, cursorY - dotSize/2, dotSize, dotSize);


sendCursor();

  delay(5);
}

public void sendCursor() {
  
  myPort.write('x');   
  myPort.write((byte)map(cursorX, 0, width, 0, 255));
  myPort.write(0x0D);             
  myPort.write(0x0A);             

  myPort.write('y');   
  myPort.write((byte)map(cursorY, 0, height, 0, 255));  
  myPort.write(0x0D);             
  myPort.write(0x0A);             
  

  return;
}
public boolean mouseOverRect() { // Test if mouse is over square
  return ((mouseX >= 50) && (mouseX <= 150) && (mouseY >= 50) && (mouseY <= 150));
}


public void oscEvent(OscMessage msg) {

  if (msg.addrPattern().contains("heart")) {
    return;
  }

  if (msg.addrPattern().startsWith("/cursor")) {
    float x = msg.get(0).floatValue();
    float y = msg.get(1).floatValue();
    cursorX = (int) map (x, -0.3f, 0.3f, 0, width); 
    cursorY = (int) map (y, -0.2f, 0.3f, 0, height);
    println( cursorX + "," + cursorY);
}
  


}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "OscRsSerialWrite_WIN" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
