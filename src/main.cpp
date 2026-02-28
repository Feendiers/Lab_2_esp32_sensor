#include <Arduino.h>
#include <SFE_BMP180.h>
#include <Adafruit_BMP280.h>


// Перечисление с типом датчиков
enum SensorType {BMP180 = 180, BMP280 = 280};

// Структура c данными, выравнивание 1 байт
#pragma pack(push, 1)
typedef struct _SENSOR_DATA{
  int Group; 
  char Name1[4]; 
  char Name2[4]; 
  SensorType Sensor; 
  double Pressure; 
  double Temperature; 
} SENSOR_DATA, *PSENSOR_DATA;
#pragma pack(pop)

// Создание структуры
SENSOR_DATA Data {
  30301,
  {'F', 'E', 'D', 'Y'},
  {'K', 'U', 'R', 'M'},
  BMP280,   // Выбор типа датчика, вписать BMP180 или BMP280
  0.0,
  0.0
};

// Объекты для работы с датчиками
SFE_BMP180 bmp180;
Adafruit_BMP280 bmp280;

void setup() 
{
  // Инициазация программного порта
  Serial.begin(115200);
  Serial.println("Init serial");
  
  // Проверка типа датчика

  // Для BMP180
  if (Data.Sensor == 180){
    bmp180.begin();
    Serial.println("BMP180 initialized");
  }

  // Для BMP280
  else if (Data.Sensor == 280){
    bmp280.begin(0x76);
    Serial.println("BMP280 initialized");
  }

  // Обработка ошибки проверки
  else {
    Serial.println("Mistake sensor initializtion");
    while (1); // Остановка программы при не корректном выборе
  }

  
  delay(1000);
}

void loop() 
{
  

  // Проверка типа датчика
  // Для 180
  if (Data.Sensor == 180){

    double temp, press;   // Переменные для записи

    // Начало работы датчика, получение данных для считывания температуры
    char check = bmp180.startTemperature();

    if (check > 0)
    {
      delay(check);
      bmp180.getTemperature(temp);    // Получение значения температуры
      
      // Получение данныз для считывания давления
      check = bmp180.startPressure(1);
      if (check > 0)
      {
        delay(check);
        bmp180.getPressure(press, temp);    // Получение значения давления
      }
    
    // Перевод в миллиметры ртутного столбца
    press = press * (760 * 100) / 101325;
   }

    // Отладочное сообщение
    //Serial.printf("Temp: %f\t Press: %f\n", temp, press);
    //delay(1000);

    // Запись данных в структуру
    Data.Temperature = temp;
    Data.Pressure = press;
  }

  // Для 280
  if (Data.Sensor == 280){

    // Считывание данных
    float T = bmp280.readTemperature();   
    float P = bmp280.readPressure();         

    // Перевод давления в миллиметры ртутного столбца
    P = P * 760 / 101325;

    // Отладочное сообщение
    //Serial.printf("Temp: %f, C | Press: %f, mmHg\n", T, P);
    //delay(1000);

    // Запись данных в структуру
    Data.Temperature = T;
    Data.Pressure = P;
  }

  // Отправка структуры в последовательный порт
  Serial.write((uint8_t*)&Data, sizeof(Data));
  delay(1000);
}