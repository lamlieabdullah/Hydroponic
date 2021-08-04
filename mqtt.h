/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
*********/

#include <PubSubClient.h>


// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
int value = 0;
bool msgReceive;

char tempString1[16];
char tempString2[16];
char tempString3[16];
char tempString4[16];
char tempString5[16];
char tempString6[16];
char tempString7[16];
char tempString8[16];
char tempString9[16];
char tempString10[16];
char tempString11[16];
char tempString12[16];
char tempString13[16];
char tempString14[16];
char tempString15[16];
char tempString16[16];
char tempString17[16];
int tempString18;

bool temppumpRunning = false;
bool tempTestRunning = false;
bool tempMixRunning = false;
bool tempDoseRunning = false;

bool tempAuto = false;
bool tempAutoDose = false;
float tempTargetEC;
float tempMinEC;


void callback(char* topic, byte* message, unsigned int length) {
  //  Serial.print("Message arrived on topic: ");
  //  Serial.println(topic);
  msgReceive = true;
  //  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    //    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  //  Serial.println();

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  //  Serial.print(String(topic));
  //  Serial.print(messageTemp);

  if (String(topic) == "hydroponic/set/RunMin") {
    menuRunMin.setCurrentValue(messageTemp.toInt());
  }
  if (String(topic) == "hydroponic/set/IntervalMin") {
    menuIntervalMin.setCurrentValue(messageTemp.toInt());
  }
  if (String(topic) == "hydroponic/set/MlMin") {
    menuMlMin.setCurrentValue(messageTemp.toInt());
  }
  if (String(topic) == "hydroponic/set/Ml") {
    menuMl.setCurrentValue(messageTemp.toInt());
  }
  if (String(topic) == "hydroponic/set/MinimumEC") {
    tempMinEC = messageTemp.toFloat() * 10;
    menuMinimumEC.setCurrentValue(tempMinEC);
  }
  if (String(topic) == "hydroponic/set/TargetEC") {
    tempTargetEC = messageTemp.toFloat() * 10;
    menuTargetEC.setCurrentValue(tempTargetEC);
  }
  if (String(topic) == "hydroponic/set/EndTime") {
    menuEndTime.setCurrentValue(messageTemp.toInt());
  }
  if (String(topic) == "hydroponic/set/StartTime") {
    menuStartTime.setCurrentValue(messageTemp.toInt());
  }
  if (String(topic) == "hydroponic/set/TestTime") {
    menuTestTime.setCurrentValue(messageTemp.toInt());
  }
  if (String(topic) == "hydroponic/set/TestDuration") {
    menuTestDuration.setCurrentValue(messageTemp.toInt());
  }
  if (String(topic) == "hydroponic/set/MixDuration") {
    menuMixDuration.setCurrentValue(messageTemp.toInt());
  }

  if (String(topic) == "hydroponic/set/AutoDose") {
    if (messageTemp == "ON") {
      menuAutoDose.setBoolean(true);
    } else if (messageTemp == "OFF") {
      menuAutoDose.setBoolean(false);
    }
  }
  if (String(topic) == "hydroponic/set/Auto") {
    if (messageTemp == "ON") {
      menuAuto.setBoolean(true);
    } else if (messageTemp == "OFF") {
      menuAuto.setBoolean(false);
    }
  }
  if (String(topic) == "hydroponic/set/RunMix") {
    if (messageTemp == "ON") {
      runMix(40);
    } else if (messageTemp == "OFF") {
      onStopAll(40);
    }    
  }
  if (String(topic) == "hydroponic/set/runDose") {
    if (messageTemp == "ON") {
      runDose(40);
    } else if (messageTemp == "OFF") {
      onStopAll(40);  
    }
  }
  if (String(topic) == "hydroponic/set/TestNow") {
    if (messageTemp == "ON") {
      runTest(40);
    } else if (messageTemp == "OFF") {
      onStopAll(40);
    }    
  }
  if (String(topic) == "hydroponic/set/Pumping") {
    if (messageTemp == "ON") {
      RunPump(40);
    } else if (messageTemp == "OFF") {
      onStopAll(40);
    }      
  }
  if (String(topic) == "hydroponic/set/StopAll") {
    onStopAll(40);
  }
  if (String(topic) == "hydroponic/set/SaveSettings") {
    onSaveSettings(40);
  }
  if (String(topic) == "hydroponic/set/Reboot") {
    onReboot(40);
  }
  if (String(topic) == "hydroponic/set/CalSelect") {
    if (messageTemp == "EC") {
      (menuoptCall.setCurrentValue(0));
    } else if (messageTemp == "pH") {
      (menuoptCall.setCurrentValue(1));
    }
  }
  if (String(topic) == "hydroponic/set/EnterCall") {
    calecph(40);
  } 
}

