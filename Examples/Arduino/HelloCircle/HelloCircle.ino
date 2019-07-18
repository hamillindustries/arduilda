#include <MCP4922.h> //DAC library from https://github.com/helgenodland/MCP4922-Arduino-SPI-Library
#include <SPI.h> //Serial Peripheral Interface library used by the DAC library


// https://pastebin.com/bdWv9BPf


MCP4922 DAC(51, 52, 53, 5); // (MOSI,SCK,CS,LDAC) define Connections for MEGA_board,
//MCP4922 DAC(11,13,10,5);    // (MOSI,SCK,CS,LDAC) define Connections for UNO_board,

int sampling_points = 500; //Define how many sampling points are going to draw the circle.
int r = 4, g = 3, b = 2; //Define color pins

int r_v = 1, g_v = 0, b_v = 0;

void setup()
{
  SPI.begin();
  setColors();
}

void loop()
{
  for (int i = 0; i < sampling_points; i++) {
    int x = map(
      sin(i * TWO_PI / sampling_points)*2048, //Compute location of sampling point.
      -2048, 2048, 0, 4095); //Map it to DAC code values.
    int y = map(
      cos(i * TWO_PI / sampling_points)*2048, //Compute location of sampling point.
      -2048, 2048, 0, 4095);  //Map it to DAC code values.
    DAC.Set(x, y); // Send to DAC
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
