#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#include <vector>

#include "ColorWheel/ColorWheel.h"
#include "ESP8266TrueRandom.h"
#include "blink/blink.h"
#include "button/button.h"
#include "led/led.h"

// Variables


// Constants
const int MAX_SEEN_MSGS = 32;

// Structs
struct struct_message {
  byte reciverID[WL_MAC_ADDR_LENGTH];
  byte senderID[WL_MAC_ADDR_LENGTH];
  uint16_t msgID;
  uint16_t command;
  byte data[8];
  byte RFU[6];
  uint16_t CHK;
};

// CAN Variables
std::vector<uint16_t> seenMsgs;
struct_message myData;
struct_message recivedData;
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// LED Objects
Blink activity;
LED red(D5);
LED green(D6);
LED blue(D7);
LED white(D8);
ColorWheel colorWheel;

// LED Variables
uint8_t brightness = 0;
// bool ledState = false;

// Button Variables
Button b1(D2);
Button b2(D3);

unsigned long prevSendMillis = 0;

// Function Declarations
void printStruct(const struct_message &msg);
void copyMacAddress(struct struct_message *msg);
void printByteArray(byte *byteArray, int length);
bool isSeen(uint16_t seqNum);
void led(int i);
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len);

void handleLed(int pin, uint8_t brightness) { analogWrite(pin, brightness); }

void ledSelfTest() {
  for (int i = 0; i < 2; i++) {
    red.on();
    green.on();
    blue.on();
    white.on();
    delay(400);
    red.off();
    green.off();
    blue.off();
    white.off();
    delay(400);
  }
  red.on();
  green.on();
  blue.on();
  white.on();
  for (int i = 0; i < 255; i++) {
    red.setBrightness(i);
    green.setBrightness(i);
    blue.setBrightness(i);
    white.setBrightness(i);
  }

  red.setBrightness(0);
  green.setBrightness(0);
  blue.setBrightness(0);
  white.setBrightness(0);
}

// Setup Function
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  activity.setCallback(led);
  red.setCallback(handleLed);
  green.setCallback(handleLed);
  blue.setCallback(handleLed);
  white.setCallback(handleLed);

  ledSelfTest();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("Sucessfully initalized ESP-NOW");
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}

// Loop Function
void loop() {
  activity.handle();
  b1.handle();
  b2.handle();

  if (buttonsactive) {
  }

  if (b1.isShortPress()) {
    colorWheel.increment();
    red.setBrightness(colorWheel.getRed());
    green.setBrightness(colorWheel.getGreen());
    blue.setBrightness(colorWheel.getBlue());
    white.setBrightness(colorWheel.getWhite());
  }

  if (b1.isLongPress()) {
    for (int i = 0; i < 10; i++) {
      colorWheel.increment();
      red.setBrightness(colorWheel.getRed());
      green.setBrightness(colorWheel.getGreen());
      blue.setBrightness(colorWheel.getBlue());
      white.setBrightness(colorWheel.getWhite());
      delay(100);
    }
  }

  if (millis() - prevSendMillis >= 1000 && false) {
    Serial.println("\n\n\nSendingData:");
    prevSendMillis = millis();

    copyMacAddress(&myData);

    myData.msgID = ESP8266TrueRandom.random(UINT16_MAX);

    byte data[8] = {0xff, 0xff, 0xff, 0xff, 0, 0, 0, 0};
    memcpy(myData.data, data, sizeof(myData.data));

    myData.CHK = 0xFAFA;

    printStruct(myData);
    isSeen(myData.msgID);
    esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    activity.blink();
  }
}

// Callback when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&recivedData, incomingData, sizeof(recivedData));
  activity.blink();
  if (!isSeen(recivedData.msgID)) {
    Serial.print("\n\n\n");
    Serial.print("Bytes received: ");
    Serial.println(len);
    // Print the values received
    printStruct(recivedData);
    Serial.println("Rebroadcastin Data");
    esp_now_send(broadcastAddress, (uint8_t *)&recivedData,
                 sizeof(recivedData));
  } else {
    Serial.print("Message already seen with ID:");
    Serial.println(recivedData.msgID);
  }
}

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  } else {
    Serial.println("Delivery fail");
  }
}

// Check if message is seen
bool isSeen(uint16_t seqNum) {
  // Check if the message ID is already in the vector
  for (uint16_t msgID : seenMsgs) {
    if (msgID == seqNum) {
      return true;  // Message ID found
    }
  }

  // Message ID not found, add it to the vector
  seenMsgs.push_back(seqNum);

  // Check if the vector exceeds the maximum size, remove the oldest element
  // if necessary
  if (seenMsgs.size() > MAX_SEEN_MSGS) {
    seenMsgs.erase(seenMsgs.begin());
  }

  return false;  // Message ID not found previously
}

// Print byte array
void printByteArray(byte *byteArray, int length) {
  Serial.print("[ ");
  for (int i = 0; i < length; i++) {
    Serial.print(byteArray[i], HEX);
    Serial.print(" ");
  }
  Serial.println("]");
}

// Copy MAC address
void copyMacAddress(struct struct_message *msg) {
  // Get the MAC address of the ESP8266
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(mac);

  // Copy the MAC address into the senderID array
  memcpy(msg->senderID, mac, WL_MAC_ADDR_LENGTH);
}

// Print Struct
void printStruct(const struct_message &msg) {
  Serial.print("Receiver ID: ");
  for (int i = 0; i < WL_MAC_ADDR_LENGTH; i++) {
    Serial.print(msg.reciverID[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Sender ID: ");
  for (int i = 0; i < WL_MAC_ADDR_LENGTH; i++) {
    Serial.print(msg.senderID[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Message ID: ");
  Serial.println(msg.msgID);

  Serial.print("Command: ");
  Serial.println(msg.command);

  Serial.print("Data: ");
  for (unsigned int i = 0; i < sizeof(msg.data); i++) {
    Serial.print(msg.data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("RFU: ");
  for (unsigned int i = 0; i < sizeof(msg.RFU); i++) {
    Serial.print(msg.RFU[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.print("Checksum: ");
  Serial.println(msg.CHK);
}

// LED function
void led(int i) { digitalWrite(LED_BUILTIN, !i); }