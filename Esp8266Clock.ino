#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>

const char* ssid     = "AAAA";
const char* password = "BBBB";

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN 14
#define DATA_PIN 13
#define CS_PIN 15

bool blinkState = true; 

MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 25200, 60000); 
// GMT+7 for Thailand (7*3600 = 25200 seconds)

void setup(){
  Serial.begin(115200);

  display.begin();
  display.setIntensity(0);
  display.displayClear();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  timeClient.begin();
}

void loop() {
  timeClient.update();

  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();

  char timeString[6];
  if (blinkState) {
    sprintf(timeString, "%02d:%02d", currentHour, currentMinute);
  } else {
    sprintf(timeString, "%02d %02d", currentHour, currentMinute);
  }
  
  blinkState = !blinkState;

  display.displayClear();
  display.setTextAlignment(PA_CENTER);
  display.print(timeString);

  delay(1000);
}
