#include <Arduino.h>
#include <MD_TCS230.h>
#include <LedControl.h>


#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5
MD_TCS230 ColorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);

const int displayCount = 1;
const int dataPin = 12;
const int  clkPin = 10;
const int csPin = 11;
LedControl lc = LedControl(dataPin, clkPin, csPin, displayCount);


void setup()
{
    Serial.begin(115200);

    sensorData whiteCalibration;
    whiteCalibration.value[TCS230_RGB_R] = 169420;
    whiteCalibration.value[TCS230_RGB_G] = 150530;
    whiteCalibration.value[TCS230_RGB_B] = 197380;

    sensorData blackCalibration;
    blackCalibration.value[TCS230_RGB_R] = 14030;
    blackCalibration.value[TCS230_RGB_G] = 11970;
    blackCalibration.value[TCS230_RGB_B] = 14570;


    ColorSensor.begin();
    ColorSensor.setDarkCal(&blackCalibration);
    ColorSensor.setWhiteCal(&whiteCalibration);

    lc.shutdown(0,false);
    lc.setIntensity(0,16);
    lc.clearDisplay(0);

}

void loop() 
{
    colorData rgb;
    ColorSensor.read();
    while (!ColorSensor.available());
    ColorSensor.getRGB(&rgb);
    
    lc.clearDisplay(0);
    int curR = convertToPixel(rgb.value[TCS230_RGB_R]);
    int curG = convertToPixel(rgb.value[TCS230_RGB_G]);
    int curB = convertToPixel(rgb.value[TCS230_RGB_B]);
    
    for (int i =0;i<=curR; i++)
    {
      lc.setLed(0, 0, i, true);
      lc.setLed(0, 1, i, true);
    }
    for (int i =0;i<=curG; i++)
    {
      lc.setLed(0, 3, i, true);
      lc.setLed(0, 4, i, true);
    }
    for (int i =0;i<=curB; i++)
    {
      lc.setLed(0, 6, i, true);
      lc.setLed(0, 7, i, true);
    }
    
    print_rgb(rgb);
}
int convertToPixel(float value)
{
  float constrained = constrain(value, 20, 120);
  return (int) constrain((constrained - 20)/7, 0, 7);
}

void print_rgb(colorData rgb)
{
  Serial.print(rgb.value[TCS230_RGB_R]);
  Serial.print(" ");
  Serial.print(rgb.value[TCS230_RGB_G]);
  Serial.print(" ");
  Serial.print(rgb.value[TCS230_RGB_B]);
  Serial.println();
}
