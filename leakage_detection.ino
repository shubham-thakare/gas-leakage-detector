/* Schematics
 * GSM --> Arduino
 * GND --> GND 
 * TX  --> D9
 * RX  --> D10
 * 
 * MQ-5 --> Arduino
 * VCC --> 5V
 * GND --> GND
 * D0  --> D8
 * 
 * PIEZO --> Arduino
 * VCC   --> 5V
 * BLACK --> D12
 * 
 * 16x2 CLCD DISPLAY I2C --> Arduino
 * VCC --> 5V
 * GND --> GND
 * SDA --> A4
 * SCL --> A5
 */

#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
SoftwareSerial myGsm(9, 10);
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //16x2 CLCD Display
char msg;

int sensorPin = 8;
int piezoPin = 12;
int gas_value;
String mblNo = "+917776096117";

void setup()
{
  //CLCD Display
  lcd.begin(16,2); 
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("GAS Leackage");
  lcd.setCursor(0,1);
  lcd.print("Detection System");
  
  Serial.println("GAS Leackage Detection System");
  myGsm.begin(9600);                                      // Setting the baud rate of GSM Module Communication 
  pinMode(sensorPin,INPUT);
  Serial.begin(9600);                                     // Setting the baud rate of Serial Communication 
  delay(1000);
}

void loop()
{
  gas_value=digitalRead(sensorPin);
  Serial.println(gas_value);
  if(gas_value == 0)
  {
    SendMessage();
    tone(piezoPin, 1000, 100000); 
    delay(10000000);
  }
  
  if (myGsm.available()>0)
    Serial.write(myGsm.read());
}

void SendMessage()
{   
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GAS Leackage");
  lcd.setCursor(0,1);
  lcd.print("Detected");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sending Message");
  lcd.setCursor(0,1);
  lcd.print(mblNo);
  delay(1000);
 
  myGsm.println("AT+CMGF=1");                             // Sets the GSM Module in Text Mode
  delay(1000);                                            // Delay of 1000 milli seconds or 1 second
  myGsm.println("AT+CMGS=\""+mblNo+"\"\r");               // Receipant's mobile number
  delay(1000);
  myGsm.print("Security Alert...");                       // SMS Text 1st Line 
  myGsm.println("GAS Leackage Detected!");                // SMS Text 2nd Line
  delay(100);
  myGsm.println((char)26);
  delay(5000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Message Sent!");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Alert Activated!");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press Reset");
  lcd.setCursor(0,1);
  lcd.print("To Stop Alert...");
}
