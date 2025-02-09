// Aerolithe_ScissorLift.ino

// Code pour aller avec un ESP32-Wroom DevKit1 Wifi+BlueTooth
// https://abra-electronics.com/robotics-embedded-electronics/esp-series/wifi-esp32-esp-wroom-32-wi-fi-bluetooth-ble-low-power-iot-microcontroller.html
// utiliser le board ESP32 Dev Module ici dans Arduino IDE

#include <WiFi.h>
#include "wifi_module.h"
#include "stepper.h"
#include "aerolitheUdp.h"


const char *ssid = "Aerolithe";
const char *password = "Galactium2013";
const IPAddress local_IP(192, 168, 2, 13);  // THIS IP ADDRESS
const IPAddress gateway(192, 168, 2, 1);
const IPAddress subnet(255, 255, 255, 0);
const int udpPort = 44477;  // THIS RECEIVING PORT
unsigned long previousMillisTest;
const long intervalTest = 1000;

#include "stepper.h"

void setup() {
  Serial.begin(115200);
  connectToWiFi(ssid, password, local_IP, gateway, subnet);
  initializeLimitSwitches();
  startAerolitheUDP();
  sendResponse("J'ai terminé mon setup!");
  // Debug prints to check initial state
}

void loop() {
  checkAndReconnectWiFi(ssid, password, local_IP, gateway, subnet);
  udpGetIncoming();  // Check incomping UDP messages

  if (!runSpeedBool) {
    stepper.run();  // Allow the stepper motor to run at a fixed speed to a position
  } else {
    stepper.runSpeed();  // Allow the stepper motor to run loose at a defined speed
  }

  // if (upperLimitInterruptTriggered){
  //   Serial.println("upper limit switch reached");
  // }
  // if (lowerLimitInterruptTriggered){
  //   Serial.println("lower limit switch reached");
  // }


  // Non-blocking print of enablePin state every second
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisTest >= intervalTest) {
    previousMillisTest = currentMillis;
    // Serial.print("enablePin state: ");
    // Serial.println(digitalRead(enablePin));
    // Debugging: Read and print the state of dirPin
  }

  upperLimitInterruptTriggered = lowerLimitInterruptTriggered = false;
}