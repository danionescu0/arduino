/* 
 *  Libraries used by this project:
 *  
 *  GOFi2cOLED: https://github.com/hramrach/GOFi2cOLED
 *  Adafruit_MLX90614: https://github.com/adafruit/Adafruit-MLX90614-Library
 *  Adafruit_Sensor: https://github.com/adafruit/Adafruit_Sensor
 *  Adafruit_BME280: https://github.com/adafruit/Adafruit_BME280_Library
 *  Adafruit_VEML6070: https://github.com/adafruit/Adafruit_VEML6070
 */
#include <Wire.h>
#include <GOFi2cOLED.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_VEML6070.h"

GOFi2cOLED GOFoled;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_BME280 bme; 
Adafruit_VEML6070 uv = Adafruit_VEML6070();

struct sensorData
  {
      int uv;
      byte humidity;    
      int  pressure;
      int ambientTemperature;
      int pointTemperature;      
  };

sensorData sensors;
const byte buttonPin = 2;
const byte nrMetrics = 5;
volatile byte selectedMetric = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonPressed, RISING);
  initializeDisplay();
  initializeSensors();
}

void loop() 
{
  collectSensorsData();
  clearDisplay();
  writeSensorDataOnDisplayBuffer();
  doDisplay();
    Serial.println(selectedMetric);
  delay(1000);
}

void initializeDisplay()
{
  GOFoled.init(0x3C); 
  GOFoled.clearDisplay();
  GOFoled.setCursor(0, 0);
}

void initializeSensors()
{
  mlx.begin();
  bme.begin();
  uv.begin(VEML6070_1_T);
}

void setDisplayText(byte fontSize, String label, String data)
{
  GOFoled.setTextSize(fontSize);
  GOFoled.println(label + ":" + data);
}

void setDisplayText(byte fontSize, byte x, byte y, String data) 
{
  GOFoled.setTextSize(fontSize);
  GOFoled.setCursor(x, y);
  GOFoled.println(data);  
}

void doDisplay()
{ 
  GOFoled.display();
}

void clearDisplay()
{
   GOFoled.clearDisplay();
   GOFoled.setCursor(0, 0);
}

void collectSensorsData()
{
  sensors.ambientTemperature = mlx.readAmbientTempC(); 
  sensors.pointTemperature = mlx.readObjectTempC();
  sensors.ambientTemperature = bme.readTemperature();
  sensors.pressure = bme.readPressure() / 100.0F;
  sensors.humidity = bme.readHumidity();  
  sensors.uv = uv.readUV();
}

void writeSensorDataOnDisplayBuffer() 
{
  setDisplayText(1, getSelectedText(0, "Amb T"), String(sensors.ambientTemperature));
  setDisplayText(1, getSelectedText(1, "Point T"), String(sensors.pointTemperature));
  setDisplayText(1, getSelectedText(2, "Pressure"), String(sensors.pressure));
  setDisplayText(1, getSelectedText(3, "Humidity"), String(sensors.humidity));
  setDisplayText(1, getSelectedText(4, "Uv"), String(sensors.uv));
  setDisplayText(2, 60, 40, String(getSelectedMetric()));
}

String getSelectedText(byte position, String text)
{
  return position == selectedMetric ? "*" + text : text;
}

String getSelectedMetric()
{
  switch (selectedMetric) {
    case 0:
      return String(sensors.ambientTemperature);
    case 1:
      return String(sensors.pointTemperature);
    case 2:
      return String(sensors.pressure);
    case 3:
      return String(sensors.humidity);
    case 4:
      return String(sensors.uv);                  
    default:
      return String(sensors.ambientTemperature);
  }
}

void buttonPressed() 
{
  selectedMetric = selectedMetric < nrMetrics - 1 ? selectedMetric + 1 : 0;
}

