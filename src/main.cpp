#include <Arduino.h>
#include <SFE_BMP180.h>

SFE_BMP180 bmp180;
void setup() 
{
  Serial.begin(115200);
  Serial.println("Init serial");
  
  bmp180.begin();
  delay(1000);
}

void loop() 
{
 
  double temp, press;
  char check = bmp180.startTemperature();

  if (check > 0)
  {
    delay(check);
    bmp180.getTemperature(temp);
    
  
    check = bmp180.startPressure(1);
    if (check > 0)
    {
      delay(check);
      bmp180.getPressure(press, temp);
      
    }
    
    press = press* (760 * 100) / 101325;
    
  }
  Serial.printf("Temp: %f\t Press: %f\n", temp, press);
  delay(1000);
}

