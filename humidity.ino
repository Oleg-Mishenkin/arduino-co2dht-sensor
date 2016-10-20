#include "DHT.h"
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);
/*int prevCO2Val = LOW;
long th, tl, h, l, ppm;*/
SoftwareSerial co2SensorSerial(A0, A1); // A0 - к TX сенсора, A1 - к RX
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
int led = 7;
bool ledIsOn = false;
byte co2cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; 
unsigned char response[9];

void setup() {
  // put your setup code here, to run once:
  co2SensorSerial.begin(9600);

  dht.begin();
  lcd.begin(16, 2);
  lcd.print("FogSoft LLC");
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  readCO2();  
  readHumidityAndTemp();  
}

void readCO2(){
  co2SensorSerial.write(co2cmd, 9);
  memset(response, 0, 9);
  co2SensorSerial.readBytes(response, 9);
  int i;
  byte crc = 0;
  for (i = 1; i < 8; i++) crc+=response[i];
  crc = 255 - crc;
  crc++;
  lcd.setCursor(0,0);
  if ( !(response[0] == 0xFF && response[1] == 0x86 && response[8] == crc) ) {
    lcd.print("CRC error: " + String(crc) + " / "+ String(response[8]));
  } else {
    unsigned int responseHigh = (unsigned int) response[2];
    unsigned int responseLow = (unsigned int) response[3];
    unsigned int ppm = (256*responseHigh) + responseLow;
    lcd.print("PPM=" + String(ppm));

    if (ppm <= 1200 && ppm >= 800)
      analogWrite(led, 254); 
    else if (ppm > 1200){
      if (ledIsOn) {
        analogWrite(led, 0);
        ledIsOn = false;
      }
      else {
        analogWrite(led, 254); 
        ledIsOn = true;
      }
    }
    else analogWrite(led, 0);
  }
}

void readHumidityAndTemp()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    lcd.print("Failed read DHT!");
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  lcd.print(" H=" + String(h) + "%");
  lcd.setCursor(0, 1);
  lcd.print("T=" + String(t));
  lcd.print(" Ti=" + String(hic));
}


