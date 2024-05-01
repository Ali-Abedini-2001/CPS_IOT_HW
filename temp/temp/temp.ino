// #include <Ethernet.h>

// #define REQUEST_RATE 5000 // milliseconds

// // ethernet interface mac address
// byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
// // ethernet interface ip address
// byte myip[] = { 192,168,2,2 };
// // gateway ip address
// byte gwip[] = { 192,168,2,1 };
// // remote website ip address and port
// byte hisip[] = { 192,168,2,1 };
// // remote website name
// const char website[] = "192.168.2.1";

// EthernetClient client;

// int count = 0;
// char c;
// String id;

// void setup() {
//   Serial.begin(57600);
//   Serial.println("\n[getStaticIP]");

//   if (Ethernet.begin(mymac) == 0) {
//     Serial.println("Failed to configure Ethernet using DHCP");
//     // no point in carrying on, so do nothing:
//     while (true) {
//       delay(1);
//     }
//   }

//   Serial.println("Gateway found");
// }

// void loop() {
//   while (Serial.available()>0){
//         c = Serial.read();
//         count++;
//         id += c;
//   }
//   count = 0;
//   id = "";
//   Serial.println("\n>>> REQ");

//   if (client.connect(website, 80)) {
//     String postData = "{\"type\":\"RFID\", \"data\":{\"rfid\":\"sadadadsfsf\"}}";
//     client.print("POST /rfidcheck HTTP/1.1\r\n");
//     client.print("Host: localhost:4321\r\n");
//     client.print("Content-Type: application/json\r\n");
//     client.print("Content-Length: ");
//     client.print(postData.length());
//     client.print("\r\n\r\n");
//     client.print(postData);

//     // read the response
//     while (client.connected()) {
//       if (client.available()) {
//         char ch = client.read();
//         Serial.write(ch);
//       }
//     }

//     client.stop();
//   } else {
//     Serial.println("connection failed");
//   }

//   delay(REQUEST_RATE);
// }


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
const char website[] PROGMEM = "localhost:4321";
byte Ethernet::buffer[700];

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
    Serial.println();
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, &Ethernet::buffer[off]);
    if (!error) {
        const char* message = doc["message"];
        if (strcmp(message, "RFID accepted") == 0) {
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
           

            StaticJsonDocument<200> jsonDocument;
            jsonDocument["type"] = "RFID";
            jsonDocument["data"]["rfid"] = id;
            serializeJson(jsonDocument, jsonBuffer);
            }
            break;
      }
        ether.httpPost(PSTR("/rfidcheck"), website, PSTR("Content-Type: application/json"), jsonBuffer, website_callback);
      }

     
    }
  }
    
    count = 0;
    id = "";
}
//sadadadsfsf


