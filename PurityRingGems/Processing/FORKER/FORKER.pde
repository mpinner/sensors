/**
 * oscP5sendreceive by andreas schlegel
 * example shows how to send and receive osc messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */

import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress submergency;
NetAddress gems;

HashMap lastHits = new HashMap<String, Integer>();

String gemAddresses[] = {
  "gem", "behavior", "color", "neighbor"
};


int bounce = 50;

void setup() {


  size(400, 400);
  // frameRate(25);
  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this, 7777);

  /* myRemoteLocation is a NetAddress. a NetAddress takes 2 parameters,
   * an ip address and a port number. myRemoteLocation is used as parameter in
   * oscP5.send() when sending osc packets to another computer, device, 
   * application. usage see below. for testing purposes the listening port
   * and the port of the remote location address are the same, hence you will
   * send messages back to this sketch.
   */
  gems = new NetAddress("127.0.0.1", 8000);
  submergency = new NetAddress("127.0.0.1", 12345);
}


void draw() {
  background(0);
}


/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {

  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");

  String pattern = theOscMessage.addrPattern();
  println(" addrpattern: "+theOscMessage.addrPattern());

  for (int i = 0; i < gemAddresses.length; i++) {

    if (pattern.contains(gemAddresses[i])) {

      int hit = getLastHit(pattern);
      if (hit + bounce < millis()) {

        oscP5.send(theOscMessage, gems);
        lastHits.put(pattern, millis());
        println(" last hit: "+hit);
      }
    }
  } 

  oscP5.send(theOscMessage, submergency);
}


int getLastHit(String pattern) {
  if (null == lastHits.get(pattern)) {
    return 0;
  }

  return (Integer) lastHits.get(pattern);
}

