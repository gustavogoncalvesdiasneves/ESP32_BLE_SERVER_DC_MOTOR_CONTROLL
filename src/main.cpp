#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLEUtils.h>

BLEServer* pServer;
BLECharacteristic* pCharacteristic;
bool deviceConnected = false;

#define SERVICE_UUID        "19B10000-E8F2-537E-4F6C-D104768A1214"
#define CHARACTERISTIC_UUID "19B10001-E8F2-537E-4F6C-D104768A1214"

class MyServerCallbacks : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(NimBLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCharacteristicCallbacks : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      if (value.length() > 0) {
        Serial.println("Valor recebido: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);
        Serial.println();
        
        // Send response back to the client
        std::string response = "Received: ";
        response += value;
        pCharacteristic->setValue(response);
        pCharacteristic->notify();
      }
    }

     void onNotify(NimBLECharacteristic *pCharacteristic) {
      // This callback will be called when the characteristic is notified
      Serial.println("Notified value: ");
      std::string value = pCharacteristic->getValue();
      pCharacteristic->notify();
      Serial.println(value.c_str());
    }

    void onReadRequest(NimBLECharacteristic *pCharacteristic) {
      // This callback will be called when a client requests to read the characteristic's value
      Serial.println("Characteristic read requested");

      // You can set the value of the characteristic here before it's read by the client
      // For example, set the value to "Hello" when read is requested
      pCharacteristic->setValue("Hello");
    }
};

void setup() {
  Serial.begin(115200);

  // Initialize NimBLE
  NimBLEDevice::init("LED");

  // Create a server
  pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create a service
  NimBLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
                    );

  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  // Start the service
  pService->start();
  
  // Start advertising
  pServer->startAdvertising();
}

void loop() {
  if (deviceConnected) {
    // Do something while device is connected
  }
  delay(1000);
}
