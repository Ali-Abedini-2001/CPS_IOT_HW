// include the library code:
#include <LiquidCrystal.h> //library for LCD 
#include <TimeLib.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

const int RedlED = 5;
const int GreenlED = 6;
const int DCmotor = 7;

int count = 0;
char c;
String id;

int wait = 5000;

void setup()
{
  Serial.begin(9600);
  pinMode(RedlED, OUTPUT);
  pinMode(GreenlED, OUTPUT);
  pinMode(DCmotor, OUTPUT);
  Serial.println("Please scan your RFID Card");

  lcd.begin(20, 4); // set up the LCD's number of columns and rows:
  lcd.setCursor(0, 0);
  lcd.print("Traffic Control Sys. ");
  lcd.setCursor(0, 1);
  lcd.print(" CARD:   ");
  lcd.setCursor(0, 2);
  lcd.print(" DATE:   ");
  lcd.setCursor(0, 3);
  lcd.print(" TIME:   ");
}

void loop()
{
  while (Serial.available() > 0)
  {
    c = Serial.read();
    count++;
    id += c;
    if (count == 10)
    {
      Serial.print(id);
      //break;

      if (id == "0810199555")
      {
        Serial.println("Valid Card");
        lcd.setCursor(0, 1);
        lcd.print(" CARD: valid   ");
        lcd.setCursor(0, 2);
        lcd.print(" DATE: 2024-04-18   ");
        lcd.setCursor(0, 3);
        lcd.print(" TIME: 2:29:30   ");
        digitalWrite(GreenlED, HIGH);
        digitalWrite(DCmotor, HIGH);
        delay(500);
        digitalWrite(DCmotor, LOW);
        delay(wait);
        digitalWrite(GreenlED, LOW);
        lcd.setCursor(0, 1);
        lcd.print(" CARD:           ");
        lcd.setCursor(0, 2);
        lcd.print(" DATE:           ");
        lcd.setCursor(0, 3);
        lcd.print(" TIME:           ");
      }

      else
      {
        Serial.println("Invalid Card");
        lcd.setCursor(0, 1);
        lcd.print(" CARD: Access Denied   ");
        lcd.setCursor(0, 2);
        lcd.print(" DATE: 2024-18-04     ");
        lcd.setCursor(0, 3);
        lcd.print(" TIME: 2:29:30   ");
        digitalWrite(RedlED, HIGH);
        delay(4000);
        digitalWrite(RedlED, LOW);
        lcd.setCursor(0, 1);
        lcd.print(" CARD:              ");
        lcd.setCursor(0, 2);
        lcd.print(" DATE:           ");
        lcd.setCursor(0, 3);
        lcd.print(" TIME:           ");
      }
    }
  }
  count = 0;
  id = "";
  delay(500);
}
