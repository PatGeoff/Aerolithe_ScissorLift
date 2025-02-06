//Test_Stepper.ino
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

  upperLimitInterruptTriggered = lowerLimitInterruptTriggered = false;
}