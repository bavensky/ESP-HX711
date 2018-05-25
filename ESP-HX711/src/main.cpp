#include <Arduino.h>
#include "HX711.h"

#define DOUT  12
#define CLK   14
#define DEC_POINT  2
#define zero_factor 8618202 

// real weight 513 g
float calibration_factor = 8801.00; 


float offset=0;
float get_units_kg();

HX711 scale(DOUT, CLK);

void setup() 
{
  Serial.begin(115200);
  Serial.println("Load Cell");
  scale.set_scale(calibration_factor); 
  scale.set_offset(zero_factor);   
}
void loop() 
{ 
  Serial.print("Reading: ");
  String data = String(get_units_kg()+offset, DEC_POINT);
  Serial.print(data);
  Serial.println(" kg");
}
float get_units_kg()
{
  return(scale.get_units()*0.453592);
}