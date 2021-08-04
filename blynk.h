

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  // Serial.print("V1 Slider value is: ");
  //Serial.println(pinValue);
  menuRunMin.setCurrentValue(pinValue);
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    menuIntervalMin.setCurrentValue(pinValue);
}
BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    menuMlMin.setCurrentValue(pinValue);

}
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    menuMl.setCurrentValue(pinValue);

}
BLYNK_WRITE(V5)
{
  double pinValue = param.asDouble(); // assigning incoming value from pin V1 to a variable
    pinValue = pinValue * 10;
    menuMinimumEC.setCurrentValue(pinValue);

}
BLYNK_WRITE(V6)
{
  double pinValue = param.asDouble(); // assigning incoming value from pin V1 to a variable
    pinValue = pinValue * 10;
    menuTargetEC.setCurrentValue(pinValue);
}
BLYNK_WRITE(V7)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    menuEndTime.setCurrentValue(pinValue);
}
BLYNK_WRITE(V8)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    menuStartTime.setCurrentValue(pinValue);

}
BLYNK_WRITE(V9)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    menuTestTime.setCurrentValue(pinValue);

}
BLYNK_WRITE(V10)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    menuTestDuration.setCurrentValue(pinValue);

}
BLYNK_WRITE(V11)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    menuMixDuration.setCurrentValue(pinValue);

}

BLYNK_WRITE(V12)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1) {
      menuAutoDose.setBoolean(true);
    } else if (pinValue == 0) {
      menuAutoDose.setBoolean(false); }
}

BLYNK_WRITE(V13)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1) {
      menuAuto.setBoolean(true);
    } else if (pinValue == 0) {
      menuAuto.setBoolean(false); }
}

BLYNK_WRITE(V14)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1) {
      runMix(40);
    } else if (pinValue == 0) {
      onStopAll(40); }
}

BLYNK_WRITE(V15)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1) {
      runDose(40);
    } else if (pinValue == 0) {
      onStopAll(40); }
}

BLYNK_WRITE(V16)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1) {
      runTest(40);
    } else if (pinValue == 0) {
      onStopAll(40); }
}

BLYNK_WRITE(V17)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1) {
      RunPump(40);
    } else if (pinValue == 0) {
      onStopAll(40); }
}

BLYNK_WRITE(V18)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1) {
      onStopAll(40);
    } 
}

BLYNK_WRITE(V19)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1) {
      onSaveSettings(40);
    } 
}

BLYNK_WRITE(V20)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1) {
      onReboot(40);
    }
}

BLYNK_WRITE(V21)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1) {
      menuoptCall.setCurrentValue(0);
    } else if (pinValue == 0) {
      menuoptCall.setCurrentValue(1); }
}

BLYNK_WRITE(V22)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    if (pinValue == 1) {
      calecph(40);
    }
}

