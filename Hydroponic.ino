//
// 1/10/2021 - Remove Blynk
//
//
//
//
//
//



const char* host = "hydroponic";
char*  mqttUser = "mqtt";
char* mqttPass = "mqtt";
const char* mqtt_server = "192.168.88.100";
const char* ssid = "LamLie-IOT";
const char* password = "0193115005";
bool iConnected = false;
//bool netConnected = false;
bool ret;
bool mqttServerConnect = false;

#include "Hydroponic_menu.h"
#include "DFRobot_PH_EC.h"
#include <WiFi.h>
#include <ArduinoEEPROMAbstraction.h>
#include <ESPmDNS.h>
#include "ota.h"        //ota and wifi setup
#include "schedule.h"
#include "mqtt.h"
#include <ESP32Ping.h> //https://github.com/marian-craciunescu/ESP32Ping

const char pgmCommittedToRom[] PROGMEM = "Saved to ROM";
ArduinoEEPROMAbstraction arduinoEeprom(&EEPROM);

void setup() {
    Serial.begin(115200);
    // here we initialise the EEPROM class to 512 bytes of storage
    // don't commit often to this, it's in FLASH
    EEPROM.begin(512);
    // importantly, we then tell menu manager that this is the eeprom we are using
    menuMgr.setEepromRef(&arduinoEeprom);
    
    setupMenu();
    
    // lastly we load state from EEPROM.
    menuMgr.load();
    
    if (String(menuSSID.getTextValue()).length() > 0){
      ssid     = menuSSID.getTextValue();
      password = menuPassword.getTextValue();
    }

    Serial.print("ssid '");
    Serial.print(ssid);
    Serial.println("'");

    wifiSetup();
    dfrobot_ph_ec();
    otaSetup();
    
    if (WiFi.status() == WL_CONNECTED) {
        ret = Ping.ping("www.google.com",1);
        if (ret) { 
          setupNTP(); 
          iConnected = true; 
        } 
        ret = Ping.ping(mqtt_server);
        if (ret) { 
          MQTTsetup(); 
          mqttServerConnect = true; 
        }
    }
    scheduleSetup();
    offAll();

//run every 30 sec
  taskManager.scheduleFixedRate(30000, [] {
        if (WiFi.status() == !WL_CONNECTED) {
          wifiSetup();
        }
        if (WiFi.status() == !WL_CONNECTED && !iConnected) {
          ret = Ping.ping("www.google.com",1);
          if (ret) { 
            setupNTP(); 
            iConnected = true; 
          }
        }
    
        PumpSchedule();
    });  

  taskManager.scheduleFixedRate(5000, [] {
    
    
    if (!client.connected()) {
      ret = Ping.ping(mqtt_server,2);
      if (ret) { reconnect(); }    //mqtt
    }

  }); 
//OTA every 1 sec
  taskManager.scheduleFixedRate(1000, [] {
        
        timer();
        if (iConnected) { 
          printLocalTime(); }
        otaLoop();
        if (mqttServerConnect) { MQTTloop(); }
        readSensors();
        TestSchedule();
    });  
}

void loop() {
     taskManager.runLoop();
     
}

//
// This is attached to the save action on settings, in a real system we may have a
// low voltage detector or other solution for saving.
//
void CALLBACK_FUNCTION onSaveSettings(int id) {
  menuMgr.save();
  EEPROM.commit();
  Serial.println("Settings Saved");
 }

void CALLBACK_FUNCTION onReboot(int id) {
    ESP.restart();
}
