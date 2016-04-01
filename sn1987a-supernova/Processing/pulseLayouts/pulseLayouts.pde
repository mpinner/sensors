


int rings = 4;

int ringRows[] = {
 48, 20, 20, 20 
};

int ringRowSpacing[] = {
 4, 20, 20, 20 
};



// count of balls per ring
int count[] = {
  16, 7, 13, 21
};





// postitions where balls can exist
int spacing[] = {
  20, 16, 14, 10
};


int ballSize = 4; 

color[][][] pixels = new color[count[0]][count[1]][count[2]];



// starts here
void setup() {

  size(1000, 400, P3D);
  colorMode(HSB);
  background(0);
  strokeWeight(0);
  translateToCenter();
  lights();
  sphere(1);

  jsonPoints();

  drawPoints();
}

void translateToCenter() {
  translate(width/2, height/2, 0);
  lights();
}

void mouseRotate() {
  rotateX(radians(-1*(mouseY-height/2)/2));
  rotateY(radians(-1*(mouseX-width/2)/2));
}


void draw() {
  background(0);

  translateToCenter();
  
  
  if(mousePressed) {
    float fov = PI/3.0;
    float cameraZ = (height/2.0) / tan(fov/2.0);
    perspective(fov, float(width)/float(height),
                cameraZ/2.0, cameraZ*2.0);
  } else {
//    ortho(0, width/2, 0, height, -1000, 1000);
  ortho(-width/2, width/2, -height/2, height/2); 
}
 
 

  mouseRotate();



  pushMatrix();
  drawRadi();
  popMatrix();
}



void drawRadi() {

  
  //new radi

int ringRadius = 0;
for (int ring = 0; ring < rings; ring++) {
  ringRadius += spacing[ring];
  for (int row = 0; row < ringRows[ring]; row++ ) {

    for (int radius = 0; radius < count[0]; radius++) {

      float slice = TWO_PI / count[0];

      int x =  (int) (ringRadius * sin(slice*radius));
      int y =  (int) (ringRadius * cos(slice*radius));

      pushMatrix();

      translate(0, -1*ringRowSpacing[ring]*ringRows[ring]/2, 0);
      translate(x, ringRowSpacing[ring]*row, y);

      sphere(ballSize);
      popMatrix();
    }
  }


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


void drawPoints() {




  for (int z = 0; z < count[2]; z++) {
    for (int y = 0; y < count[1]; y++) {
      for (int x = 0; x < count[0]; x++) {
        pushMatrix();


        translate(-1*spacing[0]*count[0]/2, -1*spacing[1]*count[1]/2, -1*spacing[2]*count[2]/2);
        translate(x*spacing[0], y*spacing[1], z*spacing[2]);


        sphere(ballSize);
        popMatrix();
      }
    }
  }
}




void jsonPoints() {

  String jsonFileName = "layout-" +count[0]+"x"+count[1]+"x"+count[2]+".json";
  PrintWriter output;
  output = createWriter(jsonFileName);

  print("[\n");
  output.print("[\n");



  for (int offset = 0; offset < 4; offset++) {  


    for (int z = 0; z < count[2]; z++) {
      for (int y = 0; y < count[1]; y++) {
        for (int x = 0; x < count[0]; x++) {

          // output layout data
          if ((offset != 0) || (x != 0) || (y != 0) || (z != 0) ) {
            print(",\n");
            output.print(",\n");
          }

          float i = x - 2.5 + offset * 20;
          float j = y - 2.5;
          float k = z - 2.5;

          print("  {\"point\": [" + i + ", " + j+ ", " + k + "]}");
          output.print("  {\"point\": [" + i + ", " + j+ ", " + k + "]}");
        }
      }
    }
  } // boxes


  println("\n]");
  output.println("\n]");

  output.flush(); // Write the remaining data
  output.close(); // Finish the file

  return;
}