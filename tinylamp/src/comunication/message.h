#ifndef message_H
#define message_H

#include <Arduino.h>

#include <array>

#include "ESP8266TrueRandom.h"

#define LENGTH 24

class Message {
 private:
  std::array<uint8_t, LENGTH> data;

  uint16_t calculateChecksum() const {
    uint16_t checksum = 0;
    for (size_t i = 0; i < 22; ++i) {
      checksum += data[i];
    }
    return checksum;
  }

 public:
  Message() {
    data.fill(0);
  }

  void setSenderMacAddress(const std::array<uint8_t, 6> &mac) {
    std::copy(mac.begin(), mac.end(), data.begin());
  }

  std::array<uint8_t, 6> getSenderMacAddress() const {
    std::array<uint8_t, 6> mac;
    std::copy(data.begin(), data.begin() + 6, mac.begin());
    return mac;
  }

  void setReceiverMacAddress(const std::array<uint8_t, 6> &mac) {
    std::copy(mac.begin(), mac.end(), data.begin() + 6);
  }

  std::array<uint8_t, 6> getReceiverMacAddress() const {
    std::array<uint8_t, 6> mac;
    std::copy(data.begin() + 6, data.begin() + 12, mac.begin());
    return mac;
  }

  void setMsgID(uint16_t msgID) {
    data[12] = msgID >> 8;
    data[13] = msgID & 0xFF;
  }

  uint16_t getMsgID() const {
    return (data[12] << 8) | data[13];
  }

  void setGroup(uint8_t group) {
    data[14] = group;
  }

  uint8_t getGroup() const {
    return data[14];
  }

  void setFunction(uint8_t function) {
    data[15] = (function & 0b00111111) | 0x80;
  }

  void setCommand(uint8_t command) {
    data[15] = command & 0b00111111;
  }

  uint8_t getFunctionOrCommand() const {
    return data[15];
  }

  void setCommandFunctionData(
      const std::array<uint8_t, 6> &commandFunctionData) {
    std::copy(commandFunctionData.begin(), commandFunctionData.end(),
              data.begin() + 16);
  }

  std::array<uint8_t, 6> getCommandFunctionData() const {
    std::array<uint8_t, 6> commandFunctionData;
    std::copy(data.begin() + 16, data.begin() + 22,
              commandFunctionData.begin());
    return commandFunctionData;
  }

  // std::array<uint8_t, LENGTH> encodeMessage() {
  //   uint16_t checksum = calculateChecksum();
  //   data[22] = checksum >> 8;
  //   data[23] = checksum & 0xFF;
  //   return data;
  // }

  void *encode() {
    uint16_t checksum = calculateChecksum();
    data[22] = checksum >> 8;
    data[23] = checksum & 0xFF;

    uint8_t *encodedData = new uint8_t[LENGTH];
    std::memcpy(encodedData, data.data(), LENGTH);
    return static_cast<void *>(encodedData);
  }

  void decodeMessage(const uint8_t *encodedData) {
    std::copy(encodedData, encodedData + LENGTH, data.begin());
  }

  void decodeMessage(const std::array<uint8_t, LENGTH> &message) {
    data = message;
  }

  bool checkChecksum() const {
    uint16_t checksum = calculateChecksum();
    uint16_t receivedChecksum = (data[22] << 8) | data[23];
    return checksum == receivedChecksum;
  }
};

#endif  // message_H
