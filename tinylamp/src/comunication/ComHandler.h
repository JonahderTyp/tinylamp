#ifndef ComHandler_H
#define ComHandler_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#include "message.h"

class ComHandler {
 public:
  // Define a type for the callback function
  typedef void (*CallbackFunction)(void* data, size_t len);
  ComHandler() : sendCallbackFunction(nullptr) {
  }
  void setSendCallback(CallbackFunction callback) {
    sendCallbackFunction = callback;
  }

  void dataReceived(const uint8_t* data, size_t len) {
    if (len != 24) {
      return;
    }
    // byte recivedData[24];

    Message message;
    message.decodeMessage(data);

    if (message.checkChecksum() && !isSeen(message.getMsgID())) {
      newMessage = message;
      hasNewMessageAvailable = true;
      sendCallbackFunction(message.encode(), len);
    }
  }

  bool hasNewMessage() {
    return hasNewMessageAvailable;
  }

  Message getNewMessage() {
    hasNewMessageAvailable = false;
    return newMessage;
  }

 private:
  CallbackFunction sendCallbackFunction;
  std::vector<uint16_t> seenMsgs;

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