
import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;


import com.heroicrobot.dropbit.registry.*;
import com.heroicrobot.dropbit.devices.pixelpusher.Pixel;
import com.heroicrobot.dropbit.devices.pixelpusher.Strip;
import java.util.*;

DeviceRegistry registry;

long[] drumHitMs = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int defaultHue = 281;
int[] drumHue = {
  defaultHue, defaultHue, defaultHue, defaultHue, 
  defaultHue,defaultHue, defaultHue, defaultHue
};

int mode = 0;
int maxMode = 1;


long drumHitDurationMs = 125;

int[][] colors = {
  {127, 0, 0}, 
  {0, 127, 0}, 
  {0, 0, 127}
};


class TestObserver implements Observer {
  public boolean hasStrips = false;
  public void update(Observable registry, Object updatedDevice) {
    println("Registry changed!");
    if (updatedDevice != null) {
      println("Device change: " + updatedDevice);
    }
    this.hasStrips = true;
  }
}

TestObserver testObserver;
int c = 0;

void setup() {
  registry = new DeviceRegistry();
  testObserver = new TestObserver();
  registry.addObserver(testObserver);
  colorMode(HSB, 360);
  size(480, 64);
  frameRate(60);

  oscP5 = new OscP5(this, 12000);
}

void draw() {
  
  
  for (int i = 0; i < 8; i++ ) {
    fill(getDrumColor(i));
    rect(0, i * (height/8), width/2, (i+1) * (height/8));
  }
  
  int x=0;
  int y=0;
  
  
  if (testObserver.hasStrips) {
    registry.setFrameLimit(1000);   
    registry.startPushing();
    registry.setExtraDelay(0);
    registry.setAutoThrottle(false);    
    int stripy = 0;
    List<Strip> strips = registry.getStrips();
    
    
    if (++c > 99)
      c = 0;
    int numStrips = strips.size();
    //println("Strips total = "+numStrips);
    if (numStrips == 0)
      return;
      
    for (int stripNo = 0; stripNo < numStrips; stripNo++) {
    fill(getDrumColor(stripNo));
    rect(width/2, stripNo * (height/8), width, (stripNo+1) * (height/8));
    }    


    int yscale = height / strips.size();
    for (int i = 0; i < numStrips; i++ ) {
      Strip strip = strips.get(i);
      color c = getDrumColor(i);

      int xscale = width / strip.getLength();
      for (int stripx = 0; stripx < strip.getLength(); stripx++) {
        x = stripx*xscale + 1;
        y = stripy*yscale + 1; 


if (mode == 0) {
        strip.setPixel(c, stripx);
        
} else if (mode == 1) {
        strip.setPixel(c, getSpiralColor(stripx, i));
  
}

    
  }
      
      stripy++;
    }
  }
  
  
  
  
 
}

color getDrumColor(int drum) {
  long onDuration = millis() - drumHitMs[drum];
  if (onDuration < drumHitDurationMs) {
    float percentRemaining = 1.0 - ((float)onDuration / (float)drumHitDurationMs);
    return color(getHue(drum), 360, 360*percentRemaining);
  } else {
    return color(0, 0, 0);
  }
}

color getSpiralColor(int drum, int pixel) {
  
  long onDuration = millis() - drumHitMs[drum];
  if (onDuration < drumHitDurationMs) {
    float percentRemaining = 1.0 - ((float)onDuration / (float)drumHitDurationMs);
  
    float currentIndex = (50*percentRemaining);
    
    if (abs (pixel - currentIndex) < 10) {
         return color(getHue(drum), 360, 360*percentRemaining);
    }
  
  }

    return color(0, 0, 0);

}

int getHue(int drum) {
  return drumHue[drum];
}


void setHue(int hue, int drum) {
  drumHue[drum] = hue;
  return;
}


void setHue(int hue) {
  for (int i = 0; i < 8; i++ ) {
    setHue(hue, i);
  }
  return;
}


void hit(int drumNum) {
  drumHitMs[drumNum] = millis();
  return;
}





void oscEvent(OscMessage theOscMessage) {
  /* check if theOscMessage has the address pattern we are looking for. */

  if (theOscMessage.checkAddrPattern("/drum")==true) {
    if (theOscMessage.checkTypetag("i")) {
      int drumNum = theOscMessage.get(0).intValue();  

      hit(drumNum);
      println(" drum-hit: "+drumNum);
      return;
    }
  } 

  if (theOscMessage.checkAddrPattern("/hue")==true) {
    if (theOscMessage.checkTypetag("i")) {
      int hue = theOscMessage.get(0).intValue();  
      
      setHue(hue);
      println(" hue: "+hue);
      return;
    } 
    
    if (theOscMessage.checkTypetag("ii")) {
      int hue = theOscMessage.get(0).intValue();  
      int drumNum = theOscMessage.get(1).intValue();  

      setHue(hue, drumNum);
      println(" drum: "+drumNum+ " hue:" + hue);
      return;
    } 
    
     
    
  } 


  if (theOscMessage.checkAddrPattern("/duration")==true) {
    if (theOscMessage.checkTypetag("i")) {
      drumHitDurationMs = theOscMessage.get(0).intValue();  
      println(" drumHitDurationMs: "+drumHitDurationMs);
      return;
    }
  } 


  if (theOscMessage.checkAddrPattern("/mode")==true) {
    if (theOscMessage.checkTypetag("i")) {
      mode = theOscMessage.get(1).intValue();  
      if (mode > maxMode) mode = 0;
      println(" mode: "+mode);
      return;
    }
  } 

  println("### received an unused osc message with address pattern "+theOscMessage.addrPattern());
}