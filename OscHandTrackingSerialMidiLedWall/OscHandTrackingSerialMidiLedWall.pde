
// midi used for button input
import themidibus.*; 

// osc used to recieve hand cursor position info
import oscP5.*;
import netP5.*;

// serial used to talk out via xbee
import processing.serial.*;

// track state of serial device connection
static boolean useSerial = false;


// state of previous hand
int cursorX = 200;
int cursorY = 200;
int cursorZ = 50;


// references
OscP5 oscP5;
MidiBus myBus; 
Serial myPort;
OPC opc;


int val;        // Data received from the serial port
PImage dot1, dot2;  

void setup() 
{
  size(600, 400);
  frameRate(30);
 
  // list and open serial connection
  for (int i=0; i< Serial.list().length; i++ ) {
    print(i + ":");
    println(Serial.list()[i]);
  }

  String portName = Serial.list()[0];
  try {
    myPort = new Serial(this, portName, 57600);
    useSerial = true;
  } 
  catch (Throwable t) {
    useSerial = false;
  }

  // open multicast
  oscP5 = new OscP5(this, "239.255.0.85", 10085);


  // open midi device
  MidiBus.list(); // List all available Midi devices on STDOUT. This will show each device's index and name.
  myBus = new MidiBus(this, 0, -1); 

  dot1 = loadImage("greenDot.png");
  dot2 = loadImage("purpleDot.png");

  // Connect to the local instance of fcserver. You can change this line to connect to another computer's fcserver
  opc = new OPC(this, "127.0.0.1", 7890);

  // Map an 8x8 grid of LEDs to the center of the window, scaled to take up most of the space
  float spacing = height / 20.0;

  // interface :: opc.ledGrid(index, 8, 8, x, y, spacing, spacing, angle, zigzag);
  opc.ledGrid(64, 7, 6, width*2/3, height/2, spacing, spacing, 0, true);
  opc.ledGrid(0, 7, 6, width/3, height/2, spacing, -1*spacing, PI, true);
}

float px = 0;
float py = 0;
float bz = 0;

void draw()
{
  background(0);
  blendMode(ADD);

  // Smooth out the mouse location
  px += (cursorX - px) * 0.05;
  py += (cursorY - py) * 0.1;
  bz += (cursorZ - bz) * 0.05;

  float a = millis() * 0.0005 * (bz/255.0);
  float r = py * 0.5;
  float dotSize = r * 4;  

  float dx = width/2 + cos(a) * r;
  float dy = height/2 + sin(a) * r;

  // Draw it centered at the mouse location
  image(dot1, dx - dotSize/2, dy - dotSize/2, dotSize, dotSize);

  // Another dot, mirrored around the center
  image(dot2, width - dx - dotSize/2, height - dy - dotSize/2, dotSize, dotSize);
}


// send position via serial
void sendCursor() {

  if (useSerial) {

    myPort.write('x');   
    myPort.write((byte)map(cursorX, 0, width, 0, 255));
    myPort.write(0x0D);             
    myPort.write(0x0A);             

    myPort.write('y');   
    myPort.write((byte)map(cursorY, 0, height, 0, 255));  
    myPort.write(0x0D);             
    myPort.write(0x0A);
  }

  return;
}

// recieve hand position
void oscEvent(OscMessage msg) {

  if (msg.addrPattern().contains("heart")) {
    return;
  }

  if (msg.addrPattern().startsWith("/cursor")) {
    float x = msg.get(0).floatValue();
    float y = msg.get(1).floatValue();
    float z = msg.get(2).floatValue();
    cursorX = (int) map (x, 0.7, -0.7, 0, width); 
    cursorY = (int) map (y, -0.5, 0.5, 0, height);
    cursorZ = (int) map (z, 0.2, 1.0, 0, 255);
    println( cursorX + "," + cursorY + "," + cursorZ + " : " + z);

    sendCursor();
  }
}

// emulate hand cursor via mouse presses and drags
void mouseDragged() {
  mousePressed();
}
  
void mousePressed() {
   cursorX = mouseX;
   cursorY = mouseY;
   return;
}

// use button to flip modes
int mode = 0;
int maxMode = 3;
int nextMode() {
  mode++;
  if (mode > maxMode) { 
    mode = 0;
  }
  return mode;
}

void noteOn(int channel, int pitch, int velocity) {

  // logging
  String msg = "" + nextMode() + "\n";
  print("mode: "+msg );


  if (useSerial) {

    myPort.write(msg);   
    myPort.write(0x0D);             
    myPort.write(0x0A);
  }

  return;
}