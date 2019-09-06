/*
This code was written and adapted for monitoring and storing UV, Pressure,
Luminosity, Altitude, Temperature, Humidity, Accelation, and Direction for 
Purdue HAB Club

Written by Josh Fitch, Gavin Williams, Alex Loomis, and Noah Pictor

Wiring
Micro-SD
  CS-Pin4
  SCK-PIN13
  MOSI-PIN11
  MISO-PIN12

UV
  AOUT-A0

Ac/Mg
  A5
  A4
 */

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BME280.h>

#define BME_SCK 5
#define BME_MISO 6
#define BME_MOSI 7
#define BME_CS 8
#define SEALEVELPRESSURE_HPA (1013.25)

File data = SD.open("Data.txt", FILE_WRITE);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

void BME_startup()
{
  bool status;
  status = bme.begin();
  if (!status)
  {
    Serial.println("Could not find valid BME sensor");
  }
}

void SD_startup()
{
  if (!SD.begin(4)) {
    Serial.println("SD card initialization FAILED");
  }
  else  {
    Serial.println("SD card initialization SUCESS");
  }
  data = SD.open("data.txt", FILE_WRITE);
  if (data) {
    Serial.print("Writing first line in SD card...");
    data.println("UV(uW/m^2),Fx(m/s^2),Fy(m/s^2),Fz(m/s^2),Mx(uT),My(uT),Mz(uT),Temp(degC),Pres(hPa),Alt(m),Hum(%)"); //write first line (variable names) NEED TO EDIT LATER
    data.close();
    Serial.println("done");
  } 
  else {
    Serial.println("error opening data.txt");
  }
}

void UV_startup()
{
  pinMode(A0, INPUT); //declare pin A0 as an input  
}

void mag_startup()
{
  #ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  mag.enableAutoRange(true);
  if(!mag.begin())
  {
    Serial.println("No Mag");
  }  
  if(!accel.begin())
  {
    Serial.println("No Mag");
  }
}

uint16_t read_UV()
{
  int bits; //assign variabled
  float voltage;
  float mwatts;

    
  bits = analogRead(A0); //read the 10-bit value sent by the UV sensor to pin A0
  voltage = bits * (5.0/1023); //convert the 10-bit value into a voltage
  mwatts = (voltage * 168.75); //convert the voltage into mWatts by multiplying by a constant (volts into mW/m^2) 

  return mwatts;
}


void setup() {
  Serial.begin(9600);
//  lum_startup();
  BME_startup();
  SD_startup();
  UV_startup();
  mag_startup();
}

void loop() {
  uint16_t UV = read_UV();
  sensors_event_t eventa;
  sensors_event_t eventm;
  accel.getEvent(&eventa);
  mag.getEvent(&eventm);

  data = SD.open("data.txt", FILE_WRITE);
  if (data) {
    data.print(UV);
    data.print(",");
    data.print(eventa.acceleration.x);
    data.print(",");
    data.print(eventa.acceleration.y);
    data.print(",");
    data.print(eventa.acceleration.z);
    data.print(",");
    data.print(eventm.magnetic.x);
    data.print(",");
    data.print(eventm.magnetic.y);
    data.print(",");
    data.print(eventm.magnetic.z);
    data.print(",");
    data.print(bme.readTemperature());
    data.print(",");
    data.print(bme.readPressure()/100.0F);
    data.print(",");
    data.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    data.print(",");
    data.println(bme.readHumidity());
    data.close();
  } else {
    Serial.println("error opening test.txt");
  }  

  delay(1000);
}
