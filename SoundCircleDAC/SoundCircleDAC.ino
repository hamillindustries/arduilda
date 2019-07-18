#include <MCP4922.h> //DAC library from https://github.com/helgenodland/MCP4922-Arduino-SPI-Library
#include <SPI.h> //Serial Peripheral Interface library used by the DAC library

MCP4922 DAC(51, 52, 53, 5); // (MOSI,SCK,CS,LDAC) define Connections for MEGA_board,
//MCP4922 DAC(11,13,10,5);    // (MOSI,SCK,CS,LDAC) define Connections for UNO_board,

int sampling_points = 50; //Define how many sampling points are going to draw the circle.

boolean trigger; //trigger boolean
int life = 0; //decay of the circle

int r = 4, g = 3, b = 2; //Define color pins
int r_v = 1, g_v = 0, b_v = 0; //Define color values

void setup()
{
  Serial.begin(19200); //Begin serial to connect with Processing
  SPI.begin();

  //Set color pins as outputs
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);

}

void loop()
{
  setColors(); //Set colors
  if(recvTrigger()) life = 100; //Set life to maximum //Receive trigger

  circle(500 + 5 * life, sampling_points, 0, 0); //Draw circle
  
  if (trigger) life -= 5; //Decay
  if (life <= 0) trigger = false; //Remove trigger when decay is over.

}

void circle(int r, int s_p, int x_o, int y_o) {

  float w = TWO_PI / s_p; //Equal divisions of the circle domain.

  for (int i = 0; i < s_p; i++) {
    int x = map(int(sin(i * w) * r) + x_o, -2048, 2048, 0, 4095); //Map sine to DAC code values.
    int y = map(int(cos(i * w) * r) + y_o,  -2048, 2048, 0, 4095); //Map sine to DAC code values.
    DAC.Set(x, y); // Send to DAC
  }

}

boolean recvTrigger() {
  if (Serial.available() > 0) { //If there's a trigger
    char receivedChar = Serial.read(); //Flush the buffer by reading the char.
    trigger = true; //Set trigger on
    return true;
  }
  else {
    return false;
  }
}

void setColors() {
  digitalWrite(r, r_v);
  digitalWrite(g, g_v);
  digitalWrite(b, b_v);
}

void resetColors() {
  digitalWrite(r, LOW);
  digitalWrite(g, LOW);
  digitalWrite(b, LOW);
}
