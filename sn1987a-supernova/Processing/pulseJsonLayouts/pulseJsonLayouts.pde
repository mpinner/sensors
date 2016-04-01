


int rings = 4;

int ringRows[] = {
  48, 20, 20, 20 
};

int ringRowSpacing[] = {
  4, 10, 10, 10 
};



// count of balls per ring
int count[] = {
  16, 7, 13, 21
};


// postitions where balls can exist
int spacing[] = {
  20, 40, 40, 40
};


int ballSize = 4; 

color[][][] pixels = new color[count[0]][count[1]][count[2]];



boolean fileSave = false;

// starts here
void setup() {

  size(1000, 400, P3D);
  colorMode(HSB);
  background(0);
  strokeWeight(0);
  translateToCenter();
  lights();
}

void translateToCenter() {
  translate(width/2, height/2, 0);
  lights();
}

void mouseRotate() {
  rotateX(radians(-1*(mouseY-height/2)/2));
  rotateY(radians(-1*(mouseX-width/2)/4));
}


void draw() {
  background(0);

  translateToCenter();

  if (mousePressed) {

    //    ortho(0, width/2, 0, height, -1000, 1000);
    ortho(-width/2, width/2, -height/2, height/2);
  } else {  
    float fov = PI/3.0;
    float cameraZ = (height/2.0) / tan(fov/2.0);
    perspective(fov, float(width)/float(height), 
      cameraZ/2.0, cameraZ*2.0);
  }



  mouseRotate();



  pushMatrix();
  drawRadi();
  popMatrix();
}



void drawRadi() {

  PrintWriter output = null;

  // open new file
  if (shouldSaveFile()) {
    String jsonFileName = "layout-" +count[0]+"x"+count[1]+"x"+count[2]+".json";

    output = createWriter(jsonFileName);
  }

  if (debug()) print("[\n");
  if (shouldSaveFile()) output.print("[\n");

  // how big is our current ever growing ring    
  int ringRadius = 0;
  for (int ring = 0; ring < rings; ring++) {

    ringRadius += spacing[ring];


    // number of leds in each ring
    for (int radius = 0; radius < count[ring]; radius++) {

      // create multiple leds per strip hanging down base on led density
      for (int row = 0; row < ringRows[ring]; row++ ) {

        // make it round
        float slice = TWO_PI / count[ring];
        int x =  (int) (ringRadius * sin(slice*radius));
        int y =  (int) (ringRadius * cos(slice*radius));

        pushMatrix();

        //move to led position and paint it
        translate(0, -1*ringRowSpacing[ring]*ringRows[ring]/2, 0);
        translate(x, ringRowSpacing[ring]*row, y);
        sphere(ballSize);

        popMatrix();

        // write json layout for this led


        float i = x;
        float j = -1*ringRowSpacing[ring]*ringRows[ring]/2 + ringRowSpacing[ring]*row;
        float k = y;

        float scale = 0.02;
        i *= scale;
        j *= scale;
        k *= scale;


        if ((ring != 0) || (row != 0) || (radius != 0) ) {
          if (debug()) print(",\n");
          if (shouldSaveFile()) output.print(",\n");
        }

        if (debug()) print("  {\"point\": [" + i + ", " + j+ ", " + k + "]}");
        if (shouldSaveFile())  output.print("  {\"point\": [" + i + ", " + j+ ", " + k + "]}");
      }
    }
  }


  // close file
  if (debug()) println("\n]");


  if (shouldSaveFile()) {
    output.println("\n]");

    output.flush(); // Write the remaining data
    output.close(); // Finish the file

    fileSave = true;
  }
  return;
}


void drawNextRadi() {

  for (int row = 0; row < 20; row++ ) {

    for (int radius = 0; radius < count[0]; radius++) {

      float slice = TWO_PI / count[0];

      int x =  (int) (spacing[0] * sin(slice*radius));
      int y =  (int) (spacing[0] * cos(slice*radius));

      pushMatrix();

      translate(0, -1*spacing[0]*5, 0);
      translate(x, 10*row, y);

      sphere(ballSize);
      popMatrix();
    }
  }


  return;
}



boolean shouldSaveFile() {

  return false == fileSave;
}

boolean debug() {
  return true;
}