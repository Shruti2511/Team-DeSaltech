#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ss 5
#define rst 14
#define dio0 2

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
String inString = "";    // string to hold input

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Two Way Communication");
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    delay(100);
    while (1);
  }
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
}

void loop()
{
  //display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(1,20);
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    // read packet    
    while (LoRa.available())
    {
      int i=0;
      String incoming = LoRa.readString(); 
      Serial.print("TDS Value: ");
      display.print("TDS Value: ");
      while(incoming[i]!='\0'){
        Serial.print(incoming[i]);
        display.print(incoming[i]);
        i++;
        if(incoming[i]==' ')
        break;
      }
      i++;
      Serial.println(" ppm");
      display.print(" ppm");
      display.setCursor(1,30);
      Serial.print("Water level: ");
      display.print("Water level: ");
      while(incoming[i]!='\0'){
        Serial.print(incoming[i]);
        display.print(incoming[i]);
        i++;
      }
      Serial.println("");
      Serial.println("--------------------");
    }
    LoRa.packetRssi();    
  }
  display.display();
  delay(1000);
}
