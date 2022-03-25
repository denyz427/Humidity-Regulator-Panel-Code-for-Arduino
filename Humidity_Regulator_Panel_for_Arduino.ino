//Coded By denyz427 on 21th May 2021 find me at https://github.com/denyz427
//Get the DHT Sensor Library here: https://github.com/adafruit/DHT-sensor-library.git or Install from Library Manager 

#include "DHT.h"
#include <EEPROM.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // Uncomment if you use DHT11
//#define DHTTYPE DHT22 // Uncomment if you use DHT22
DHT dht(DHTPIN, DHTTYPE);

//defining pin number used on the board
#define RelayInput 12
#define UpPushButton 5
#define DownPushButton 4

float Humidity_upper_threshold;
float lower_limit_range = 2; //When humidity falls below Humidity_upper_threshold substracted by lower_limit_range, relay to activate misting system will be triggered

float h=0;
float t=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(RelayInput, OUTPUT);
  digitalWrite(RelayInput, HIGH);

  pinMode(UpPushButton, INPUT_PULLUP);
  pinMode(DownPushButton, INPUT_PULLUP);
  
  EEPROM.get(0, Humidity_upper_threshold);
  delay(5);
  if(isnan(Humidity_upper_threshold) || Humidity_upper_threshold<0 || Humidity_upper_threshold>100)
  {
    Humidity_upper_threshold=80;
    EEPROM.put(0, Humidity_upper_threshold);
    delay(5);
  }
  
  //Serial.begin(9600);
  dht.begin();

  lcd.init();    
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Humid:   % SetH:");
  lcd.setCursor(0, 1);
  if(Humidity_upper_threshold<100)
  {
    lcd.print("Temp:     C  "+ String(Humidity_upper_threshold,0) +"%");    // Uncomment if you prefer Celsius unit
    //lcd.print("Temp:     F  "+ String(Humidity_upper_threshold,0) +"%");  // Uncomment if you prefer Fahrenheit unit
  }
  else
  {
    lcd.print("Temp:     C "+ String(Humidity_upper_threshold,0) +"%");     // Uncomment if you prefer Celsius unit
    //lcd.print("Temp:     F "+ String(Humidity_upper_threshold,0) +"%");   // Uncomment if you prefer Fahrenheit unit
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  h = dht.readHumidity();
  t = dht.readTemperature();       // Uncomment if you prefer Celsius unit
  //t = dht.readTemperature(true); // Uncomment if you prefer Fahrenheit unit
  
  if(h<=Humidity_upper_threshold-lower_limit_range)
  {
    //Relay on
    digitalWrite(RelayInput, LOW);
  }

  if(h>=Humidity_upper_threshold)
  {
    //relay off
    digitalWrite(RelayInput, HIGH);
  }

  //Print current HUMIDITY to LCD
  if(h<100)
  {
    lcd.setCursor(6, 0);
    lcd.print(" "+String(h,0));
  }
  else
  {
    lcd.setCursor(6, 0);
    lcd.print(String(h,0));
  }

  //Print current TEMPERATURE to LCD
  lcd.setCursor(5, 1);
  lcd.print(String(t));

  //check if pushbutton to raise Humidity_upper_threshold is pressed
  while(digitalRead(UpPushButton)==LOW)
  {
    if(Humidity_upper_threshold<99)
    {
      Humidity_upper_threshold++;
      lcd.setCursor(13, 1);
      lcd.print(String(Humidity_upper_threshold,0));
      delay(300);
    }
    else if(Humidity_upper_threshold==99)
    {
      Humidity_upper_threshold++;
      lcd.setCursor(12, 1);
      lcd.print(String(Humidity_upper_threshold,0));
      delay(300);
    }
    
    if(digitalRead(UpPushButton)==HIGH)
    {
      EEPROM.put(0, Humidity_upper_threshold);
      delay(5);
    }
  }

  //check if pushbutton to lower Humidity_upper_threshold is pressed
  while(digitalRead(DownPushButton)==LOW)
  {
    if(Humidity_upper_threshold>0)
    {
      Humidity_upper_threshold--;
      lcd.setCursor(12, 1);
      lcd.print(" "+String(Humidity_upper_threshold,0));
      delay(300);
    }

    if(digitalRead(DownPushButton)==HIGH)
    {
      EEPROM.put(0, Humidity_upper_threshold);
      delay(5);
    }
  }
}
