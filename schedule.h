 //https://www.arduinolibraries.info/libraries/esp32-time
#include "time.h" //https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/
// tok tahu perlu or not ---- #include "esp_system.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;
const int   daylightOffset_sec = 0;
struct tm timeinfo;
char stTime[51];

//timer https://www.instructables.com/Coding-Timers-and-Delays-in-Arduino/
unsigned long delayStart = 0; // the time the delay started
bool delayRunning = false; // true if still waiting for delay to finish
float duration = 0;
String sysVoltage;
      
//float  phValue,ecValue,temperature;
float minEC,targetEC;

//var utk schedule
bool newday = true;
bool pumpRunning = false;
bool TestRunning = false;
bool MixRunning = false;
bool DoseRunning = false;
int pumpStopTime;

int PumpRelay   = 25;
int DosingRelay = 26;
int Valve1Relay = 27;
int Valve2Relay = 14;

void relayOn(int relay) {
    digitalWrite(relay, LOW);
//    Serial.println("On Relay " + relay);
}

void relayOff(int relay) {
    digitalWrite(relay, HIGH);
//    Serial.println("OFF Relay " + relay);
}

void offAll(){
    relayOff(PumpRelay);
    relayOff(DosingRelay);
    relayOff(Valve1Relay);
    relayOff(Valve2Relay);

    pumpRunning = false;
    TestRunning = false;
    MixRunning = false;
    DoseRunning = false;

    pumpStopTime = (timeinfo.tm_hour*100+timeinfo.tm_min);
    menustatus.setTextValue("idle");
    delay(500);
}
void scheduleSetup() {
  
    pinMode(PumpRelay, OUTPUT);
    pinMode(DosingRelay,
    OUTPUT);
    pinMode(Valve1Relay, OUTPUT);
    pinMode(Valve2Relay, OUTPUT);
    offAll();
}

void timer(){
      if (delayRunning && ((millis() - delayStart) >= (duration*1000))) {
        delayRunning = false; // // prevent this code being run more then once
        Serial.println("Delay time's up");
        offAll();
      }
}

void Dosing() {
  float ml;
  float mlmin;

  char charVolts[5];

  if (DoseRunning) {
    
      //Serial.println((ads.readADC_SingleEnded(3)* 0.1875)/1000);
     
      //0.000719035)*11);
      //* 0.0217667844522968);
      //0.1875
      
      ml = menuMl.getCurrentValue();
      mlmin = menuMlMin.getCurrentValue();
      duration = ((ml / mlmin) * 60);

      relayOn(PumpRelay);
      relayOn(DosingRelay);
      Serial.println("Dosing Run ");
      
      menustatus.setTextValue("Dosing...      ");

      delayRunning = true;
      delayStart = millis();
  }
}

void readSensors(){
  temperature = readTemperature();
  if ((temperature > 15) && (temperature <50)) { 
    menutemperature.setCurrentValue(temperature); 
    }
  sysVoltage = String(((ads.readADC_SingleEnded(3) * 0.1875)/1000)*4.087837838);
  if (TestRunning){
        Serial.println("Reading sensors");
        menustatus.setTextValue("Reading sensors");
        dfrobotEcPh();
        menuPHEC.setTextValue(str_ph_ec.c_str(), false);
        menutemperature.setCurrentValue(temperature);
  }
}

void CALLBACK_FUNCTION runDose(int id) {
      offAll();

      DoseRunning = true;
      Dosing();
}

void CALLBACK_FUNCTION runMix(int id) {
  float sec;
      offAll();
      sec = menuMixDuration.getCurrentValue();
      duration = (sec * 60);

      relayOn(PumpRelay);
      Serial.println("Mixing");
      menustatus.setTextValue("Mixing...      ");

      MixRunning = true;
      delayRunning = true;
      delayStart = millis();          
}

void CALLBACK_FUNCTION runTest(int id) {
  float sec;

    if (!DoseRunning) {
     offAll(); 
    }
      
      sec = menuTestDuration.getCurrentValue();
      duration = (sec * 60);

      relayOn(PumpRelay);
      relayOn(Valve2Relay);
      Serial.println("Run Water Test");

      TestRunning = true;
      delayRunning = true;
      delayStart = millis();
      
}

void CALLBACK_FUNCTION onStopAll(int id) {
  offAll();
 } 
 
void CALLBACK_FUNCTION RunPump(int id) {
  float sec;
      offAll();

      sec = menuRunMin.getCurrentValue();
      duration = (sec * 60);

      relayOn(PumpRelay);
      relayOn(Valve1Relay);      
      Serial.println("Pump Running");
      menustatus.setTextValue("Pump Running   ");

      pumpRunning = true;
      delayRunning = true;
      delayStart = millis();
            
}

void printLocalTime(){
  char stTime[51];
  String tempStatus;
  
  if(!getLocalTime(&timeinfo)){
   Serial.println("Failed to obtain time");
  return;
  }
//  Serial.println(&timeinfo, "%H:%M:%S");
  strftime(stTime, 50, "%H:%M:%S", &timeinfo);

  tempStatus =  String(menustatus.getTextValue());
  if (tempStatus.substring(tempStatus.length() - 4, tempStatus.length()) == "idle") {
    tempStatus = (String(stTime) + "   idle");
    menustatus.setTextValue(tempStatus.c_str());
  }
  menuCurrentTime.setTimeFromString (stTime);
}

void setupNTP(){
  //Patut dalam setup
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}

void PumpSchedule(){
int ontime, offtime, now;
//  struct tm timeinfo; moved atas
  
  if(iConnected && !getLocalTime(&timeinfo)){
    Serial.println("Failed to update time");
  }
  if (timeinfo.tm_hour == 0) {
    newday = true;
  }

  Serial.print(">>> ");
  ontime = (menuStartTime.getCurrentValue()*100);
  offtime = (menuEndTime.getCurrentValue()*100);
  
  now = ((timeinfo.tm_hour*100)+(timeinfo.tm_min));

  if (menuAuto.getBoolean() == 1 && now>=ontime && now<=offtime) {
    //Pump run ikut schedule

    if (!pumpRunning && !TestRunning && !MixRunning && !DoseRunning){
      ontime = pumpStopTime + menuIntervalMin.getCurrentValue();
      if (now >= ontime){       
        RunPump(40); //auto stop after time set
        newday = false;
      } else if (newday) {
        RunPump(40);
        newday = false;
        }
    }
  }
}
void TestSchedule(){
int ontime, now;
//  struct tm timeinfo; moved atas
  
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to update time");
  }
  ontime = (menuTestTime.getCurrentValue()*100);
  now = (timeinfo.tm_hour*100)+(timeinfo.tm_min);

  //run 1 time @ o'clock
  if (menuAuto.getBoolean() == 1 && now == ontime) {
    //Pump run ikut schedule
    if (!TestRunning){
        runTest(40); //auto stop after time set
    }
  }
  if (menuAutoDose.getBoolean() == 1) {
     if (ecValue > 0.5 && ecValue < menuMinimumEC.getCurrentValue()/10){
        Serial.println("Auto Dosing Running");
        DoseRunning = true;
        Dosing();
        if (!TestRunning){
          runTest(40); //auto stop after time set
        }
     }
     if (ecValue >= menuTargetEC.getCurrentValue()/10){
        DoseRunning = false;
        //offAll();      
     }   
  }
}
