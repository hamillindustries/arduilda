import processing.sound.*;
import processing.serial.*;

Serial myPort;
PGraphics pg;
float threshold = 0.5;
float gain = 10;
boolean peak = false;

Amplitude amp;
AudioIn in;

void setup() {
  size(200, 200, P2D);
  background(0);
  pg = createGraphics(200, 200);

  //Initialize serial
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[5], 19200);

  //Initialize sound
  amp = new Amplitude(this);
  in = new AudioIn(this, 0);
  in.start();
  amp.input(in);
}

void draw() {
  background(0);
  float an = gain*amp.analyze(); //Analyze amplitude and apply digital gain

  //Visualize amplitude:
  pg.beginDraw();
  pg.stroke(255);
  pg.line(0, 0+height*(1-an), 0, height);
  pg.stroke(0);
  pg.line(0, 0, 0, height*(1-an));
  pg.copy(0, 0, width, height, 1, 0, width, height);
  pg.endDraw();
  image(pg, 0, 0);

  //Visualize threshold line:
  stroke(0, 0, 225);
  line(0, (1-threshold)*height, width, (1-threshold)*height);

  if (!peak) { //Unless we're in a peak
    if (an>threshold) { //If audio over threshold
      myPort.write('a'); //Send trigger (one byte/char)
      peak = true; //Activate peak
    }
  } else {
    if (an<threshold) peak = false; //Deactivate peak when below threshold.
  }
}

void mousePressed() {
  threshold = 1-(mouseY/float(height)); //Set the threshold according to mouse position when pressing
}
