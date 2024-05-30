#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <dimmer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

bool isNumeric(const String& str);

class MyCallbacks: public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string rawValue = pCharacteristic->getValue();

    if (rawValue.length() > 0)
    {
      Serial.println("*********");
      Serial.print("New value: ");
      String value = String(rawValue.c_str());
      Serial.print(value);
      if (value == "full") {
        Dimmer::full();
      } else if (value == "off") {
        Dimmer::off();
      } else if (isNumeric(value)) {
        Dimmer::setBrightness(value.toInt());
      }
      Serial.println();
      Serial.println("*********");
    }
  }
};

void setup()
{
  delay(5000);
  Serial.begin(9600);

  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to ESP32-BLE_Server");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");

  BLEDevice::init("ESP32-BLE-Server");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop()
{
  delay(2000);
}

bool isNumeric(const String& str) {
    if (str.length() == 0) {
        // If the string is empty, it is not numeric
        return false;
    }

    // Check each character of the string
    for (size_t i = 0; i < str.length(); ++i) {
        // Check if the character is not a digit
        if (!isdigit(str.charAt(i))) {
            return false;
        }
    }

    // If all characters are digits, the string is numeric
    return true;
}
