/*
 * forked from file DFRobot_PH_EC.ino
 *
 *  PH Calibration：
 *   enterph -> enter the PH calibration mode
 *   calph   -> calibrate with the standard buffer solution, two buffer solutions(4.0 and 7.0) will be automaticlly recognized
 *   exitph  -> save the calibrated parameters and exit from PH calibration mode
 *
 *  EC Calibration：
 *   enterec -> enter the EC calibration mode
 *   calec   -> calibrate with the standard buffer solution, two buffer solutions(1413us/cm and 12.88ms/cm) will be automaticlly recognized
 *   exitec  -> save the calibrated parameters and exit from EC calibration mode
 *
 */

#include <DFRobot_PH.h>
#include <DFRobot_EC.h>
#include <EEPROM.h>
//Temperature
#include <OneWire.h> //Temperature
//ADS1115
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;
//#include <Wire.h>

int DS18S20_Pin = 4; //DS18S20 Signal pin on digital pin 4
OneWire ds(DS18S20_Pin);   //Temperature chip i/o

//int input1 = 39;
//int input2 = 34;
//int input3 = 35;

int relay1 = 18;
int relay2 = 19;

#define PH_PIN 0
#define EC_PIN 1

float  voltagePH,voltageEC,phValue,ecValue,temperature = 25;
DFRobot_PH ph;
DFRobot_EC ec;

String str_ph_ec,strph,strec;

// oee void setup()
void dfrobot_ph_ec()

{
    ads.begin();
    ph.begin();
    ec.begin();
    
}

float readTemperature()
{
  //returns the temperature from one DS18S20 in DEG Celsius 
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  } 
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end
  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad   
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  ds.reset_search();   
  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  if (TemperatureSum > 15 && TemperatureSum < 50) {
    return TemperatureSum;   
  } else {
    //return;
  }
}

int i = 0;
bool readSerial(char result[]){
    while(Serial.available() > 0){
        char inChar = Serial.read();
        if(inChar == '\n'){
             result[i] = '\0';
             Serial.flush();
             i=0;
             return true;
        }
        if(inChar != '\r'){
             result[i] = inChar;
             i++;
        }
        delay(1);
    }
    return false;
}

//Oee void loop()
void dfrobotEcPh()
{
    float tempPH;
    char cmd[10];
    static unsigned long timepoint = millis();
    if(millis()-timepoint>1000U){                            //time interval: 1s
        timepoint = millis();
        temperature = readTemperature();                   // read your temperature sensor to execute temperature compensation
        voltagePH = (ads.readADC_SingleEnded(PH_PIN) * 0.1875);
        tempPH = ph.readPH(voltagePH,temperature);       // convert voltage to pH with temperature compensation
        if(tempPH <= 9) { phValue = tempPH; }
        Serial.print("Temp:");
        Serial.print(temperature,2);
        Serial.print(", voltage:");
        Serial.print(voltagePH,2);
        Serial.print(", pH:");
        Serial.print(phValue,2);
        
        voltageEC = (ads.readADC_SingleEnded(EC_PIN) * 0.1875);      
        ecValue = ec.readEC(voltageEC,temperature);       // convert voltage to EC with temperature compensation
        
        str_ph_ec = "pH" + String(phValue) + " " + String(ecValue) + "ms/cm";
        
        Serial.print(", voltage:");
        Serial.print(voltageEC,2);
        Serial.print(", EC:");
        Serial.print(ecValue,2);
        Serial.println("ms/cm");
    }
    if(readSerial(cmd)){
        strupr(cmd);
        if(strstr(cmd,"PH")){
            ph.calibration(voltagePH,temperature,cmd);       //PH calibration process by Serail CMD
        }
        if(strstr(cmd,"EC")){
            ec.calibration(voltageEC,temperature,cmd);       //EC calibration process by Serail CMD
        }
    }
}