void myBlynkEventsss()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  char BLtempString[16];
  String str;
  float BLtempTargetEC;
  float BLtempMinEC;

  dtostrf(menuRunMin.getCurrentValue(), 1, 0, BLtempString);
  if (strcmp(BLtempString, BLtempString7) != 0) {
    strcpy(BLtempString7, BLtempString);
    Blynk.virtualWrite(V1, BLtempString);
  }
  dtostrf(menuIntervalMin.getCurrentValue(), 1, 0, BLtempString);
  if (strcmp(BLtempString, BLtempString6) != 0) {
    strcpy(BLtempString6, BLtempString);
    Blynk.virtualWrite(V2, BLtempString);
  }
  dtostrf(menuMlMin.getCurrentValue(), 1, 0, BLtempString);
  if (strcmp(BLtempString, BLtempString10) != 0) {
    strcpy(BLtempString10, BLtempString);
    Blynk.virtualWrite(V3, BLtempString);
  }
  dtostrf(menuMl.getCurrentValue(), 1, 0, BLtempString);
  if (strcmp(BLtempString, BLtempString11) != 0) {
    strcpy(BLtempString11, BLtempString);
    Blynk.virtualWrite(V4, BLtempString);
  }
  BLtempMinEC = menuMinimumEC.getCurrentValue();
  dtostrf(BLtempMinEC/10, 1, 1, BLtempString);
  if (strcmp(BLtempString, BLtempString8) != 0) {
    strcpy(BLtempString8, BLtempString);
    Blynk.virtualWrite(V5, BLtempString);
  }
  BLtempTargetEC = menuTargetEC.getCurrentValue();
  dtostrf(BLtempTargetEC/10, 1, 1, BLtempString);
  if (strcmp(BLtempString, BLtempString9) != 0) {
    strcpy(BLtempString9, BLtempString);
    Blynk.virtualWrite(V6, BLtempString);
  }
  dtostrf(menuEndTime.getCurrentValue(), 1, 0, BLtempString);
  if (strcmp(BLtempString, BLtempString4) != 0) {
    strcpy(BLtempString4, BLtempString);
    Blynk.virtualWrite(V7, BLtempString);
  }
  dtostrf(menuStartTime.getCurrentValue(), 1, 0, BLtempString);
  if (strcmp(BLtempString, BLtempString5) != 0) {
    strcpy(BLtempString5, BLtempString);
    Blynk.virtualWrite(V8, BLtempString);
  }
 dtostrf(menuTestTime.getCurrentValue(), 1, 0, BLtempString);
  if (strcmp(BLtempString, BLtempString1) != 0) {
    strcpy(BLtempString1, BLtempString);
    Blynk.virtualWrite(V9, BLtempString);
  }
  dtostrf(menuTestDuration.getCurrentValue(), 1, 0, BLtempString);
  if (strcmp(BLtempString, BLtempString2) != 0) {
    strcpy(BLtempString2, BLtempString);
    Blynk.virtualWrite(V10, BLtempString);
  }
  dtostrf(menuMixDuration.getCurrentValue(), 1, 0, BLtempString);
  if (strcmp(BLtempString, BLtempString3) != 0) {
    strcpy(BLtempString3, BLtempString);
    Blynk.virtualWrite(V11, BLtempString);
  }
  if (menuAutoDose.getBoolean() != BLtempAutoDose) {
    BLtempAutoDose = menuAutoDose.getBoolean();
    if (tempAutoDose) {
    Blynk.virtualWrite(V12, 1); 
    } 
    else { Blynk.virtualWrite(V12, 0);}
  }
  if (menuAuto.getBoolean() != BLtempAuto) {
    BLtempAuto = menuAuto.getBoolean();
    if (tempAuto) {
    Blynk.virtualWrite(V13, 1);} 
    else { Blynk.virtualWrite(V13, 0);}
  }
  
  if (tempMixRunning != MixRunning) {
    BLtempMixRunning = MixRunning;
    if (tempMixRunning){Blynk.virtualWrite(V14, 1); } 
    else { Blynk.virtualWrite(V14, 0); } 
  }
  if (tempDoseRunning != DoseRunning) {
    BLtempDoseRunning = DoseRunning;
    if (tempDoseRunning){Blynk.virtualWrite(V15, 1); } 
    else { Blynk.virtualWrite(V15, 0); } 
  }  
  if (tempTestRunning != TestRunning) {
    BLtempTestRunning = TestRunning;
    if (tempTestRunning){Blynk.virtualWrite(V16, 1); } 
    else { Blynk.virtualWrite(V16, 0); } 
  }
  if (temppumpRunning != pumpRunning) {
    BLtemppumpRunning = pumpRunning;
    if (temppumpRunning){Blynk.virtualWrite(V17, 1); } 
    else { Blynk.virtualWrite(V17, 0); } 
  }
  if (menuoptCall.getCurrentValue() !=  BLtempString18) {
    BLtempString18 = menuoptCall.getCurrentValue();
    if (BLtempString18 = 0) { Blynk.virtualWrite(V21, 0); } 
    else {Blynk.virtualWrite(V21, 1); } 
  }
  str = menustatus.getTextValue();
  str.toCharArray(BLtempString, 16);
  if (strcmp(BLtempString, BLtempString13) != 0) {
    strcpy(BLtempString13, BLtempString);
    Blynk.virtualWrite(V0, BLtempString);  
  }
  if ((temperature > 15) && (temperature < 50)) {
    dtostrf(temperature, 1, 2, BLtempString);
    if (strcmp(BLtempString, BLtempString12) != 0) {
      strcpy(BLtempString12, BLtempString);
      Blynk.virtualWrite(V23, BLtempString); 
    }
  }
  dtostrf(phValue, 1, 2, BLtempString);
  if (strcmp(BLtempString, BLtempString14) != 0) {
    strcpy(BLtempString14, BLtempString);
    Blynk.virtualWrite(V24, BLtempString);
  }
  dtostrf(ecValue, 1, 2, BLtempString);
  if (strcmp(BLtempString, BLtempString15) != 0) {
    strcpy(BLtempString15, BLtempString);
    Blynk.virtualWrite(V25, BLtempString);
  }
  sysVoltage.toCharArray(BLtempString, 16);
  if (strcmp(BLtempString, BLtempString16) != 0) {
    strcpy(BLtempString16, BLtempString);
    Blynk.virtualWrite(V26, BLtempString);
  }
  
  str = menuCalStatus.getTextValue();
  str.toCharArray(BLtempString, 16);
  if (strcmp(BLtempString, BLtempString17) != 0) {
    strcpy(BLtempString17, BLtempString);
    Blynk.virtualWrite(V27, BLtempString);
  }
}