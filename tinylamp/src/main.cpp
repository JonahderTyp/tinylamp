#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#include "button/button.h"
#include "comunication/ComHandler.h"
#include "ledcontroller.h"
#include "valuewheel.h"

uint8_t broadcastAddress[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t macAddress[6];

ComHandler comHandler;
LedController ledController(D5);
// Plus Button
Button colorButton(D1);
// Minus Button
Button brgButton(D2, 500);

bool bothPressed = false;

uint8_t group = 0;
ValueWheel<uint8_t> modeWheel = ValueWheel<uint8_t>({0, 1});
ValueWheel<uint8_t> menuWheel = ValueWheel<uint8_t>({0});

void sendMsg(void *data, size_t len) {
  esp_now_send(broadcastAddress, (uint8_t *)data, len);
}

// Callback when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  comHandler.dataReceived(incomingData, len);
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

void lamp() {
  if (colorButton.isShortPress()) {
    Serial.println("Increase Color");
    ledController.increaseColor();
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
    // uint8_t macAddress[6] = message.getReceiverMacAddress();

    message.print();
  }
}

void menu() {
  if (colorButton.isLongPress()) {
    menuWheel.increaseIndex();
    Serial.print("Menu ");
    Serial.println(menuWheel.getIndex());
  }
  switch (menuWheel.getIndex()) {
    case 0:
      if (colorButton.isShortPress()) {
        Serial.println("Increase Group");
        group++;
      }
      if (brgButton.isShortPress()) {
        Serial.println("Decrease Group");
        group--;
      }
      ledController.menu(menuWheel.getIndex(), group);
      break;
    default:
      ledController.menu(menuWheel.getIndex(), -1);
      break;
  }
}

// Example usage
void setup() {
  Serial.begin(115200);
  Serial.println("\nStarting...");
  wifi_get_macaddr(STATION_IF, macAddress);

  comHandler.setSendCallback(sendMsg);
  comHandler.setMacAddress(macAddress);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("Successfully initalized ESP-NOW");
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

  unsigned long colorPressTime = colorButton.getCurrentPressTime();
  unsigned long brgPressTime = brgButton.getCurrentPressTime();

  if (colorPressTime > 1000 && brgPressTime > 1000 && !bothPressed) {
    Serial.println("Double Press");
    bothPressed = true;
    modeWheel.increaseIndex();
    ledController.clear();
  } else if (colorPressTime == 0 && brgPressTime == 0) {
    bothPressed = false;
  }
  if (!(colorPressTime > 10 && brgPressTime > 10)) {
    switch (modeWheel.getIndex()) {
      case 1:
        // Serial.println("Menu");
        menu();
        break;
      case 0:
        // Serial.println("Lamp");
        lamp();
        break;
    }
  }
}
