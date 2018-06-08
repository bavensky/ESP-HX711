#include <Arduino.h>
#include "HX711.h"

#define DOUT1 4
#define CLK1 5

#define DOUT2 12
#define CLK2 14

float calibration_factor1 = 11636.00;
#define zero_factor1 8123913

float calibration_factor2 = 12120.00;
#define zero_factor2 7842768

const int numReadings = 10;
float readings1[numReadings];
float readings2[numReadings];
int readIndex = 0;
double average = 0;
uint32_t arrayScale[1000];
unsigned long standCount = 0;
unsigned long readSpeed = 0;
boolean standState = false;

HX711 scale1(DOUT1, CLK1);
HX711 scale2(DOUT2, CLK2);

float get_units_kg1()
{
  return (scale1.get_units() * 0.453592);
}

float get_units_kg2()
{
  return (scale2.get_units() * 0.453592);
}

void get_average()
{
  readings1[readIndex] = get_units_kg1(); // * 1000;
  readings2[readIndex] = get_units_kg2(); // * 1000;
  // Serial.print(get_units_kg1());
  // Serial.print("\t");
  // Serial.println(get_units_kg2());

  readIndex++;
  if (readIndex >= numReadings)
  {
    readIndex = 0;
  }

  average = 0;
  for (int x = 0; x < numReadings; x++)
  {
    average += (readings1[x] + readings2[x]);
  }

  average = (average / numReadings) - 2.65;

  // Serial.print("average = ");
  // Serial.println(average);
}

void setup()
{
  Serial.begin(57600);
  Serial.println("Load Cell");
  scale1.set_scale(calibration_factor1);
  scale1.set_offset(zero_factor1);

  scale2.set_scale(calibration_factor2);
  scale2.set_offset(zero_factor2);
}
void loop()
{
  get_average();

  standCount = 0;
  while (average > 50.00)
  {
    get_average();
    arrayScale[standCount] = average;
    // Serial.println(arrayScale[standCount]);
    delay(10);

    standCount += 1;
    standState = true;
  }

  if (standState == true)
  {
    int32_t arraySize = standCount;
    arraySize = arraySize / 5;

    double sumAVE = 0;
    for (int i = arraySize; i < (standCount - arraySize); i++)
    {
      sumAVE += arrayScale[i];
    }

    sumAVE = sumAVE / ((standCount - arraySize) - arraySize);
    Serial.println(sumAVE);

    standCount = 0;
    standState = false;
  }
}
