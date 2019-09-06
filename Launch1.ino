/*
  SD card read/write
 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 This example code is in the public domain.
 */
 /***************************************************************************

  This is a library for the BME280 humidity, temperature & pressure sensor
  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650
  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.
  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!
  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SD.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
int timeKeep = 0;

Adafruit_BME280 bme; // I2C

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  if (SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("Launch1.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt");
    myFile.print("Temperature ");
    myFile.print("Pressure ");
    myFile.print("Altitude ");
    myFile.print("Humidity ");
    myFile.print("Time ");
    myFile.println();
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening Launch1.txt");
  }
  
  
  // BME280 Portion of Setup Code
  Serial.begin(9600);
    Serial.println(F("BME280 test"));
    bool status;
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    Serial.println("-- Default Test --");
    Serial.println();
}

void loop() {
  printvalues();
  delay(10000);
}

void printvalues() {
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("Launch1.txt", FILE_WRITE);
  // if the file opened okay, write to it:
  if (myFile) {
   
    myFile.print(bme.readTemperature());
    myFile.print(" ");
    myFile.print(bme.readPressure() / 100.0F);
    myFile.print(" ");
    myFile.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    myFile.print(" ");
    myFile.print(bme.readHumidity()); 
    myFile.print(" ");   
    myFile.print(timeKeep*10);
    myFile.print(" ");
    myFile.println();
        
    timeKeep++;
    Serial.println();
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening Launch1.txt");
  }
}

