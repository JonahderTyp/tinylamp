#ifndef message_H
#define message_H

#include <Arduino.h>
#include <ESP8266TrueRandom.h>

#define LENGTH 24

/*
Function MSB: 1
Command MSB: 0

*/

enum command {
  COLORINDEX = 0,
  BRIGHTNESSINDEX,
  GROUP,
  COMMANDRESERVE3,
  COMMANDRESERVE4,
  COMMANDRESERVE5
};
enum function {
  KEYLOCK = 0,
  FUNCTIONRESERVE1,
  FUNCTIONRESERVE2,
  FUNCTIONRESERVE3,
  FUNCTIONRESERVE4,
  FUNCTIONRESERVE5
};

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

  void setMsgID(uint16_t msgID) {
    data[12] = msgID >> 8;
    data[13] = msgID & 0xFF;
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

  uint16_t getMsgID() const {
    return (data[12] << 8) | data[13];
  }

  void setGroup(uint8_t group) {
    data[14] = group;
  }

  uint8_t getGroup() const {
    return data[14];
  }

  // void setFunction(uint8_t function) {
  //   data[15] = (function & 0b00111111) | 0x80;
  // }

  // void setCommand(uint8_t command) {
  //   data[15] = command & 0b00111111;
  // }

  // uint8_t getFunctionOrCommand() const {
  //   return data[15];
  // }

  void setIsComand() {
    data[15] &= 0x7F;
  }

  void setIsFunction() {
    data[15] |= 0x80;
  }

  bool isFunction() const {
    return data[15] & 0x80;
  }

  bool isCommand() const {
    return !isFunction();
  }

  // void setCommandFunctionData(const uint8_t commandFunctionData[6]) {
  //   std::copy(commandFunctionData, commandFunctionData + 6, data + 16);
  // }

  // void getCommandFunctionData(uint8_t commandFunctionData[6]) const {
  //   std::copy(data + 16, data + 22, commandFunctionData);
  // }

  bool getCFActive(uint8_t index) const {
    index = index % 6;
    return data[15] & (1 << index);
  }

  // activate or deactivate a command
  // automatically via setCommandFunctionDataAtIndex
  void setCommandActive(uint8_t index, bool active = true) {
    index = index % 6;
    if (active) {
      data[15] |= (1 << index);
    } else {
      data[15] &= ~(1 << index);
    }
  }

  uint8_t getCFDataAtIndex(uint8_t index) const {
    index = index % 6;
    return data[16 + index];
  }

  void setCFDataAtIndex(uint8_t index, uint8_t value) {
    index = index % 6;
    this->setCommandActive(index);
    data[16 + index] = value;
  }

  void finalize() {
    this->setMsgID(ESP8266TrueRandom.random(0, UINT16_MAX));
  }

  // from object to raw
  uint8_t* encode() const {
    // uint8_t localData[LENGTH];
    // std::memcpy(localData, data, LENGTH);

    uint16_t checksum = calculateChecksum();
    // localData[22] = checksum >> 8;
    // localData[23] = checksum & 0xFF;

    uint8_t* encodedData = new uint8_t[LENGTH];
    std::memcpy(encodedData, data, LENGTH);
    encodedData[22] = checksum >> 8;
    encodedData[23] = checksum & 0xFF;
    return encodedData;
  }

  // from raw to object
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

    if (isCommand()) {
      Serial.println("Is Command");
    } else {
      Serial.println("Is Function");
    }

    Serial.print("Active: ");
    for (int i = 0; i < 6; i++) {
      if (getCFActive(i))
        Serial.print("J");
      else
        Serial.print("N");
      Serial.print("\t");
    }
    Serial.println();

    Serial.print("Value:  ");
    for (int i = 0; i < 6; i++) {
      Serial.print(this->getCFDataAtIndex(i), HEX);
      Serial.print("\t");
    }
    Serial.println();

    Serial.print("Checksum: ");
    Serial.print(data[22], HEX);
    Serial.print(" ");
    Serial.println(data[23], HEX);
  }
};

#endif  // message_H
