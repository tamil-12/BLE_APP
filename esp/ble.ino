#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Define the UUIDs for the service and characteristic
#define SERVICE_UUID        "55072829-bc9e-4c53-938a-74a6d4c78776"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// Declare MyServerCallbacks class before setup()
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("A central device connected.");
    }

    void onDisconnect(BLEServer* pServer) {
      Serial.println("A central device disconnected.");
    }

    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("Received value: ");
        for (int i = 0; i < value.length(); i++) {
          Serial.print(value[i]);
        }
        Serial.println();
      }
    }
};

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;

void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create the BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );

  // Set the initial value for the characteristic
  pCharacteristic->setValue("Hello World");

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a central device to connect...");
}

void loop() {
  // Nothing to do here for a BLE server
  delay(1000);
}
