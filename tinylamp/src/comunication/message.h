#ifndef message_H
#define message_H

#include <Arduino.h>

#define LENGTH 24

class Message {
 private:
  uint8_t data[LENGTH];

  uint16_t calculateChecksum() const {
    uint16_t checksum = 0;
    for (size_t i = 0; i < 22; ++i) {
      checksum += data[i];
    }
    return checksum;
  }

 public:
  Message() {
    std::fill(data, data + LENGTH, 0);
  }

  void setSenderMacAddress(const uint8_t mac[6]) {
    std::copy(mac, mac + 6, data);
  }

  void getSenderMacAddress(uint8_t mac[6]) const {
    std::copy(data, data + 6, mac);
  }

  void setReceiverMacAddress(const uint8_t mac[6]) {
    std::copy(mac, mac + 6, data + 6);
  }

  void getReceiverMacAddress(uint8_t mac[6]) const {
    std::copy(data + 6, data + 12, mac);
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

  void setCommandFunctionData(const uint8_t commandFunctionData[6]) {
    std::copy(commandFunctionData, commandFunctionData + 6, data + 16);
  }

  void getCommandFunctionData(uint8_t commandFunctionData[6]) const {
    std::copy(data + 16, data + 22, commandFunctionData);
  }

  uint8_t* encode() {
    uint16_t checksum = calculateChecksum();
    data[22] = checksum >> 8;
    data[23] = checksum & 0xFF;

    uint8_t* encodedData = new uint8_t[LENGTH];
    std::memcpy(encodedData, data, LENGTH);
    return encodedData;
  }

  void decode(const uint8_t* encodedData) {
    std::copy(encodedData, encodedData + LENGTH, data);
  }

  bool checkChecksum() const {
    uint16_t checksum = calculateChecksum();
    uint16_t receivedChecksum = (data[22] << 8) | data[23];
    return checksum == receivedChecksum;
  }

  void print() const {
    Serial.print("Sender MAC Address: ");
    for (int i = 0; i < 6; i++) {
      Serial.print(data[i], HEX);
      if (i < 5) Serial.print(":");
    }
    Serial.println();

    Serial.print("Receiver MAC Address: ");
    for (int i = 6; i < 12; i++) {
      Serial.print(data[i], HEX);
      if (i < 11) Serial.print(":");
    }
    Serial.println();

    Serial.print("Message ID: ");
    Serial.println(getMsgID());

    Serial.print("Group: ");
    Serial.println(getGroup());

    Serial.print("Function/Command: ");
    Serial.println(getFunctionOrCommand());

    Serial.print("Command/Function Data: ");
    for (int i = 16; i < 22; i++) {
      Serial.print(data[i], HEX);
      if (i < 21) Serial.print(" ");
    }
    Serial.println();

    Serial.print("Checksum: ");
    Serial.print(data[22], HEX);
    Serial.print(" ");
    Serial.println(data[23], HEX);
  }
};

#endif  // message_H
