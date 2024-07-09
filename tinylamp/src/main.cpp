#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#include "button/button.h"
#include "comunication/ComHandler.h"
#include "ledcontroller.h"
#include "valuewheel.h"

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

ComHandler comHandler;
LedController ledController(D5);
Button colorButton(D1);
Button brgButton(D2, 500);

ValueWheel<uint8_t> groupeWheel = ValueWheel<uint8_t>({0, 1, 2, 3, 4});

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
  Serial.println("\nStarting...");

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
  colorButton.handle();
  brgButton.handle();
  ledController.loop();

  if (colorButton.isShortPress()) {
    Serial.println("Increase Color");
    ledController.increaseColor();
  }
  if (colorButton.isLongPress()) {
    Serial.println("Increase Group");
    groupeWheel.increaseIndex();
    ledController.setGroup(groupeWheel.getIndex());
  }

  if (brgButton.isShortPress()) {
    Serial.println("Increase Brightness");
    ledController.increaseBrightness();
  }

  if (brgButton.isLongPress()) {
    Serial.println("Turning Off");
    ledController.turnOff();
  }

  if (comHandler.hasNewMessage()) {
    Message message = comHandler.getNewMessage();
    message.print();
  }
}
