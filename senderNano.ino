#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <EEPROM.h>
#include "GravityTDS.h"
 
#define TdsSensorPin A3
GravityTDS gravityTds;

#define sensorPower 4
#define sensorPin A4
int value = 0;

float temperature = 25,tdsValue = 0;

#define ss 10
#define rst 9
#define dio0 2

String inString = "";    // string to hold input
char tdsValue_s[8];
char level_s[8];
float outgoing;
long lastSendTime = 0;        // last send time
int interval = 50;          // interval between sends

void setup()
{
  Serial.begin(115200);
    // Start the LCD and turn on the backlight:
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();  //initialization
  pinMode(sensorPower, OUTPUT);
	// Set to LOW so no power flows through the sensor
	digitalWrite(sensorPower, LOW);
	
  while (!Serial);
  Serial.println("LoRa one Way Communication");
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    delay(100);
    while (1);
  }
}

void loop()
{
  
    //temperature = readTemperature();  //add your temperature sensor and read it
    gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
    gravityTds.update();  //sample and calculate
    tdsValue = gravityTds.getTdsValue();  // then get the value
    //Serial.println("-----------------");
    Serial.print("TDS Value: ");
    Serial.print(tdsValue);
    Serial.println(" ppm");

    float level = readSensor();
	  Serial.print("Water level: ");
	  Serial.println(level);
    Serial.println("--------------------");
    dtostrf(tdsValue,5,2,tdsValue_s); /*Double converted to string*/
    dtostrf(level,5,2,level_s);

    delay(1000);
  // parse for a packet, and call onReceive with the result:
  sendMessage(tdsValue_s, level_s);
  //sendMessage(level);
}

//This is a function used to get the reading
float readSensor() {
	digitalWrite(sensorPower, HIGH);	// Turn the sensor ON
	delay(10);							// wait 10 milliseconds
	value = analogRead(sensorPin);		// Read the analog value form sensor
	digitalWrite(sensorPower, LOW);		// Turn the sensor OFF
	return value;							// send current reading
}

void sendMessage(String outgoing1, String outgoing2) {
  LoRa.beginPacket();                   // start packet
  LoRa.print(outgoing1+" "+outgoing2);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
}