void MQTTsetup() {

  client.setServer(mqtt_server, 1883);
  if (client.connect(host, mqttUser, mqttPass)) {
    client.publish("outTopic", "hello world");
    boolean test = client.subscribe("hydroponic/set/#");
    if (test) {
      Serial.println("MQTT Subscribed"); 
    }

  }
  client.setCallback(callback);

}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(host, mqttUser, mqttPass)) {
      Serial.println("connected"); 
      // Subscribe
      client.subscribe("hydroponic/set/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      // jangan la delayyyy delay(5000);
      return;
    }
  }
}
void MQTTloop() {
  String str;
  float tempTargetEC;
  float tempMinEC;

  client.loop();

  if (msgReceive) {
    msgReceive = false;
    return;
  }
  char tempString[16];

  str = menustatus.getTextValue();
  str.toCharArray(tempString, 16);
  if (strcmp(tempString, tempString13) != 0) {
    strcpy(tempString13, tempString);
    client.publish("hydroponic/status", tempString);
    Blynk.virtualWrite(V0, tempString);
  }

 
  dtostrf(menuRunMin.getCurrentValue(), 1, 0, tempString);
  if (strcmp(tempString, tempString7) != 0) {
    strcpy(tempString7, tempString);
    client.publish("hydroponic/RunMin", tempString);
    Blynk.virtualWrite(V1, tempString);
  
  }  
  dtostrf(menuIntervalMin.getCurrentValue(), 1, 0, tempString);
  if (strcmp(tempString, tempString6) != 0) {
    strcpy(tempString6, tempString);
    client.publish("hydroponic/IntervalMin", tempString);
    Blynk.virtualWrite(V2, tempString);

  }
  dtostrf(menuMlMin.getCurrentValue(), 1, 0, tempString);
  if (strcmp(tempString, tempString10) != 0) {
    strcpy(tempString10, tempString);
    client.publish("hydroponic/MlMin", tempString);
    Blynk.virtualWrite(V3, tempString);
  }
  dtostrf(menuMl.getCurrentValue(), 1, 0, tempString);
  if (strcmp(tempString, tempString11) != 0) {
    strcpy(tempString11, tempString);
    client.publish("hydroponic/Ml", tempString);
    Blynk.virtualWrite(V4, tempString);
  }
  tempMinEC = menuMinimumEC.getCurrentValue();
  dtostrf(tempMinEC/10, 1, 1, tempString);
  if (strcmp(tempString, tempString8) != 0) {
    strcpy(tempString8, tempString);
    client.publish("hydroponic/MinimumEC", tempString);
    Blynk.virtualWrite(V5, tempString);
  }
  tempTargetEC = menuTargetEC.getCurrentValue();
  dtostrf(tempTargetEC/10, 1, 1, tempString);
  if (strcmp(tempString, tempString9) != 0) {
    strcpy(tempString9, tempString);
    client.publish("hydroponic/TargetEC", tempString);
    Blynk.virtualWrite(V6, tempString);
  }
  dtostrf(menuEndTime.getCurrentValue(), 1, 0, tempString);
  if (strcmp(tempString, tempString4) != 0) {
    strcpy(tempString4, tempString);
    client.publish("hydroponic/EndTime", tempString);
    Blynk.virtualWrite(V7, tempString);
  }
  dtostrf(menuStartTime.getCurrentValue(), 1, 0, tempString);
  if (strcmp(tempString, tempString5) != 0) {
    strcpy(tempString5, tempString);
    client.publish("hydroponic/StartTime", tempString);
    Blynk.virtualWrite(V8, tempString);
  }
  dtostrf(menuTestTime.getCurrentValue(), 1, 0, tempString);
  if (strcmp(tempString, tempString1) != 0) {
    strcpy(tempString1, tempString);
    client.publish("hydroponic/TestTime", tempString);
    Blynk.virtualWrite(V9, tempString);
  }
  dtostrf(menuTestDuration.getCurrentValue(), 1, 0, tempString);
  if (strcmp(tempString, tempString2) != 0) {
    strcpy(tempString2, tempString);
    client.publish("hydroponic/TestDuration", tempString);
    Blynk.virtualWrite(V10, tempString);
  }

  dtostrf(menuMixDuration.getCurrentValue(), 1, 0, tempString);
  if (strcmp(tempString, tempString3) != 0) {
    strcpy(tempString3, tempString);
    client.publish("hydroponic/MixDuration", tempString);
    Blynk.virtualWrite(V11, tempString);
  }
  if (menuAutoDose.getBoolean() != tempAutoDose) {
    tempAutoDose = menuAutoDose.getBoolean();
    if (tempAutoDose) {
      client.publish("hydroponic/AutoDose", "ON", true);
      Blynk.virtualWrite(V12, 1);
    } else {
      client.publish("hydroponic/AutoDose", "OFF", true);
      Blynk.virtualWrite(V12, 0);
    }
  }
  if (menuAuto.getBoolean() != tempAuto) {
    tempAuto = menuAuto.getBoolean();
    if (tempAuto) {
      client.publish("hydroponic/Auto", "ON", true);
      Blynk.virtualWrite(V13, 1);  
    } else {
      client.publish("hydroponic/Auto", "OFF", true);
      Blynk.virtualWrite(V13, 0);
    }
  } 
  if (tempMixRunning != MixRunning) {
    tempMixRunning = MixRunning;
    if (tempMixRunning){client.publish("hydroponic/MixRunning", "ON"); 
      Blynk.virtualWrite(V14, 1);    
    } else { 
      client.publish("hydroponic/MixRunning", "OFF"); 
      Blynk.virtualWrite(V14, 0);      
      }
  }
  if (tempDoseRunning != DoseRunning) {
    tempDoseRunning = DoseRunning;
    if (tempDoseRunning){client.publish("hydroponic/DoseRunning", "ON"); 
      Blynk.virtualWrite(V15, 1);    
    } else { 
      client.publish("hydroponic/DoseRunning", "OFF"); 
      Blynk.virtualWrite(V15, 0);      
      }
  }
  if (tempTestRunning != TestRunning) {
    tempTestRunning = TestRunning;
    if (tempTestRunning){client.publish("hydroponic/TestRunning", "ON"); 
      Blynk.virtualWrite(V16, 1);    
    } else { 
      client.publish("hydroponic/TestRunning", "OFF"); 
      Blynk.virtualWrite(V16, 0);     
      }
  }
  if (temppumpRunning != pumpRunning) {
    temppumpRunning = pumpRunning;
    if (temppumpRunning){
      client.publish("hydroponic/pumpRunning", "ON"); 
      Blynk.virtualWrite(V17, 1);    
    } else { 
      client.publish("hydroponic/pumpRunning", "OFF"); 
      Blynk.virtualWrite(V17, 0);     
      }
  }
  if (menuoptCall.getCurrentValue() !=  tempString18) {
    tempString18 = menuoptCall.getCurrentValue();
    if (tempString18 = 0) { 
      client.publish("hydroponic/CalSelect", "EC"); 
      Blynk.virtualWrite(V22, 0);      
      }
    else {
      client.publish("hydroponic/CalSelect", "pH"); 
      Blynk.virtualWrite(V22, 1);   
    }
  }
  if ((temperature > 15) && (temperature < 50)) {
    dtostrf(temperature, 1, 2, tempString);
    if (strcmp(tempString, tempString12) != 0) {
      strcpy(tempString12, tempString);
      client.publish("hydroponic/temperature", tempString);
      Blynk.virtualWrite(V23, tempString);
    }
  }
  dtostrf(phValue, 1, 2, tempString);
  if (strcmp(tempString, tempString14) != 0) {
    strcpy(tempString14, tempString);
    client.publish("hydroponic/phValue", tempString);
    Blynk.virtualWrite(V24, tempString);
  }
  dtostrf(ecValue, 1, 2, tempString);
  if (strcmp(tempString, tempString15) != 0) {
    strcpy(tempString15, tempString);
    client.publish("hydroponic/ecValue", tempString);
    Blynk.virtualWrite(V25, tempString);
  }
  sysVoltage.toCharArray(tempString, 16);
  if (strcmp(tempString, tempString16) != 0) {
    strcpy(tempString16, tempString);
    client.publish("hydroponic/sysVoltage", tempString);
    Blynk.virtualWrite(V26, tempString);
  }
  str = menuCalStatus.getTextValue();
  str.toCharArray(tempString, 16);
  if (strcmp(tempString, tempString17) != 0) {
    strcpy(tempString17, tempString);
    client.publish("hydroponic/callstatus", tempString);
    Blynk.virtualWrite(V27, tempString);
  }

}
