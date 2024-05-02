#include <LiquidCrystal.h> 
#include <EtherCard.h>
#include <ArduinoJson.h>

const int RedlED = 5;
const int GreenlED = 6;
const int DCmotor = 7;

int count = 0;
char c;
String id;

#define REQUEST_RATE 5000 // milliseconds
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 }; // ethernet interface
static byte myip[] = { 192,168,2,2 };
static byte mask[] = { 255, 255, 255, 0 };
static byte gwip[] = { 192,168,2,1 };
static byte hisip[] = { 192,168,2,1 };
const unsigned int websiteport = 80;
const char website[] PROGMEM = "localhost:80";
byte Ethernet::buffer[900];

static long timer;
Stash stash;

unsigned long printPeriod = 1000; //Refresh rate
unsigned long previousMillis = 0;

void setup() {
    Serial.begin(9600);
    pinMode(RedlED, OUTPUT);
    pinMode(GreenlED, OUTPUT);
    pinMode(DCmotor, OUTPUT);

    if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0) 
        Serial.println(F("Failed to access Ethernet controller"));

    ether.staticSetup(myip, gwip, NULL, mask);
    ether.copyIp(ether.hisip, hisip);
    ether.printIp("Server: ", ether.hisip);

    while (ether.clientWaitingGw())
        ether.packetLoop(ether.packetReceive());
    Serial.println("Gateway found");
    timer = - REQUEST_RATE; // start timing out right away
   
}

void website_callback (byte status, word off, word len) {
    Serial.println("<<<< RES");
    Serial.println((const char*) Ethernet::buffer + off);
    Serial.println(F("Received response from server:"));
    for (int i = off; i < off+len; i++) Serial.print((char)Ethernet::buffer[i]);
    char k = (char)Ethernet::buffer[off + 9];
    //for (int i = off; i < off+len; i++) (i == 10) ? k = (char)Ethernet::buffer[i] : ;

    //StaticJsonDocument<200> doc;
    //DeserializationError error = deserializeJson(doc, &Ethernet::buffer[off]);
    if (1) {
        //const char* message = doc["message"];
        const char* message = &Ethernet::buffer[off]; 
        //Serial.println(message);
        char a = '2';
        if (k == a) {
            Serial.println("Valid Card");
            digitalWrite(GreenlED, HIGH);
            digitalWrite(RedlED, LOW); 
            digitalWrite(DCmotor, HIGH);
            delay(500);
            digitalWrite(DCmotor, LOW);
            delay(5000); 
            digitalWrite(GreenlED, LOW);
        } else {
            Serial.println("Invalid Card");
            digitalWrite(RedlED, HIGH);
            digitalWrite(GreenlED, LOW); 
            delay(4000); 
            digitalWrite(RedlED, LOW);
        }
    } else {
        Serial.println("Error parsing JSON");
        digitalWrite(RedlED, HIGH);
        digitalWrite(GreenlED, LOW); 
        delay(4000); 
        digitalWrite(RedlED, LOW);
    }
}

void loop() {

  while ( true ) {
    if ((unsigned long)(millis() - previousMillis) >= printPeriod) {
      previousMillis = millis();

      Serial.println();

      ether.packetLoop(ether.packetReceive());
        if (millis() > timer + REQUEST_RATE) {
        timer = millis();
        Serial.println("\n>>> REQ");
        ether.hisport = 80;
        char jsonBuffer[200];
         while (Serial.available()){
        c = Serial.read();
        count++;
        id += c;
        if (count == 10){
            Serial.print(id);
              ether.httpPost(PSTR("/rfid"), website, PSTR("Content-Type: application/json"), id.c_str(), website_callback);           
            break;
        }
      }
      }
    }
  }
    
    count = 0;
    id = "";
}
//1111111111
//1111111110