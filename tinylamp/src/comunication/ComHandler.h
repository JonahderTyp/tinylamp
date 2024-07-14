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
  void setMacAddress(const uint8_t mac[6]) {
    std::copy(mac, mac + 6, macAddress);
  }

  void dataReceived(const uint8_t* data, size_t len) {
    if (len != 24) {
      return;
    }

    Message message;
    message.decode(data);

    bool validchecksum = message.checkChecksum();
    bool newmessage = !isSeen(message.getMsgID());

    if (!validchecksum) {
      Serial.println("Invalid checksum");
    }
    if (!newmessage) {
      Serial.println("Already seen");
    }

    if (validchecksum && newmessage) {
      Serial.println("Message is valid");
      if (hasNewMessageAvailable == false) {
        newMessage = message;
        hasNewMessageAvailable = true;
      }
      Serial.println("Repeating message:");
      this->send(message);
    } else {
      Serial.println("Message is invalid");
    }
  }

  // Returns true if a new message is available
  bool hasNewMessage() {
    return hasNewMessageAvailable;
  }

  // Returns the new message
  Message getNewMessage() {
    hasNewMessageAvailable = false;
    return newMessage;
  }

  // Sends a new message
  void sendNewMessage(const Message& msg) {
    this->isSeen(msg.getMsgID());
    this->send(msg);
  }

 private:
  CallbackFunction sendFunction;
  std::vector<uint16_t> seenMsgs;

  uint8_t macAddress[6];

  Message newMessage;
  bool hasNewMessageAvailable = false;

  // Sends an Message object over the network
  void send(const Message& msg) {
    sendFunction(msg.encode(), 24);
  }

  // returns true if the message has been seen before
  // and adds the message to the list of seen messages
  // returns false if the message is unknown
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