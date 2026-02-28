#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// --- Configuration ---
const uint8_t SERVO_PIN = D5; 
const uint32_t SERVO_FREQ = 50; 
const uint8_t PWM_RES = 12;

int currentAngle = 90; // Starting position (Middle)
const int OPEN_ANGLE = 20;  // Adjust these for your claw's physical limits
const int CLOSE_ANGLE = 160; 
const int SMOOTH_DELAY = 15; // Lower = faster, Higher = slower

// BLE UUIDs (You can keep these as is)
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// --- Servo Functions ---
void setServoAngle(int angle) {
  angle = constrain(angle, 0, 180);
  long pulseWidth = map(angle, 0, 180, 500, 2500);
  long duty = (pulseWidth * 4095) / 20000;
  ledcWrite(SERVO_PIN, duty);
}

void moveClawSmoothly(int targetAngle) {
  Serial.print("Moving to: "); Serial.println(targetAngle);
  while (currentAngle != targetAngle) {
    if (currentAngle < targetAngle) currentAngle++;
    else currentAngle--;
    
    setServoAngle(currentAngle);
    delay(SMOOTH_DELAY);
  }
}

// --- BLE Callback Class ---
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      String value = pCharacteristic->getValue().c_str();

      if (value.length() > 0) {
        char command = value[0];
        Serial.print("Received Command: "); Serial.println(command);

        if (command == 'o' || command == 'O') {
          moveClawSmoothly(OPEN_ANGLE);
        } 
        else if (command == 'c' || command == 'C') {
          moveClawSmoothly(CLOSE_ANGLE);
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  
  // Initialize Servo
  ledcAttach(SERVO_PIN, SERVO_FREQ, PWM_RES);
  setServoAngle(currentAngle);

  // Initialize BLE
  BLEDevice::init("Nano-Claw");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  Serial.println("Bluetooth Active! Connect via LightBlue app.");
}

void loop() {
  // BLE handles everything via callbacks, loop stays empty or for other tasks
}