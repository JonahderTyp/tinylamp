#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#include "button/button.h"
#include "comunication/ComHandler.h"
#include "ledcontroller.h"

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

ComHandler comHandler;
LedController ledController(D4);
Button b1(D3);
Button b2(D2);

void sendMsg(void *data, size_t len) {
  esp_now_send(broadcastAddress, (uint8_t *)data, len);
}

// Callback when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  comHandler.dataRecieved(incomingData, len);
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

// Example usage
void setup() {
  Serial.begin(115200);

  comHandler.setSendCallback(sendMsg);

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

void loop() {
  b1.handle();
  b2.handle();

  if (b1.isShortPress()) {
    ledController.increaseColor();
  }
  if (b2.isShortPress()) {
    ledController.increaseBrightness();
  }

  if (b1.isLongPress()) {
  }

  if (comHandler.hasNewMessage()) {
    Message message = comHandler.getNewMessage();
    // message.print();
  }
}
