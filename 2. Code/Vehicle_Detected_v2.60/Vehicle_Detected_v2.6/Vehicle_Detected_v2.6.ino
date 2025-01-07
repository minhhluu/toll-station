
#define BLYNK_PRINT Serial

// WiFi Credentials
#define BLYNK_TEMPLATE_ID "TMPL613MoKHub"
#define BLYNK_TEMPLATE_NAME "esp32"
#define BLYNK_AUTH_TOKEN "S5Vq4CFJbySdiG5LERpldtBzVDHU0BSo"

#include <SPI.h>      // Include SPI library for RFID communication
#include <MFRC522.h>  // Include MFRC522 library for RFID
#include <Wire.h>     // Include Wire library for I2C communication
#include <WiFi.h>     // Include WiFi library
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>  // Include Blynk library for ESP32

// RFID Pins
#define SS_PIN 15
#define RST_PIN 0
#define RX_1 16
#define TX_1 17

// char ssid[] = "Galaxy của Alan";
// char pass[] = "123456788";

// char ssid[] = "";
// char pass[] = "";

// char ssid[] = "";
// char pass[] = "";

// char ssid[] = "";
// char pass[] = "";

// char ssid[] = "";
// char pass[] = "";

char ssid[] = "FAI'S JETKING 2.4GHz";
char pass[] = "Fptacademy@2023";

// LED Pins
#define LED1 4
#define LED2 5
#define LED3 22

// RFID Instance
MFRC522 rfid(SS_PIN, RST_PIN);

// Blynk Timer
BlynkTimer timer;

// LED States and Counters
int counter1 = 1000000, counter2 = 1000000, counter3 = 1000000, counter_vehicle1 = 0, counter_vehicle2 = 0, counter_vehicle3 = 0, counter_vehicle_all = 0;
int ledState1 = LOW, ledState2 = LOW, ledState3 = LOW;

// Predefined UIDs and Names
struct UIDMapping {
  byte uid[4];       // UID
  const char *name;  // Associated person's name
};

const UIDMapping authorizedUIDs[] = {
  { { 0xFD, 0xCE, 0x7D, 0x00 }, "1" },
  { { 0x63, 0xDE, 0x60, 0x1A }, "2" },
  { { 0x0D, 0xC2, 0x29, 0x03 }, "3" },
  { { 0xDA, 0xDF, 0x78, 0x00 }, "4" },
};
const size_t authorizedUIDsCount = sizeof(authorizedUIDs) / sizeof(authorizedUIDs[0]);

// Variables for Serial Communication
int incoming_byte;
int temp = 1;

// Function Prototypes
void sendCounterToBlynk();
void sendTo8051(const char *data);
void printAuthorizedUIDs();

// Handle Blynk Virtual Pins for LEDs
/* */

// Send Counter Values to Blynk
void sendCounterToBlynk() {
  Blynk.virtualWrite(V3, counter1);
  Blynk.virtualWrite(V4, counter2);
  Blynk.virtualWrite(V5, counter3);
  //Blynk.virtualWrite(V10, counter4);
  Blynk.virtualWrite(V6, counter_vehicle1); // Gửi tổng số lần bật LED 1 (V6)
  Blynk.virtualWrite(V7, counter_vehicle2);
  Blynk.virtualWrite(V8, counter_vehicle3);
  //Blynk.virtualWrite(V11, counter_vehicle4);
  Blynk.virtualWrite(V9, counter_vehicle_all); // Gửi tổng số lần bật 3 LED (V9)

  // Serial.println("Counter1: " + String(counter1));
  // Serial.println("Counter2: " + String(counter2));
  // Serial.println("Counter3: " + String(counter3));
  // Serial.println("Total LED Count: " + String(counter_vehicle1));
  // Serial.println("Total LED Count: " + String(counter_vehicle2));
  // Serial.println("Total LED Count: " + String(counter_vehicle3));
  // Serial.println("Total vehicle Count: " + String(counter_vehicle_all));
}

// Print Authorized UIDs
void printAuthorizedUIDs() {
  Serial.println("Authorized UIDs:");
  for (size_t i = 0; i < authorizedUIDsCount; i++) {
    Serial.print("UID");
    Serial.print(i + 1);
    Serial.print(": ");
    for (size_t j = 0; j < 4; j++) {
      Serial.print("0x");
      if (authorizedUIDs[i].uid[j] < 0x10) Serial.print("0");
      Serial.print(authorizedUIDs[i].uid[j], HEX);
      if (j < 3) Serial.print(",");
    }
    Serial.print(" -> ");
    Serial.println(authorizedUIDs[i].name);
  }
}

// Send Data to 8051
void sendTo8051(const char *data) {
  for (int i = 0; i < 10; i++) {
    Serial.println(data);
  }
}

void print8051()
{
  for (int i = 0; i < 10; i++) {
    Serial.println('a');
  }
  temp = 3;
  counter_vehicle_all++;
  
}

void setup() {

  Serial.begin(9600);
  
  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Initialize RFID
  SPI.begin();
  rfid.PCD_Init();

  // Set up Timers
  timer.setInterval(6500L, sendCounterToBlynk);

  // Print Authorized UIDs
  printAuthorizedUIDs();

  // Send Initial Data to 8051
  sendTo8051("1");
  sendTo8051("2");
  sendTo8051("3");
  sendTo8051("4");
}


void loop() {

  Blynk.run();
  timer.run();

  if (temp == 1) {
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

    const char *personName = NULL;
    for (const auto &entry : authorizedUIDs) {
      bool match = true;
      for (byte j = 0; j < rfid.uid.size; j++) {
        if (rfid.uid.uidByte[j] != entry.uid[j]) {
          match = false;
          break;
        }
      }
      if (match) {
        personName = entry.name;
        break;
      }
    }

    if (personName) {
      sendTo8051(personName);
      temp = 2;
    } else {
      sendTo8051("Access denied");
    }
    rfid.PICC_HaltA();
  } else if (temp == 2) {
    if (Serial.available() > 0) {
      incoming_byte = Serial.read();
      if (incoming_byte == '1') {
        counter_vehicle1++;
        counter1 -= 30000;
        if (counter1 <= 0) counter1 = 1000000;
        print8051();
      } else if ( incoming_byte == '2'){
        counter_vehicle2++;
        counter2 -= 35000;
        if (counter2 <= 0) counter2 = 1000000;
        print8051();        
        // counter_vehicle_all++;
      } else if ( incoming_byte == '3'){
        counter_vehicle3++;
        counter3 -= 40000;
        if (counter3 <= 0) counter3 = 1000000;
        print8051();
      }
    }
  } else if (temp == 3) {
    if (Serial.available() > 0) {
      incoming_byte = Serial.read();
      if (incoming_byte == 'b') {
        for (int i = 0; i < 10; i++) {
          Serial.println('c');
          temp = 1;
        }
        Serial.println("Total Vehicle Count: " + String(counter_vehicle_all));
      }
    }
  }
}