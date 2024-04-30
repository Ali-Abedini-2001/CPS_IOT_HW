// include the library code:
#include <LiquidCrystal.h> //library for LCD 
#include <EtherCard.h>
#include <EthernetENC.h>
#include <ArduinoJson.h>
// ENC28J60 module settings
#define Ethernet_CS 10 // CS pin for ENC28J60 module

// initialize the library with the numbers of the interface pins
// LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// HTTP server settings
#define HTTP_SERVER "http://localhost" // replace with your server URL
#define HTTP_PORT 4321

#define PATH    "rfidcheck"
#define VARIABLE    "test"

// JSON structure
char json[] = "{\"type\":\"RFID\",\"data\":{\"rfid\":\"0123456789\"}}";

// ethernet interface mac address
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
// ethernet interface ip address
static byte myip[] = { 192,168,2,2 };
// gateway ip address
static byte gwip[] = { 192,168,2,1 };
// remote website ip address and port
static byte hisip[] = { 192,168,2,1 };
// remote website name
const char website[] PROGMEM = "192.168.2.1";

byte Ethernet::buffer[700]; // buffer for sending and receiving 
uint32_t timer;
Stash stash;

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
  Serial.println("\n[webClient]");
  pinMode(RedlED, OUTPUT);
  pinMode(GreenlED, OUTPUT);
  pinMode(DCmotor, OUTPUT);
  Serial.println("Please scan your RFID Card");

  if (ether.begin(sizeof Ethernet::buffer, mymac, Ethernet_CS) == 0) {
    Serial.println(F("Failed to access Ethernet controller"));
  }
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");

  ether.staticSetup(myip, gwip);
  ether.copyIp(ether.hisip, hisip);
  ether.printIp("Server: ", ether.hisip);
  while (ether.clientWaitingGw())
    ether.packetLoop(ether.packetReceive());
  Serial.println("Gateway found");

  
  
  // Serial.println(Ethernet.localIP());
  // lcd.begin(20, 4); // set up the LCD's number of columns and rows:
  // lcd.setCursor(0, 0);
  // lcd.print("Traffic Control Sys. ");
  // lcd.setCursor(0, 1);
  // lcd.print(" CARD:   ");
  // lcd.setCursor(0, 2);
  // lcd.print(" DATE:   ");
  // lcd.setCursor(0, 3);
  // lcd.print(" TIME:   ");
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

       ether.packetLoop(ether.packetReceive());

        if (millis() > timer) {
          timer = millis() + 10000;

          byte sd = stash.create();
          stash.print("variable=");
          stash.print(VARIABLE);
          stash.print("&action=Submit");
          stash.save();

          // generate the header with payload - note that the stash size is used,
          // and that a "stash descriptor" is passed in as argument using "$H"
          Stash::prepare(PSTR("POST http://$F/$F.csv HTTP/1.0" "\r\n"
                      "Host: $F" "\r\n"
                      "Content-Length: $D" "\r\n"
                      "Content-Type: application/x-www-form-urlencoded" "\r\n"
                      "\r\n"
                      "$H"),
          website, PSTR(PATH), website, stash.size(), sd);

          // send the packet - this also releases all stash buffers once done
          ether.tcpSend();
        }

      // if (id == "0810199555")
      // {
      //   Serial.println("Valid Card");
      //   lcd.setCursor(0, 1);
      //   lcd.print(" CARD: valid   ");
      //   lcd.setCursor(0, 2);
      //   lcd.print(" DATE: 2024-04-18   ");
      //   lcd.setCursor(0, 3);
      //   lcd.print(" TIME: 2:29:30   ");
      //   digitalWrite(GreenlED, HIGH);
      //   digitalWrite(DCmotor, HIGH);
      //   delay(500);
      //   digitalWrite(DCmotor, LOW);
      //   delay(wait);
      //   digitalWrite(GreenlED, LOW);
      //   lcd.setCursor(0, 1);
      //   lcd.print(" CARD:           ");
      //   lcd.setCursor(0, 2);
      //   lcd.print(" DATE:           ");
      //   lcd.setCursor(0, 3);
      //   lcd.print(" TIME:           ");
      // }

      // else
      // {
      //   Serial.println("Invalid Card");
      //   lcd.setCursor(0, 1);
      //   lcd.print(" CARD: Access Denied   ");
      //   lcd.setCursor(0, 2);
      //   lcd.print(" DATE: 2024-18-04     ");
      //   lcd.setCursor(0, 3);
      //   lcd.print(" TIME: 2:29:30   ");
      //   digitalWrite(RedlED, HIGH);
      //   delay(4000);
      //   digitalWrite(RedlED, LOW);
      //   lcd.setCursor(0, 1);
      //   lcd.print(" CARD:              ");
      //   lcd.setCursor(0, 2);
      //   lcd.print(" DATE:           ");
      //   lcd.setCursor(0, 3);
      //   lcd.print(" TIME:           ");
      // }
    }
  }
  
 
  count = 0;
  id = "";
}

