#ifndef ComHandler_H
#define ComHandler_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#include <cstring>

#include "message.h"

class ComHandler {
 public:
  // Define a type for the callback function
  typedef void (*CallbackFunction)(void* data, size_t len);
  ComHandler() : sendFunction(nullptr) {
  }
  void setSendCallback(CallbackFunction callback) {
    sendFunction = callback;
  }

  void dataReceived(const uint8_t* data, size_t len) {
    if (len != 24) {
      return;
    }

    Message message;
    message.decode(data);

    if (message.checkChecksum() && !isSeen(message.getMsgID()) &&
        !hasNewMessageAvailable) {
      newMessage = message;
      hasNewMessageAvailable = true;
      sendFunction(message.encode(), len);
    }
  }

  bool hasNewMessage() {
    return hasNewMessageAvailable;
  }

  bool isRelevant(uint8_t group) {
    uint8_t received_mac[6];
    uint8_t broadcast_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    newMessage.getReceiverMacAddress(received_mac);
    bool chk_mac =
        memcmp(received_mac, this->macAddress, sizeof(received_mac)) == 0 ||
        memcmp(received_mac, broadcast_mac, sizeof(received_mac)) == 0;
    bool chk_group =
        newMessage.getGroup() == 0 || newMessage.getGroup() == group;
    return chk_group && chk_mac;
  }

  Message getNewMessage() {
    hasNewMessageAvailable = false;
    return newMessage;
  }

  void send(const Message& msg) {
    sendFunction(msg.encode(), 24);
  }

  void setMacAddress(const uint8_t mac[6]) {
    std::copy(mac, mac + 6, macAddress);
  }

 private:
  CallbackFunction sendFunction;
  std::vector<uint16_t> seenMsgs;

  uint8_t macAddress[6];

  Message newMessage;
  bool hasNewMessageAvailable = false;

  bool isSeen(uint16_t seqNum) {
    for (uint16_t msgID : seenMsgs) {
      if (msgID == seqNum) {
        return true;
      }
    }
    seenMsgs.push_back(seqNum);
    if (seenMsgs.size() > 32) {
      seenMsgs.erase(seenMsgs.begin());
    }
    return false;
  }
};

#endif  // ComHandler_H