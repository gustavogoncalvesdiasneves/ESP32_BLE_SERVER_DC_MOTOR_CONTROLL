#include <ArduinoBLE.h>
#include <Arduino.h>

#define SERVICE_UUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e" // UUID do serviço
#define READ_CHARACTERISTIC_UUID "6e400002-b5a3-f393-e0a9-e50e24dcca9e" // UUID da característica de leitura
#define WRITE_CHARACTERISTIC_UUID "6e400003-b5a3-f393-e0a9-e50e24dcca9e" // UUID da característica de escrita

#define MOTOR_PIN 5 // 5 pin on ESP32

BLEService customService(SERVICE_UUID); // Cria o serviço BLE
BLECharacteristic readCharacteristic(READ_CHARACTERISTIC_UUID, BLERead, 20); // Cria a característica de leitura
BLECharacteristic writeCharacteristic(WRITE_CHARACTERISTIC_UUID, BLEWrite, 20); // Cria a característica de escrita

uint8_t motor_on_off;

String base64_decode(String const& encodedString) {
  String decodedString;

  int in_len = encodedString.length();
  int i = 0;
  int j = 0;
  int in_ = 0;
  char char_array_4[4], char_array_3[3];

  const String base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

  while (in_len-- && (encodedString[in_] != '=') && (isalnum(encodedString[in_]) || (encodedString[in_] == '+') || (encodedString[in_] == '/'))) {
    char_array_4[i++] = encodedString[in_]; in_++;
    if (i == 4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.indexOf(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0x0F) << 4) + ((char_array_4[2] & 0x3C) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++) {
        decodedString += char_array_3[i];
      }
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.indexOf(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0x0F) << 4) + ((char_array_4[2] & 0x3C) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x03) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) {
      decodedString += char_array_3[j];
    }
  }

  return decodedString;
}

// Função para processar o comando recebido
void processCommand(const uint8_t* commandData, int commandSize) {
  // Decodifica os dados recebidos de base64 para uma string
  String encodedData;
  for (int i = 0; i < commandSize; i++) {
    encodedData += (char)commandData[i];
  }
  String commandString = base64_decode(encodedData);

  // Imprime o comando recebido
  Serial.print("Received command: ");
  Serial.println(commandString);

  if (commandString == "T1") {
    motor_on_off = 0x1;
    // digitalWrite(MOTOR_PIN, HIGH); // Liga o motor
    Serial.println("Motor Ligado");
  } else if (commandString == "T0") {
    motor_on_off = 0x0;
    // digitalWrite(MOTOR_PIN, LOW); // Desliga o motor
    Serial.println("Motor Desligado");
  }
}

void setup() {
  Serial.begin(115200);

  // Set the motor pin as an output
  pinMode(MOTOR_PIN, OUTPUT);

  // Inicializa o BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    // printf('Starting BLE failed!');s
    while (1);
  }

  // Define o nome e a força do sinal do dispositivo BLE
  BLE.setLocalName("ESP32_BLE");
  BLE.setAdvertisedService(customService);

  // Adiciona as características ao serviço
  customService.addCharacteristic(readCharacteristic);
  customService.addCharacteristic(writeCharacteristic);
  BLE.addService(customService);

  // Inicia a transmissão BLE
  BLE.advertise();

  Serial.println("BLE server started");
  printf("BLE server started");
}

void loop() {

  // digitalWrite(MOTOR_PIN, HIGH);

  // Espera por uma conexão BLE
  BLEDevice central = BLE.central();

  // digitalWrite(MOTOR_PIN, HIGH);

  // Se houver uma conexão BLE
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    // digitalWrite(MOTOR_PIN, motor_on_off);

    // Enquanto a conexão estiver ativa
    while (central.connected()) {

      digitalWrite(MOTOR_PIN, motor_on_off);
      // Verifica se há dados disponíveis para leitura
      // if (readCharacteristic.written()) {
      //   // Lê os dados recebidos
      //   const uint8_t* receivedData = readCharacteristic.value();
      //   int dataSize = readCharacteristic.valueLength();
        
      //   // Cria um buffer para armazenar os dados
      //   uint8_t buffer[dataSize + 1];
      //   memcpy(buffer, receivedData, dataSize);
      //   buffer[dataSize] = '\0'; // Adiciona um terminador nulo para usar a função println()

      //   // Imprime os dados recebidos
      //   Serial.print("Received: ");
      //   Serial.println((char*)buffer);
      // }

      // Verifica se há dados disponíveis para escrita
      if (writeCharacteristic.written()) {
        // Lê os dados escritos
        const uint8_t* commandData = writeCharacteristic.value();
        int commandSize = writeCharacteristic.valueLength();

        // Processa o comando recebido
        processCommand(commandData, commandSize);
      }

      delay(10);
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());

    // Reinicia o anúncio BLE para tornar o dispositivo visível novamente
    BLE.advertise();
  }
}

