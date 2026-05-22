// -- Prototipos ---
void eepromRead();
void eepromWrite();
void eepromWriteInitAndAlarms();
void eepromChanged();
void eepromInitialize(byte initType);
void eepromSetVariables();
void checkAlarms();
void checkRecurSkipped();
bool checkRecur(int i);
void saveRecNext(int i);
bool checkNonRecur(int i);
void alarmInterrupted();
void alarmEnded();
void initActions();
void initAction(int value);
void displayTime(bool updateDisplay);
void displayTimeSmall(byte charWidth, byte column, unsigned char c);
void displayCharAltFont(byte font, int startColumn, unsigned char c, bool continuePrevColumn);
void displayTimeColon();
void displayPixelModes();
void displayAnnounceMode(byte displayMode);
void displayEndAnnounceMode();
void displayPM();
void displayAlarmTime(String message);
void displayAnimPacMan(bool reverse);
void addChipidToUI_SSID();
void setupWifiConnection();
void getTimeFromServer();
void gtfsWaitForTime();
void encoderMenu();
int  encoderMenuSelect(bool firstCall);
void encoderDisplayMenuItem(int menuItem);
void encoderDisplayMenuItemShort(int menuItem);
void encoderSetAlarm0();
void encoderSetLedBrightness();
void encoderSetLedBrightnessCallback(int value);
void encoderSetNeoBacklight();
void encoderSetNeoBacklightCallback(int value);
void encoderSyncTime();
void encoderSetSleepMode();
void encoderSetSuspendMode();
void encoderSetdisplayInfo();
void encoderSetdisplayInfoDelay();
void encoderDisplayInfo();
void encoderSetTime();
void encoderSetDate();
void encoderTimeDisplayMode();
void encoderTimeDisplayModeUpdate(int item);
void encoderUIminimal();
void encoderSave();
void encoderRestart();
void encoderAnnounce();
void encoderAnnounceDisplay(int item);
void encoderDisplayItem(String item);
void encoderDisplayItemLong(String item);
void encoderDisplayItemOnOff(String item);
int  encoderSetValue(int value, int minValue, int maxValue, int startPos, byte inputType, callBackFunctionInt callbackFunction, unsigned long delayTime);
int  readRotEnc();
int  readRotEncSwitch();
void setupMatrix();
void readCurrentTime();
int getPMhour();
bool checkLeapYear(int the_year);
byte getDaysInMonth(int year, int month);
void checkDST();
String fillZero(int value);
String fillSpace(int value, int valLength);
void playBuzzer(int numOfBuzz);
void playTone(int freq, int duration, int postDelay);
void testBuzzer();
void endSleepMode(bool resetInfoTime);
void startSleepMode();
int  delayAndCheckEnc(unsigned int delayTime);
void checkLightSensor();
int handleEvents();
int  checkAndHandleEvents();
int  delayAndHandle(int delayTime);
uint16_t getVersionChecksum();
void displayChar(int x, unsigned int fontChar[], int lowHigh, bool fill, bool reverse);
void displayIntro(String message);
void displayClock(bool reverse);
void displayMessageOutLoop(int xOffset, String message, bool reverse);
void displayMessageShort(String message);
void displayMessage(String message);
void UI_sw_scanSSIDs();
void UI_filesystem();
void UI_setup();
uint16_t UI_addSlider(const char *label, int value, int minValue, int maxValue, ControlColor color, uint16_t parentControl, void (*callback)(Control *, int));
void UI_addControlsTop();
void UI_addControlsMain();
void UI_addControlsAlarms();
void UI_addControlsAnnounce();
void UI_rereadTop();
void UI_rereadMain();
void UI_rereadLS();
void UI_rereadAlarms();
void UI_rereadAnnounce();
void UI_rereadTab(int tabCode);
String UI_getInfo(int type);
void UI_callTab(Control *sender, int type);
void UI_callButton(Control *sender, int type);
void UI_callText(Control *sender, int type);
void UI_callNumber(Control *sender, int type);
int UI_validateNum(int minVal, int maxVal, String value);
void UI_callOption(Control *sender, int value);
void UI_callSwitch(Control *sender, int value);
void UI_callPad(Control *sender, int value);
void debugPrintAlarmlist();
void debugSetAlarms();
void debugValidateTime(byte &vtHour, byte &vtMin, int &vtSec);
void UI_sw_writeEEPROM();
void UI_sw_resetEEPROM();
bool UI_sw_readEEPROM();
void UI_sw_callButton(Control *sender, int type);
String UI_sw_fillAsterisk(String value, byte valLength);
void UI_sw_updatePassword();
void UI_sw_callText(Control *sender, int type);
void UI_sw_callSelect(Control *sender, int type);
void UI_sw_setupWifi(char * UI_title);

/*
***************************************************************************
  ebc_alarmclock - eeprom functions
***************************************************************************
  last update 20201112 by ericBcreator

  note: eeprom structure:
  0   UI_sw_EEPROM
  +   eepromInit
  +   alarmList[x]  array of alarms, x = 0 to maxAlarms (set by code)
***************************************************************************
*/

void eepromRead() {
  int startAddress = sizeof(UI_sw_EEPROM) + 1;
  int address = 0;
  
  EEPROM.begin((eepromBufSize - startAddress));
  EEPROM.get(startAddress + address, eepromInit);

  if (eepromInit.idStart == '$' && eepromInit.idEnd == '*') {   // if first char is $ and last is * assume it is initialized
    address = sizeof(eepromInit);
    
    displayInfo = eepromInit.displayInfo;
    displayInfoInterval = eepromInit.displayInfoInterval;
    suspendMode = eepromInit.suspendMode;
    sleepMode = eepromInit.sleepMode;
    sleepDelay = eepromInit.sleepDelay;
    displayDaylight = eepromInit.displayDaylight;
    ledBrightness = eepromInit.ledBrightness;
    ledBrightnessManualSet = eepromInit.ledBrightnessManualSet;
    ampmMode = eepromInit.ampmMode;
    timeDisplayMode = eepromInit.timeDisplayMode;
    fileSystemInitialized = eepromInit.fileSystemInitialized;
    versionChecksum = eepromInit.versionChecksum;
    neoBacklight = eepromInit.neoBacklight;    
    timeZone = eepromInit.timeZone;
    DSTmode = eepromInit.DSTmode;
    BME280_display = eepromInit.BME280_display;
    BME280_fahrenheid = eepromInit.BME280_fahrenheid;
    BME280_tempTimeSwap = eepromInit.BME280_tempTimeSwap;
  
    for (int i = 0; i < maxAlarms; i++) {      
      EEPROM.get(startAddress + address, alarmList[i]);
      address += sizeof(alarmList[0]);
    }

    matrix.setIntensity(ledBrightness);

    DEBUGPRINTLN();
    DEBUGPRINTLN("**EEPROM read");
    DEBUGPRINTLN("  Version checksum       " + (String) versionChecksum);
    DEBUGPRINTLN("  UI file initialized    " + (String) fileSystemInitialized);
    DEBUGPRINTLN("  Timezone               " + (String) timeZone);
    DEBUGPRINTLN("  DST mode               " + (String) DSTmode);
    DEBUGPRINTLN("  Time display mode      " + (String) timeDisplayMode);
    DEBUGPRINTLN("  AM/PM mode             " + (String) (ampmMode               ? msgOn : msgOff));    
    DEBUGPRINTLN("  Display info           " + (String) (displayInfo            ? msgOn : msgOff));
    DEBUGPRINTLN("  Display info interval  " + (String) displayInfoInterval);
    DEBUGPRINTLN("  Daylight info          " + (String) (displayDaylight        ? msgOn : msgOff));
    DEBUGPRINTLN("  BME280 info            " + (String) (BME280_display         ? msgOn : msgOff));
    DEBUGPRINTLN("  BME280 fahrenheid      " + (String) (BME280_fahrenheid      ? msgOn : msgOff));
    DEBUGPRINTLN("  BME280 time/temp swap  " + (String) (BME280_tempTimeSwap    ? msgOn : msgOff));
    DEBUGPRINTLN("  Suspend mode           " + (String) (suspendMode            ? msgOn : msgOff));
    DEBUGPRINTLN("  Sleep mode             " + (String) (sleepMode              ? msgOn : msgOff));
    DEBUGPRINTLN("  Sleep delay            " + (String) sleepDelay);
    DEBUGPRINTLN("  Brightness             " + (String) ledBrightness);
    DEBUGPRINTLN("  Brightness manual set  " + (String) (ledBrightnessManualSet ? msgOn : msgOff));    
    DEBUGPRINTLN("  Neopixel backlight     " + (String) neoBacklight);
    DEBUGPRINTLN("**");
    
  } else {                                                       // if not, initialize
    DEBUGPRINTLN();
    DEBUGPRINTLN("EEPROM identifiers failed " + (String) eepromInit.idStart + " " + (String) eepromInit.idEnd + ", initializing");
    eepromInitialize(0);
  }
}

void eepromWrite() {
  #ifdef DEBUG_NO_WRITE_ALARMS_TO_EEPROM
    DEBUGPRINTLN("EEPROM writing skipped");
    eepromSavedTime = millis();
    return;
  #endif

  matrix.drawPixel(31, 6, HIGH);
  int startAddress = sizeof(UI_sw_EEPROM) + 1;
  EEPROM.begin((eepromBufSize - startAddress));
  eepromWriteInitAndAlarms();  
  EEPROM.commit();

  eepromValChanged = false;
  eepromSavedTime = millis();

  matrix.drawPixel(31, 6, LOW);
  DEBUGPRINTLN("EEPROM written");  
  //playBuzzer(21);
}

void eepromWriteInitAndAlarms() {
  int startAddress = sizeof(UI_sw_EEPROM) + 1;
  int address = 0;
  eepromSetVariables();
  EEPROM.put(startAddress + address, eepromInit);
  
  address = sizeof(eepromInit);
  for (int i = 0; i < maxAlarms; i++) {
    EEPROM.put(startAddress + address, alarmList[i]);
    address += sizeof(alarmList[0]);      
  }
}

void eepromChanged() {
  eepromValChanged = true;
  eepromSavedTime = millis();
}

void eepromInitialize(byte initType) {                    // 0 is all, 1 is only alarms  
  int startAddress = sizeof(UI_sw_EEPROM) + 1;
  int bufSize = eepromBufSize - startAddress;

  if (initType == 1) {
    startAddress += sizeof(eepromInit);
    bufSize -= sizeof(eepromInit);
  }

  EEPROM.begin(bufSize);
  for (int i = 0 ; i < bufSize; i++)
    EEPROM.write(startAddress + i, 0);

  if (initType == 0) {
    initCustomSettings();
    eepromSetVariables();  
    EEPROM.put(startAddress, eepromInit);  
  }
  
  EEPROM.commit();    
  
  DEBUGPRINT("EEPROM initialized");

  if (initType == 1)
    DEBUGPRINTLN(" - only alarms");
  else
    DEBUGPRINTLN();
}

void eepromSetVariables() {
  eepromInit.idStart = '$';
  eepromInit.displayInfo = displayInfo;
  eepromInit.displayInfoInterval = displayInfoInterval;
  eepromInit.suspendMode = suspendMode;
  eepromInit.sleepMode = sleepMode;
  eepromInit.sleepDelay = sleepDelay;
  eepromInit.displayDaylight = displayDaylight;
  eepromInit.ledBrightness = ledBrightness;
  eepromInit.ledBrightnessManualSet = ledBrightnessManualSet;
  eepromInit.ampmMode = ampmMode;
  eepromInit.timeDisplayMode = timeDisplayMode;
  eepromInit.fileSystemInitialized = fileSystemInitialized;
  eepromInit.versionChecksum = versionChecksum;
  eepromInit.neoBacklight = neoBacklight;
  eepromInit.timeZone = timeZone;
  eepromInit.DSTmode = DSTmode;
  eepromInit.BME280_display = BME280_display;
  eepromInit.BME280_fahrenheid = BME280_fahrenheid;
  eepromInit.BME280_tempTimeSwap = BME280_tempTimeSwap;
  eepromInit.idEnd = '*';
}

/*
***************************************************************************
  ebc_alarmclock - alarm functions
***************************************************************************
  last update 20210319 by ericBcreator
***************************************************************************
*/

void checkAlarms() {
  static unsigned long alarmTriggerTime = 0;
  static unsigned long prevBuzzerTime = 0;
  static unsigned long prevBlinkTime = 0;
  static bool alarmBlink = false;
  bool alarmToTrigger = false;
  bool firstAlarmSet = false;

  // if the alarm is not already triggered, check if an alarm is ready to trigger
  if (!alarmTriggered) {                          // check for a newly activated alarm when the alarm is not already triggered 
    if (!forceFirstAlarm) {                       // and not forced to trigger
      for (int i = 0; i < maxAlarms; i++) {
        if (alarmList[i].active) {                // only check active alarms                      
          if (alarmList[i].recurringType > 0 && alarmList[i].recurringInterval > 0) {
            if (alarmRecTriggered[i]) {           // recurring: 1st trigger normal, then check for recurring
              alarmToTrigger = checkRecur(i);
              if (alarmToTrigger)
                alarmRepeat = true;
            } else 
              alarmToTrigger = checkNonRecur(i);
              
            if (alarmToTrigger) {
              saveRecNext(i);
              alarmRecTriggered[i] = true;
            }
            
          } else 
            alarmToTrigger = checkNonRecur(i);
  
          if (alarmToTrigger) {
            if (alarmSuspended || suspendMode)    // do not trigger if in suspendMode or alarmSuspended (active when using the encoder)
              DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   suspended at " + timeString);
            else {
              if (!firstAlarmSet) {               // loop through all alarms but only respond to the 1st triggered
                alarmNum = i;
                firstAlarmSet = true;
              }
            }
          }
        }
      }
    }

    if (forceFirstAlarm) {
      if (alarmList[alarmNum].duration == 666 || alarmList[alarmNum].duration == 777) {
        displayTime(true); // FIX: error: too few arguments to function 'void displayTime(bool)'
        displayAnimPacMan(true); // FIX: error: too few arguments to function 'void displayAnimPacMan(bool)'
        if (alarmList[alarmNum].duration == 666)
          delayAndCheckEnc(250);
        displayAnimPacMan(true);
        
        if (alarmList[alarmNum].duration == 666)
          forceFirstAlarm = false;
        else
          return;
      }
    }

    if (firstAlarmSet || forceFirstAlarm) {     // 1st alarm activated or forced      
      if (sleepMode) {
        if (sleeping)
          endSleepMode(true);
        else
          sleepStartTime = millis();
      }

      initAction(alarmNum);

      alarmTriggered = true;
      alarmTriggerTime = millis();
      alarmBlink = false;
      forceFirstAlarm = false;
      displayInfoStartTime = millis();

      displayClock(false);

      if (alarmRepeat)
        DEBUGPRINT("Recurring: ");

      readCurrentTime();
      DEBUGPRINTLN("Alarm " + fillSpace(alarmNum,2) + "   started at " + timeString);

      if (alarmList[alarmNum].bright)               // if set, turn leds bright
        matrix.setIntensity(ledBrightnessAlarm);

      switch (alarmList[alarmNum].actionType) {     
        case actionGPIOhigh:                        // set GPIO pin high
          digitalWrite(alarmList[alarmNum].actionNum, HIGH);
          DEBUGPRINTLN("           Pin " + (String) alarmList[alarmNum].actionNum + " set high");
          break;
        case actionGPIOlow:                         // set GPIO pin low
          digitalWrite(alarmList[alarmNum].actionNum, LOW);
          DEBUGPRINTLN("           Pin " + (String) alarmList[alarmNum].actionNum + " set low");
          break;

        #ifdef ENABLE_NEOPIXELS
          case actionNeoWUL:
          case actionNeoWULchain:
            neopixelsFade(0, true);                 // 1st call neopixel wake up light
            DEBUGPRINTLN("           Neopixels wake up light started");
            break;

          case actionNeoOn:
            neopixelsPresetOn(alarmList[alarmNum].actionNum);  // neopixels on, actionNum refers to neoPalette
            DEBUGPRINTLN("           Neopixels on: palette " + (String) alarmList[alarmNum].actionNum);
            break;

          case actionNeoOff:
            neopixelsOff();                         // neopixels off
            DEBUGPRINTLN("           Neopixels off");
            break;
            
        #endif
      }

      // for gpio action types only play the buzzer at the start
      if (alarmList[alarmNum].actionType == actionGPIOhigh || alarmList[alarmNum].actionType == actionGPIOlow)
        playBuzzer(alarmList[alarmNum].buzzer);
      
      if (strlen(alarmList[alarmNum].message) > 0)
        alarmMessage = true;      
    }
  }

  // alarm triggered loop
    
  if (alarmTriggered) {                                               // loop code
    if (alarmList[alarmNum].duration == 666) {
      displayTime(true); // FIX: error: too few arguments to function 'void displayTime(bool)'
      displayAnimPacMan(true); // FIX: error: too few arguments to function 'void displayAnimPacMan(bool)'
      UI_alarmToInterrupt = true;
    }
    
    if (!alarmMessage && alarmList[alarmNum].fx == 1) {
      if ((millis() - prevBlinkTime) >= 500) {                        // blink every 500 ms
        alarmBlink = !alarmBlink;
        prevBlinkTime = millis();

        if (alarmBlink) {          
          displayTime(true); // FIX: error: too few arguments to function 'void displayTime(bool)'
		  
    		  if (alarmList[alarmNum].buzzer)                             // sync the buzzer with the blinking
    		    prevBuzzerTime = 0;

          #ifdef ENABLE_NEOPIXELS                                     // blink neopixels if set
            if (alarmList[alarmNum].actionType == actionNeoOn)
              neopixelsPresetOn(alarmList[alarmNum].actionNum);
          #endif
        } else {
          matrix.fillScreen(LOW);
          matrix.write();

          #ifdef ENABLE_NEOPIXELS
            if (alarmList[alarmNum].actionType == actionNeoOn);
              neopixelsOff();
              neopixelsPresetOn(neoBacklight);
          #endif
        }
      }
    }

    if (alarmTriggered) {                                                                           // check if alarm is still triggered
      if (alarmList[alarmNum].actionType == 0 || alarmList[alarmNum].actionType == actionNeoOn) {   // don't play the buzzer in the loop for actions
        if (alarmList[alarmNum].buzzer > 0 && !alarmMessage) {                                      // play the buzzer every other sec when it is set
          if ((millis() - prevBuzzerTime) >= 2000) {                                                // and there is no message set
            prevBuzzerTime = millis();
            playBuzzer(alarmList[alarmNum].buzzer);
          }
        }
      }
    }

    #ifdef ENABLE_NEOPIXELS
      switch (alarmList[alarmNum].actionType) {
        case actionNeoWUL:
        case actionNeoWULchain:
          neopixelsFade(0, false); break;                       // loop neopixel wake up light
      }
    #endif

    if (alarmMessage)
      displayMessage((alarmRepeat ? msgRecurring : "") + fillZero(timeData.tm_hour) + ":" + fillZero(timeData.tm_min) + ":" + fillZero(timeData.tm_sec) + " " + alarmList[alarmNum].message);

    if ((millis() - alarmTriggerTime) >= (alarmList[alarmNum].duration * 1000)) {
      DEBUGPRINTLN("Alarm " + fillSpace(alarmNum, 2)+ "   ended   at " + timeString);
      alarmEnded();
    }

    if (UI_alarmToInterrupt) {
      alarmInterrupted();
      UI_alarmToInterrupt = false;
    }
  }
}

//
// functions
//

void checkRecurSkipped () {
  for (int i = 0; i < maxAlarms; i++) {      
    if (alarmList[i].active && alarmList[i].recurringType > 0 && alarmList[i].recurringInterval > 0) {
      if (alarmRecTriggered[i]) {
        if (checkRecur(i))
          saveRecNext(i);
      } else if (checkNonRecur(i)) {
        saveRecNext(i);
        alarmRecTriggered[i] = true;
      }
    }
  }
}

bool checkRecur(int i) {
  if (currentTime >= alarmRecNext[i])
    return true;
  else
    return false;
}

void saveRecNext(int i) {  
  switch (alarmList[i].recurringType) {
    case 1: // sec
      alarmRecNext[i] = currentTime + alarmList[i].recurringInterval; 
      break;
      
    case 2: // min
      alarmRecNext[i] = currentTime + (alarmList[i].recurringInterval * 60);
      break;
      
    case 3: // hour
      alarmRecNext[i] = currentTime + (alarmList[i].recurringInterval * 3600); 
      break;
      
    case 4: // day
      alarmRecNext[i] = currentTime + (alarmList[i].recurringInterval * 24 * 3600); 
      break;
      
    case 5: // month
      byte daysInMonth = 0;
      byte month = timeData.tm_mon + 1;     // convert 0-11 to 1-12
      alarmRecNext[i] = currentTime;
            
      for (int j = 0; j < alarmList[i].recurringInterval; j++) {
        daysInMonth = getDaysInMonth(timeData.tm_year, month);
        alarmRecNext[i] += (daysInMonth * 24 * 3600);
        month++;
        if (month > 12)
          month = 1;
      }
      break;
  }

  DEBUGPRINT("Alarm " + fillSpace(i, 2) + "   current time " + timeString);
  DEBUGPRINTLN(", recurring at "+ (String) alarmRecNext[i]);
}

bool checkNonRecur(int i) {
  static int prevAlarm = 99;
  static unsigned long prevTrigTime;
  bool alarmToTrigger = false;
    
  if (timeData.tm_hour == alarmList[i].hour && timeData.tm_min == alarmList[i].min && timeData.tm_sec == alarmList[i].sec) {   // hour, min and sec match
    
    if (prevAlarm != i || (millis() - prevTrigTime) > 1000) {                               // prevent checking the same alarm at the same sec
      
      alarmToTrigger = true;
      prevAlarm = i;
      prevTrigTime = millis();
      
      if (alarmList[i].month > 0 && timeData.tm_mon != (alarmList[i].month - 1)) {          // month set, no match. Note: timeDate.tm_mon ranges 0 to 11 !!
        alarmToTrigger = false;
        DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   no match for the month");
      }
    
      if (alarmList[i].day > 0 && timeData.tm_mday != alarmList[i].day) {                   // day set, no match
        alarmToTrigger = false;
        DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   no match for the day");
      }
    
      if (alarmList[i].weekDay > 0 && timeData.tm_wday != (alarmList[i].weekDay - 1)) {     // weekday set, no match
        alarmToTrigger = false;
        DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   no match for the day of the week");
      }

      if (alarmList[i].weekDays && alarmList[i].weekendDays) {                              // if both weekDays and weekendDays are set then don't check each
        DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   triggers week and weekend days");
      } else {
        if (alarmList[i].weekDays)
          if (timeData.tm_wday == 0 || timeData.tm_wday == 6) {                             // weekdays set, no match
            alarmToTrigger = false;
            DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   no match for weekdays");
          }
        
        if (alarmList[i].weekendDays)
          if (timeData.tm_wday != 0 && timeData.tm_wday != 6) {                             // weekenddays set, no match
            alarmToTrigger = false;
            DEBUGPRINTLN("Alarm " + fillSpace(i, 2) + "   no match for weekenddays");
          }
      }
    }
  }

  return alarmToTrigger;
}

void alarmInterrupted() {
  alarmIsInterrupted = true;
  alarmEnded();
  alarmIsInterrupted = false;
  DEBUGPRINTLN("Alarm " + fillSpace(alarmNum, 2) + "   interrupted at " + timeString);
  displayMessage(msgAlarmOff);
}

void alarmEnded() {
  #ifndef NO_BUZZER
    #ifdef ESP32
      ledcWrite(0, LOW);
      delayAndCheckEnc(50);
      ledcDetachPin(PIN_BUZZER);
    #else
      noTone(PIN_BUZZER);
    #endif
    pinMode(PIN_BUZZER, INPUT);
  #endif

  displayClock(false);
    
  alarmTriggered = false;
  alarmMessage = false;
  alarmRepeat = false;

  if (alarmList[alarmNum].bright)                 // if set, reset led brightness
    matrix.setIntensity(ledBrightness);

  if (alarmList[alarmNum].duration > 0 || alarmIsInterrupted) {    // if duration is > 0 or the alarm is interrupted, reset gpio or neopixels otherwise keep it in the current state
    switch (alarmList[alarmNum].actionType) {     // reset the GPIO pin or turn the neopixels off
      case actionGPIOhigh:
        digitalWrite(alarmList[alarmNum].actionNum, LOW);
        DEBUGPRINTLN("           Pin " + (String) alarmList[alarmNum].actionNum + " set low");
        break;
      case actionGPIOlow:
        digitalWrite(alarmList[alarmNum].actionNum, HIGH);
        DEBUGPRINTLN("           Pin " + (String) alarmList[alarmNum].actionNum + " set high");
        break;
  
      #ifdef ENABLE_NEOPIXELS
        case actionNeoWUL:
        case actionNeoWULchain:
        case actionNeoOn:
          neopixelsOff();
          DEBUGPRINTLN("           Neopixels off");
          neopixelsPresetOn(neoBacklight);
          break;
      #endif
    }
  }
  
  if (alarmList[alarmNum].actionType)       // play the buzzer again at the end
    playBuzzer(alarmList[alarmNum].buzzer);

  if (sleepMode)
    sleepStartTime = millis();
  readCurrentTime();
  
  displayInfoStartTime = millis();
  
  if (!alarmIsInterrupted) {                // neopixel wakeup light chain next alarm, only chain when the alarm is not interrupted
    if (alarmList[alarmNum].actionType == actionNeoWULchain) {
      forceFirstAlarm = true;
      alarmNum++; 
      if (alarmNum == maxAlarms)
        alarmNum = 0;
      DEBUGPRINTLN("           Chaining next alarm " + (String) alarmNum);
    }
  }
}

void initActions() {  
  for (int i = 0; i < maxAlarms; i++)  
    initAction(i);  
}

void initAction(int value) {    
  if (alarmList[value].active || forceFirstAlarm) {
    if (alarmList[value].actionType == actionGPIOhigh || alarmList[value].actionType == actionGPIOlow || forceFirstAlarm) {
      DEBUGPRINTLN("Init action " + (String) alarmList[value].actionNum);
      switch (alarmList[value].actionNum) {
        case PIN_CS: case PIN_BUZZER: case RE_PINA: case RE_PINB: case RE_SWITCH: case PIN_LS: 
          #ifdef ENABLE_PIR
            case PIN_PIR: 
          #endif
          #ifdef ENABLE_NEOPIXELS
            case PIN_NEOPIXELS:
          #endif
        
          DEBUGPRINTLN("** Pin " + fillZero(alarmList[value].actionNum) + " in use by one of the defined pins, skipped");
          break;
          
        default:  
          pinMode(alarmList[value].actionNum, OUTPUT);
          DEBUGPRINTLN("Pin " + fillZero(alarmList[value].actionNum) + "     set to output");
          
          switch (alarmList[value].actionType) {
            case actionGPIOhigh:
              digitalWrite(alarmList[value].actionNum, LOW);
              DEBUGPRINTLN("Pin " + fillZero(alarmList[value].actionNum) + "     set low");
              break;
            case actionGPIOlow:
              digitalWrite(alarmList[value].actionNum, HIGH);
              DEBUGPRINTLN("Pin " + fillZero(alarmList[value].actionNum) + "     set high");
              break;
          }
      }
    }
  }
}
/*
***************************************************************************
  ebc_alarmclock - display functions
***************************************************************************
  last update 20210320 by ericBcreator
***************************************************************************
*/

void displayTime(bool updateDisplay) {
  static unsigned long startTime = millis();
  char timeArray[7], bmeArray[7];
  int xOffset = 0, displayTemp;
  activeTimeDisplayMode = timeDisplayMode;

  if (updateDisplay)
    readCurrentTime();

  if (refreshTempSwapTime > 0 && !animationStarted) {               // after sleepmode ended, reset the time/temp swap start time
    if (refreshTempSwapTime == 2)                                   // reset to time display when changing the displaymode from the ESPUI
      BME280_tempTimeSwitch = false;
    refreshTempSwapTime = 0;
    refreshTimeDisplay = true;
    startTime = millis();
  }
 
  #ifndef NO_WIFI                                                   // only resync when wifi and timeserver are enabled
    #ifndef DEBUG_NO_TIMESERVER
      if (timeData.tm_hour == 3 && timeData.tm_min == 0 && timeData.tm_sec == 0) {         // @EB-todo reread the time every day at 3:00 AM to synchronize and check DST
        if (!timeManualSet) {
          DEBUGPRINT("3 AM, resyncing the time");
          getTimeFromServer();
        }
      }
    #endif
  #endif

  if (refreshTimeDisplay) {
    matrix.fillScreen(LOW);
    refreshTimeDisplay = false;
    
    if (!animationStarted)
      updateDisplay = true;

//    if (!timeManualSet) {                    // @EB-todo: trying to capture the exact start of the second: not working, time info from the time server
//                                             //           is second based, so there is no way of knowing the exact start of the second
//      int tmpSec = timeData.tm_sec;
//      
//      while (timeData.tm_sec == tmpSec) {    // when refreshing, wait until a new second has started before continuing (to sync multiple clocks)
//        readCurrentTime();
//        delay(20);
//      };
//    }
  }

  String (fillZero(getPMhour()) + fillZero(timeData.tm_min) + fillZero(timeData.tm_sec)).toCharArray(timeArray, 7);      

  #ifdef ENABLE_BME280  
    if (LED_NUM_OF_HOR_DISPLAYS < 8) {                              // don't alternate when there are 8 (or more) matrices
      if (BME280_tempTimeSwap) {                                    // alternate between time and temp display if selected
        if (BME280_tempTimeSwitch) {
          if (millis() - startTime > BME280_dispTempInterval) {
            BME280_tempTimeSwitch = false;
  //        if (!animationStarted) displayAnimPacMan(true);         // @EB-todo
            matrix.fillScreen(LOW);
            startTime = millis();
          }
        } else if (millis() - startTime > BME280_dispTimeInterval) {
          BME280_tempTimeSwitch = true;
  //      if (!animationStarted) displayAnimPacMan();               // @EB-todo
          matrix.fillScreen(LOW);
          startTime = millis();
        }
  
        if (BME280_tempTimeSwitch)
          activeTimeDisplayMode = 6;
      }
    }
  #endif
  
  switch(activeTimeDisplayMode) {
    case 0: // HH:MM
      for (int i = 0; i < 4; i++)
        matrix.drawChar(charPos[i], yOffset, timeArray[i], HIGH, LOW, 1);
      break;

    case 1: // HH:MM big font
      for (int i = 0; i < 4; i++)
        displayCharAltFont(0, charPosBigFont[i], timeArray[i], true); // FIX: error: too few arguments to function 'void displayCharAltFont(byte, int, unsigned char, bool)'
      break;
      
    case 2: // HH MM ss
      for (int i = 0; i < 4; i++) {
        if (ampmMode) {
          if (i > 0 || timeArray[i] > 48)
            matrix.drawChar(charPosSec[i], yOffset, timeArray[i], HIGH, LOW, 1);
        } else
          matrix.drawChar(charPosSec[i], yOffset, timeArray[i], HIGH, LOW, 1);
      }
              
      for (int i = 4; i < 6; i++)
        displayTimeSmall(3, charPosSec[i], timeArray[i]);
      break;

    case 3: // HH MM ap
      for (int i = 0; i < 4; i++) {
        if (ampmMode) {
          if (i > 0 || timeArray[i] > 48)
            matrix.drawChar(charPosSec[i], yOffset, timeArray[i], HIGH, LOW, 1);
        } else
          matrix.drawChar(charPosSec[i], yOffset, timeArray[i], HIGH, LOW, 1);
      }
              
      if (pm) displayTimeSmall(3, charPosSec[4], 59); // P
      else    displayTimeSmall(3, charPosSec[4], 58); // A
      displayTimeSmall(3, charPosSec[5], 60);         // M
      break;

    case 4: // hh mm ss
      for (int i = 0; i < 6; i++)
        if (ampmMode) {
          if (i > 0 || timeArray[i] > 48)
            displayTimeSmall(4, charPosSmallPM[i], timeArray[i]);
        } else
          displayTimeSmall(4, charPosSmall[i], timeArray[i]);
      break;

    case 5: // hh mm:ss
      for (int i = 0; i < 6; i++)
        if (ampmMode) {
          if (i > 0 || timeArray[i] > 48)
            displayTimeSmall(4, charPosSmallColPM[i], timeArray[i]);
        } else
          displayTimeSmall(4, charPosSmallCol[i], timeArray[i]);
      break;
  }
    
  #ifdef ENABLE_BME280
    if (activeTimeDisplayMode == 6 || LED_NUM_OF_HOR_DISPLAYS >= 8) {      // temp and humidity
      if (BME280_temperature < 0)
        displayTemp = (BME280_temperature - .5);
      else
        displayTemp = (BME280_temperature + .5);
        
      String (fillSpace(displayTemp, 3) + (BME280_fahrenheid ? "F" : "C") + fillZero((BME280_humidity + .5))).toCharArray(bmeArray, 7);

      #ifdef AMPM_2ND_MATRICES
        if (LED_NUM_OF_HOR_DISPLAYS >= 8 && ampmMode) {
          if (BME280_temperature < -9 or BME280_temperature > 99)         // temp is 3 chars so use temp symbol as well
            xOffset = 8;
          else
            xOffset = 3;
            
          for (int i = 0; i < 3; i++)
            displayTimeSmall(4, xOffset + xOffset2ndMatrixSet + charPosBME[i], bmeArray[i]);
  
          if (xOffset == 3) {                                             // only display the temp symbol when it fits
            xOffset = 1;
    
            if (BME280_fahrenheid)
              displayTimeSmall(4, xOffset + xOffset2ndMatrixSet + charPosBME[3], 59);
            else
              displayTimeSmall(4, xOffset + xOffset2ndMatrixSet + charPosBME[3], 58);
          }
        } else {
      #endif
        for (int i = 0; i < 3; i++)
          displayTimeSmall(4, xOffset2ndMatrixSet + charPosBME[i], bmeArray[i]);      
  
        if (BME280_fahrenheid)
          displayTimeSmall(4, xOffset2ndMatrixSet + charPosBME[3], 59);
        else
          displayTimeSmall(4, xOffset2ndMatrixSet + charPosBME[3], 58);
          
      #ifdef AMPM_2ND_MATRICES
        }
      #endif
              
      for (int i = 4; i < 6; i++)
        displayTimeSmall(3, xOffset2ndMatrixSet + charPosBME[i], bmeArray[i]);
    }
  #endif

  displayPixelModes();
  displayTimeColon();
  if (activeTimeDisplayMode != 3) 
    displayPM();

  if (updateDisplay)
    matrix.write();
}

void displayTimeSmall(byte charWidth, byte column, unsigned char c) {
  unsigned int line;
  unsigned char newC = c;

  if (c == 45) newC = 60;   // - char
  
  for (int i = 0; i < charWidth; i++ ) {
    if (newC == 32) {
      line = 0;
    } else {
      if (charWidth == 3)     
        line = numberFont3[(newC - 48) * 4 + i + 1];
      else if (charWidth == 4)
        line = numberFont4[(newC - 48) * 5 + i + 1];
    }
        
    for (int j = 0; j < 8; j++, line >>= 1) {
      if (line & 1)
        matrix.drawPixel(column + i, yOffset + j, HIGH);
      else
        matrix.drawPixel(column + i, yOffset + j, LOW);
    }
  } 
}

void displayCharAltFont(byte font, int startColumn, unsigned char c, bool continuePrevColumn) {
  static int prevColumn = 0;  
  unsigned int maxCharWidth, charWidth, charStartPos;
  unsigned char newC = c;
  int line, column;

  if (continuePrevColumn)
    prevColumn +=2;
  else
    prevColumn = startColumn;

  if (font == 0) {      // big number font
    maxCharWidth = numberFontBig[0];
  }
  
  if (newC == 32) 
    charWidth = 1;
  else {
    charStartPos = (newC - 48) * (maxCharWidth + 1) + 1;

    if (font == 0) {    // big number font
      charWidth = numberFontBig[charStartPos];
    }
  }

  for (int i = 0; i < charWidth; i++ ) {
    column = prevColumn + i;
    line = 0;
    
    if (newC != 32) {      
      if (font == 0) {  // big number font
        line = numberFontBig[(charStartPos + i + 1)];
      }
    }

    for (int j = 0; j < 8; j++, line >>= 1) {      
      if (line & 1)
        matrix.drawPixel(column, yOffset + j, HIGH);
      else
        matrix.drawPixel(column, yOffset + j, LOW);
    }
  } 

  prevColumn = column;
}

void displayTimeColon() {
  static unsigned int prevSec = 0;

  if (timeData.tm_sec != prevSec) {
    if (!blinkingColon)
      colon = HIGH;
    else {
      if (timeData.tm_sec % 2 == 0)
        colon = HIGH;
      else
        colon = LOW;
    }
    
    if (activeTimeDisplayMode == 0) {
      matrix.drawPixel(15, 2, colon);
      matrix.drawPixel(15, 4, colon);
    } else if (activeTimeDisplayMode == 1) {
      matrix.drawPixel(31, 2, colon);
      matrix.drawPixel(31, 3, (colon ? colon : !colon));
      matrix.drawPixel(31, 4, (colon ? colon : !colon));
      matrix.drawPixel(31, 5, !colon);
    } else if (activeTimeDisplayMode == 5) {
      matrix.drawPixel(21, 2, colon);
      matrix.drawPixel(21, 4, colon);
    }

    #ifdef AMPM_2ND_MATRICES
      if (activeTimeDisplayMode == 6 || (LED_NUM_OF_HOR_DISPLAYS >= 8 && !ampmMode))
    #endif
        matrix.drawPixel(xOffset2ndMatrixSet + 16, 0, colon);

    prevSec = timeData.tm_sec;
  }
}

void displayPixelModes() {
  matrix.drawPixel(0,  0, (suspendMode ? HIGH : LOW));                            // top left suspend mode
  matrix.drawPixel(0,  7, (sleepMode ? HIGH : LOW));                              // bottom left sleep mode
  matrix.drawPixel((matrixWidth - 1), 0, (alarmList[0].active ? HIGH : LOW));     // top right alarm 1 active
  matrix.drawPixel((matrixWidth - 1), 7, (sleeping ? HIGH : LOW));                // bottom right sleeping
}

void displayAnnounceMode(byte displayMode) {
  matrix.fillScreen(LOW);
  matrix.drawPixel(14,  3, HIGH);    
  matrix.drawPixel(18,  3, HIGH);    

  if (displayMode == 1)
    matrix.drawPixel(16,  3, HIGH);    
    
  matrix.write();  
}

void displayEndAnnounceMode() {
  matrix.fillScreen(LOW);
  matrix.write();  
}

void displayPM() {
  bool highLow = LOW;

  #ifdef AMPM_2ND_MATRICES
    if (LED_NUM_OF_HOR_DISPLAYS >= 8 && ampmMode) {                 // when AM/PM mode is active, display AM/PM on the right set of 4 matrices
      if (activeTimeDisplayMode == 3)                               // AM/PM is already displayed in this mode so exit
        return;
        
      if (pm) displayTimeSmall(3, 8 + charPosSec[4], 59);  // P
      else    displayTimeSmall(3, 8 + charPosSec[4], 58);  // A      
      displayTimeSmall(3, 8 + charPosSec[5], 60);          // M
      return;
    }
  #endif
  
  if ((activeTimeDisplayMode == 4 || activeTimeDisplayMode == 5) && !ampmMode) // prevent the am/pm pixels from interfering with the left char
    return;
    
  if (ampmMode && pm)
    highLow = HIGH;
        
  for (int i = 3; i < 5; i++)
    matrix.drawPixel(0, i, highLow);  
}

void displayAlarmTime(String message) {  
  String strAlarmTime;
  strAlarmTime = fillZero(alarmList[0].hour) + ":" + fillZero(alarmList[0].min) + ":" + fillZero(alarmList[0].sec);

  if (alarmList[0].actionType == actionNeoWUL)
    strAlarmTime += msgWakeupLight;
  else if (alarmList[0].actionType == actionNeoWULchain)
    strAlarmTime += msgWakeupLightChain;
  
  displayMessage(message + " " + strAlarmTime);
}

#ifdef DAYLIGHTINFO
  void displayDaylightInfo(bool displayDaylightInfo) {
    String daylightInfoMessage;
    time_t t_today;
    
    char d2dSunrise[] = "00:00";
    char d2dSunset[] = "00:00";
    char d2dDaylightTime[] = "00:00";  
    Dusk2Dawn d2d(d2d_lattitude, d2d_longitude, d2d_timezone);
  
    if (timeManualSet) {
      t_today= now();
    } else {
      time(&t_today);     // @EB-todo: connects to timeserver with every call ???
    }
    
    int todaySunrise = d2d.sunrise(year(t_today), month(t_today), day(t_today), DST);
    int todaySunset = d2d.sunset(year(t_today), month(t_today), day(t_today), DST);
    int daylightTime = todaySunset - todaySunrise;
  
    time_t t_yesterday = (t_today - (24 * 60 * 60));
    int yesterdaySunrise = d2d.sunrise(year(t_yesterday), month(t_yesterday), day(t_yesterday), DST);
    int yesterdaySunset = d2d.sunset(year(t_yesterday), month(t_yesterday), day(t_yesterday), DST);
    int daylightTimeYesterday = yesterdaySunset - yesterdaySunrise;
  
    int sunriseDiff = yesterdaySunrise - todaySunrise;
    int sunsetDiff = todaySunset - yesterdaySunset;
    int daylightTimeDiff = daylightTime - daylightTimeYesterday;
  
    String sunriseDiffS = (sunriseDiff > 0 ? "+" : "");
    String sunsetDiffS = (sunsetDiff > 0 ? "+" : "");
    String dltDiffS = (daylightTimeDiff > 0 ? "+" : "");
    
    Dusk2Dawn::min2str(d2dSunrise, todaySunrise);
    Dusk2Dawn::min2str(d2dSunset, todaySunset);
    Dusk2Dawn::min2str(d2dDaylightTime, daylightTime);
  
  //  sprintf(pd_message2, "%s : %s %s, %s %s, %s %s (%s: %d min)", 
  //    d2d_City, msgSunrise, d2dSunrise, msgSunset, d2dSunset, msgDaylightTime, d2dDaylightTime, msgDaylightTimeDifference, daylightTimeDiff);

    daylightInfo =  (String) msgSunrise + (String) + " " + (String) d2dSunrise + (String) " (" + (String) sunriseDiffS + (String) sunriseDiff + (String) + " min), ";
    daylightInfo += "<br>";  // insert line break for the web interface
    daylightInfo += (String) msgSunset  + (String) + " " + (String) d2dSunset  + (String) " (" + (String) sunsetDiffS  + (String) sunsetDiff  + (String) + " min), ";
    daylightInfo += (String) msgDaylightTime + (String) + " " + (String) d2dDaylightTime + (String) " (" + (String) dltDiffS + (String) daylightTimeDiff + (String) + " min)";
  
    if (displayDaylightInfo) {
      daylightInfoMessage = d2d_City + (String) " : " + (String) daylightInfo;
      daylightInfoMessage.replace("<br>", "");  // remove the linebreaks for the display
      displayMessage(daylightInfoMessage);
    }
      
  }
#endif

#ifdef ENABLE_BME280
  void displayBME280() {
    char charBuffer[10];
    String BME280_info;

    sprintf(charBuffer, "%3.1f", BME280_temperature);
    BME280_info = (String) msgbmeTemperature + ' ' + (String) charBuffer + (BME280_fahrenheid ? "F " : "C ");
    BME280_info += (String) msgbmeHumidity + ' ' +  (String) int((BME280_humidity + .5)) + "% ";
    BME280_info += (String) msgbmePressure + ' ' +  (String) int((BME280_pressure + .5)) + " hPa ";

    #ifdef BME280_SEA_LEVEL_PRESSURE
      sprintf(charBuffer, "%2.1f", BME280_altitude);
      BME280_info += (String) charBuffer + " mtr ";
    #endif

    displayMessage(BME280_info);
  }
#endif

//
// display main functions
//

void displayMessage(String message) {
  unsigned long delayTime;
  int letter = 0;
  int x = 0;
  int y = yOffset + (matrix.height() - 8) / 2; 				// center the text vertically  

  if (alarmMessage)
    DEBUGPRINT("           Message at ");
  else
    DEBUGPRINT("Message    at ");

  readCurrentTime();
  DEBUGPRINTLN((String) currentTime + " - " + timeString + ": " + message);

 
  for (int i = 0 ; i < charDefaultWidth * message.length() + matrixWidth - charSpacer; i++) {
    letter = i / charDefaultWidth;
    x = (matrixWidth - 1) - i % charDefaultWidth;
    
    while (x + charDefaultWidth - charSpacer >= 0 && letter >= 0) {
      if (letter < message.length()) {
        if (message[letter] == '|')
          matrix.drawChar(x, y, ' ', HIGH, LOW, 1);
        else
          matrix.drawChar(x, y, message[letter], HIGH, LOW, 1);
      }
      letter--;
      x -= charDefaultWidth;
    }
    matrix.write();

    readCurrentTime();
    delayTime = millis();

    while ((millis() - delayTime) < scrollDelayTime) {
      if (alarmTriggered) {
        checkRecurSkipped();
        #ifdef ENABLE_NEOPIXELS
          switch (alarmList[alarmNum].actionType) {
            case actionNeoWUL: 
            case actionNeoWULchain:
              neopixelsFade(0, false); break;                       // loop neopixel wake up light
          }
        #endif
     
      } else {
        checkAlarms();
        if (alarmTriggered) 
          return;
      }

      if (UI_enabled) {                                             // @EB-todo
        if ((millis() - startUIupdateTime) >= UI_updateDelay) {     // reread the settings after x milliseconds      
          UI_rereadTop();
          startUIupdateTime = millis();
        }
        UI_dnsServer.processNextRequest();
      }
  
      if (UI_alarmToInterrupt) {
        UI_alarmToInterrupt = false;
        if (alarmTriggered) {                       // the Alarm off/stop button is clicked, check if an alarm is triggered. if so, interrupt the alarm
          alarmInterrupted();
        } else {                                    // if not, stop the message
          matrix.fillScreen(LOW);
          matrix.write();
        }
        return;
      }

      #ifndef NO_ENCODER
        reSwitch = readRotEncSwitch();
        if (reSwitch != false) {
          matrix.fillScreen(0);
          matrix.write();
          if (alarmTriggered)
            alarmInterrupted();
          return;
        }
    
        if (readRotEnc() != false) {
          matrix.fillScreen(0);
          matrix.write();
          return;
        }
      #endif

      delay(5);
    }
      
    #ifdef ENABLE_LIGHTSENSOR
      checkLightSensor();
    #endif
  }
}

void displayMessageShort(String message) {
  static int maxChars = LED_NUM_OF_HOR_DISPLAYS * 2 - 1;
  int x = 0;
  int y = yOffset + (matrix.height() - 8) / 2;

  readCurrentTime();  
  DEBUGPRINTLN("Short message " + (String) currentTime + " - " + timeString + ": " + message);

  for (int i = 0; i < maxChars; i++) {
    if (message[i] == '|' || message[i] == 32) {    // set spacing between chars:
      x++;                                          // 1 pixel
      if (LED_NUM_OF_HOR_DISPLAYS >= 8 )            // 2 pixels when using 8 (or more) matrices
        x++;
    } else if (message[i] == ':') {
      matrix.drawPixel(x, 2, HIGH);
      matrix.drawPixel(x, 4, HIGH);
      x += 2;
    } else {
      matrix.drawChar(x, y, message[i], HIGH, LOW, 1);
      x += charDefaultWidth + charSpacer - 1;
    }
  }
  matrix.write();
}

void displayMessageOutLoop(int xOffset, String message, bool reverse) {
  int letter, x;
  int y = yOffset + (matrix.height() - 8) / 2; // center the text vertically
  int xMax = matrix.width() - xOffset;

  int fgCol = HIGH, bgCol = LOW;
  if (reverse) {
    fgCol = LOW;
    bgCol = HIGH;    
  }

  message += " ";
  
  for (int i = 0 ; i < charDefaultWidth * message.length() + xMax - charSpacer; i++) {
    letter = i / charDefaultWidth;
    x = (xMax - 1) - i % charDefaultWidth;
        
    while (x + charDefaultWidth - charSpacer >= 0 && letter >= 0) {
      if (letter < message.length()) {
        if (x >= 0)
          matrix.drawChar(xOffset + x, y, message[letter], fgCol, bgCol, 1);
      }
      letter--;
      x -= charDefaultWidth;
    }
    matrix.write();
    delayAndHandle(30);
  }
}

void displayClock(bool reverse) {
  unsigned int line;
  int xOffset = 11;
  int fgCol = HIGH;
  int bgCol = LOW;

  if (reverse) {
    fgCol = LOW;
    bgCol = HIGH;
  }

  matrix.fillScreen(bgCol);
  displayChar(xOffset, fontCharLogo, fgCol, false, false); // FIX: error: too few arguments to function 'void displayChar(int, unsigned int*, int, bool, bool)'
  matrix.write();
  delayAndHandle(150);
  matrix.fillScreen(LOW);
}

void displayIntro(String message) {  
  unsigned int line;
  int x = 0, xOffset = 11;  
  String tmpMessage = message;
  bool reverse = true;
  
  if (tmpMessage == "")
    tmpMessage = "ericBclock"; 

  playBuzzer(20);

  for (x = (matrixWidth - 1); x >= 0; x--) {
    displayChar(x, fontCharLogo, LOW, false, false); // FIX: error: too few arguments to function 'void displayChar(int, unsigned int*, int, bool, bool)'
    matrix.write();
    delay(30);
  }

  displayMessageOutLoop(xOffset, tmpMessage, reverse);

  if (!reverse) {
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < matrixWidth; x++) {
        matrix.drawPixel(x, y, HIGH);
      }
      matrix.write();
      delay(40);
    }
  }

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < matrixWidth; x++) {
      matrix.drawPixel(x, (7 - y), LOW);
    }
    matrix.write();
    delay(40);
  }

  delay(100);
}

void displayChar(int x, unsigned int fontChar[], int lowHigh, bool fill, bool reverse) {
  unsigned int line;
  int column;  
    
  for (int i = 1; i <= fontChar[0]; i++ ) {
    if (reverse)
      line = fontChar[fontChar[0] + 1 - i];
    else
      line = fontChar[i];
    column = x + i - 1;  
    
    if (column >= 0 && column < matrixWidth) {
      for (int j = 0; j < 8; j++, line >>= 1) {
        if (line & 1 || fill)
          matrix.drawPixel(column, yOffset + j, lowHigh);
        else
          matrix.drawPixel(column, yOffset + j, !lowHigh);
      }
    }
  }
}

void displayAnimPacMan(bool reverse) {  
  animationStarted = true;
  unsigned long startTime = millis();
  int state = 0, column, maxColumn, charWidth, events;
  charWidth = fontCharPacMan[0][0];
  maxColumn = matrixWidth - charWidth;
  readCurrentTime();
  
  for (int x = -charWidth; x <= matrixWidth; x++) {
    if (reverse) {
      column = matrixWidth - x - charWidth;      
    } else {
      column = x;
    }
    
  if (column < maxColumn) // @EB-todo
      displayTime(false);
      
    if (column >= 0 && column < maxColumn) {
      for (int xx = 0; xx <= column; xx++) {        
        for (int yy = 0; yy < 8; yy++)
          matrix.drawPixel(xx, yy, LOW);
      }
    }

    displayChar(column, fontCharPacMan[state], HIGH, false, false); // FIX: error: too few arguments to function 'void displayChar(int, unsigned int*, int, bool, bool)'
    
    matrix.write();
    
    if (x % 3 == 0) {
      state++;
      state = state % 2;
    }
    
    if (millis() - startTime > 500) {
      readCurrentTime();
      startTime = millis();
    }

    events = delayAndHandle(40);
    if (events != 0 && events != 21) {     // delay and abort if there was a user event (except for PIR movement)
      playBuzzer(20);
      return;
    }
    
    if (alarmTriggered)
      return;
  }

  animationStarted = false;
}
/*
***************************************************************************
  ebc_alarmclock - wifi setup and timeserver 
***************************************************************************
  last update 20201130 by ericBcreator
***************************************************************************
*/

void addChipidToUI_SSID() {
//  Serial.printf(" ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));  // print High 2 bytes
//  Serial.printf("%08X  ",(uint32_t)chipid);                       // print Low 4 bytes.

  byte startPos = strlen(UI_ssid);
  if (startPos > (21 - 1 - 5))
    startPos = 21 - 1 - 5;

  #ifdef ESP32
    String idLow4 = String((uint16_t) ESP.getEfuseMac(), HEX);
  #else
    String idLow4 = String((uint16_t) ESP.getChipId(), HEX);
  #endif
  idLow4.toUpperCase();  

  UI_ssid_ca[startPos] = '-';
  for (int i = 0; i < 5; i++)
    UI_ssid_ca[startPos + 1 + i] = idLow4[i];

  DEBUGPRINT("Web interface SSID set to ");
  DEBUGPRINTLN(UI_ssid_ca);
}

void setupWifiConnection() {
  unsigned long wifiStartTime = millis();
  bool connectStat = true;
  matrix.drawChar(3,  yOffset, 'W', HIGH, LOW, 1);
  matrix.drawChar(8,  yOffset, 'i', HIGH, LOW, 1);
  matrix.drawChar(12, yOffset, 'f', HIGH, LOW, 1);
  matrix.drawChar(17, yOffset, 'i', HIGH, LOW, 1);
  matrix.write();

  DEBUGPRINT("Connecting to Wifi ");

  #ifndef NO_WIFI
    WiFi.mode(WIFI_AP_STA);                 // @EB-todo

    #ifdef ESP32
      WiFi.setHostname(UI_ssid_ca);
    #else
      WiFi.hostname(UI_ssid_ca);
    #endif
    DEBUGPRINT("Hostname set to ");
    DEBUGPRINTLN(UI_ssid);

    WiFi.begin(WN_ssid, WN_password);
  
    while (WiFi.status() != WL_CONNECTED && ((millis() - wifiStartTime) < (wifiTimeout * 1000))) {
      reSwitch = readRotEncSwitch();      
      if (reSwitch == 2) {
        DEBUGPRINTLN("* switch pressed, not connecting *");
        return;
      }  

      displayChar(23, fontCharWifi, (connectStat ? HIGH : LOW), (connectStat ? false : true), false); // FIX: error: too few arguments to function 'void displayChar(int, unsigned int*, int, bool, bool)'
      matrix.write();
      connectStat = !connectStat;
      Serial.print(">");
      delay(500);
    }
  
    if (WiFi.status() == WL_CONNECTED) {  
      wifiConnected = true;
      lanIP = WiFi.localIP();
      DEBUGPRINT("connected to Wifi at ");
      DEBUGPRINTLN(lanIP);

      displayChar(23, fontCharWifi, HIGH, false, false); // FIX: error: too few arguments to function 'void displayChar(int, unsigned int*, int, bool, bool)'
      
      #ifndef DEBUG_FAST_START
        while ((millis() - wifiStartTime) < startupAnimDelay) { delay(2); }         // display the wifi logo a min time
      #endif
      
    } else {    
      DEBUGPRINTLN(msgConnWifiFailed);

      #ifndef DEBUG_FAST_START
        displayMessage(msgConnWifiFailed);
      #endif
    }
  #endif
}

void getTimeFromServer() {
  // void configTime(int timezone, int daylightOffset_sec, const char* server1, const char* server2, const char* server3)
  DEBUGPRINT("Connecting to a timeserver ");  
  configTime(timeZone * 3600, (abs(DST) * 3600) + timeOffsetSec, timeserver1, timeserver2, timeserver3);
  gtfsWaitForTime();
  checkDST();
  configTime(timeZone * 3600, (abs(DST) * 3600) + timeOffsetSec, timeserver1, timeserver2, timeserver3);

  if (!timeServerRead)
    return;

  String tmpMsg = msgTimeSynced;
  if (DST == false) 
    tmpMsg = msgDSToff;
  else 
    tmpMsg = msgDSTon;    
  if (DSTmode == 2) 
    tmpMsg += " (auto)";

  #ifndef DEBUG_FAST_START
    if (UI_displayTimeRead) {
      displayMessage(tmpMsg);
      UI_displayTimeRead = false;
    }
  #endif

  timeManualSet = false;
  refreshTimeDisplay = true;

  DEBUGPRINTLN(tmpMsg);
}

void gtfsWaitForTime() {
  static bool firstCall = true;
  unsigned long tsStartTime = millis();
  unsigned int clockAnimation = 0;
  int startColumn = 0;  
  currentTime = 0;

  if (LED_NUM_OF_HOR_DISPLAYS >= 8)
    startColumn = 32;

  time(&currentTime);
  if (currentTime < 30000 || firstCall) {          // only display when it takes more 'time' ;-) and at first boot    
    if (!firstCall || (firstCall && LED_NUM_OF_HOR_DISPLAYS < 8))
      matrix.fillScreen(LOW);
      
    matrix.drawChar((startColumn + 1),  yOffset, 'T', HIGH, LOW, 1);
    matrix.drawChar((startColumn + 6),  yOffset, 'i', HIGH, LOW, 1);
    matrix.drawChar((startColumn + 11), yOffset, 'm', HIGH, LOW, 1);
    matrix.drawChar((startColumn + 17), yOffset, 'e', HIGH, LOW, 1);
    matrix.write();
    
    while((currentTime < 30000 || firstCall) && ((millis() - tsStartTime) < (timeServerTimeout * 1000))) {
      reSwitch = readRotEncSwitch();      
      if (reSwitch > 0) {
        DEBUGPRINTLN("* switch pressed, aborting *");
        return;
      }  
  
      time(&currentTime);
      DEBUGPRINT(".");
      displayChar((startColumn + 24), fontCharClock[clockAnimation], HIGH, false, false); // FIX: error: too few arguments to function 'void displayChar(int, unsigned int*, int, bool, bool)'
      matrix.write();
      clockAnimation++;
      clockAnimation = clockAnimation % 4;
      delay(250);

      #ifdef OTA_UPDATE
        ArduinoOTA.handle();
      #endif
    }

    #ifndef DEBUG_FAST_START
      if (firstCall) {
        while ((millis() - tsStartTime) < startupAnimDelay) { delay(2); }         // display the time logo a min time
      }
    #endif
    
    firstCall = false;
    matrix.fillScreen(LOW);
  }  

  if (currentTime < 30000) {
    DEBUGPRINTLN(msgConnTSfailed);

    #ifndef DEBUG_FAST_START
      displayMessage(msgConnTSfailed);
    #endif
  } else {
    timeServerRead = true;
    DEBUGPRINT("time read from server: ");
    DEBUGPRINTLN(currentTime);
  }
}
/*
***************************************************************************
  ebc_alarmclock - rotary encoder menu
***************************************************************************
  last update 20210319 by ericBcreator
***************************************************************************
*/

void encoderMenu() {
  int menuItem;
  bool firstCall = true;
  
  DEBUGPRINTLN("Entering encoder menu");
  alarmSuspended = true;
  DEBUGPRINTLN("Alarm temporarily suspended");

  do {
    menuItem = encoderMenuSelect(firstCall);
    DEBUGPRINTLN("Menu selection " + (String) menuItem);
    
    switch (menuItem) {
      case  0: encoderSetAlarm0();           break;
      case  1: encoderSetLedBrightness();    break;
      case  2: encoderSetNeoBacklight();     break;
      case  3: encoderSyncTime();            break;
      case  4: encoderSetSleepMode();        break;
      case  5: encoderSetSuspendMode();      break;
      case  6: encoderSetdisplayInfo();      break;
      case  7: encoderSetdisplayInfoDelay(); break;
      case  8: encoderSetTime();             break;
      case  9: encoderSetDate();             break;
      case 10: encoderTimeDisplayMode();     break;
//      case 11: encoderUIminimal();           break; // @EB-todo disabled
      case 11: encoderDisplayInfo();         break;
      case 12: encoderSave();                break;
      case 13: encoderRestart();             break;

      #ifdef ANNOUNCE_MODE
        case 14: 
          encoderAnnounce();
          if (announceMode) {
            customAnnounceMsg = false;
            menuItem = -1;   
          }         
          break;
      #endif
    }    

    if (!announceMode)
      eepromChanged();
      
    firstCall = false;

    if (UI_enabled) {                        // handle the UI if the webinterface is enabled  @EB-todo: use interrupt? also no UI handling when displaying messages
      UI_rereadTab(-1);
      UI_dnsServer.processNextRequest();
    }

  } while (menuItem != -1 && menuItem != FN_ARRAY_SIZE(menu)- 1);

  matrix.fillScreen(0);
  alarmSuspended = false;
  sleepStartTime = millis();
  displayInfoStartTime = millis();
  sleeping = false;
  refreshTimeDisplay = true;
  DEBUGPRINTLN("Menu exited");  
  DEBUGPRINTLN("Alarm suspending ended");  
}
  
int encoderMenuSelect(bool firstCall) {
  static int menuItem = 0;
  int reResult = 0;
  bool reSwitchPressed = false;
  unsigned long menuStartTime = 0, itemChangedStartTime = 0;

  if (firstCall) {
    menuItem = 0;
    displayMessage(msgMenu);
  }
  
  encoderDisplayMenuItemShort(menuItem);
  menuStartTime = millis();
  itemChangedStartTime = menuStartTime;
  
  do {
    reResult = readRotEnc();

    if (reResult < 0) {             // down
      --menuItem;
      if (menuItem < 0)
        menuItem = FN_ARRAY_SIZE(menu) - 1;
    } else if (reResult > 0) {      // up
      menuItem++;
      menuItem = menuItem % FN_ARRAY_SIZE(menu);
    }

    if (reResult != 0) {
      matrix.fillScreen(0);
      encoderDisplayMenuItemShort(menuItem);
      menuStartTime = millis();
      itemChangedStartTime = menuStartTime;
    }

    if (itemChangedStartTime) {
      if (millis() - itemChangedStartTime > 1500) {
        matrix.fillScreen(0);
        encoderDisplayMenuItem(menuItem);
        encoderDisplayMenuItemShort(menuItem);
        menuStartTime = millis();
        itemChangedStartTime  = 0;
      }
    }

    reSwitchPressed = readRotEncSwitch();

    delay(5);
  } while ((millis() - menuStartTime < (1000 * menuTimeout)) && reSwitchPressed == false);

  if (reSwitchPressed == false)
    return -1;
  else
    return menuItem;
}

void encoderDisplayMenuItem(int menuItem) {
  String tmpMsg = menu[menuItem];
  displayMessage(tmpMsg);
}

void encoderDisplayMenuItemShort(int menuItem) {
  String tmpMsg = (String) menuItem + "|" + menu[menuItem];
  displayMessageShort(tmpMsg);
}

//
// rotary encoder menu - functions
//

void encoderSetAlarm0() {
  String msgItem = fillZero(alarmList[0].hour) + fillZero(alarmList[0].min);
  
  displayMessage(msgMenuAlarm0);
  encoderDisplayItem(msgItem);

  alarmList[0].hour = encoderSetValue(alarmList[0].hour, 0, 23, 0, 0, NULL, 200);
  if (returnCode == 9) return;
  alarmList[0].min = encoderSetValue(alarmList[0].min, 0, 59, 2, 0, NULL, 500);
  if (returnCode == 9) return;
  
  alarmList[0].sec = 0;      

  if (alarmList[0].duration == 0) {                   // set to the default values if no duration is set
    alarmList[0].duration = defaultAlarmDuration;
    alarmList[0].buzzer = 4;
    alarmList[0].bright = false;
    alarmList[0].fx = 1;
  }

  #ifdef ENABLE_NEOPIXELS
    msgItem = "Wu" + (String) (sleepMode ? msgOn : msgOff);
    displayMessage(msgMenuAlarm0wul);
    encoderDisplayItemOnOff(msgItem);
    
    bool wul = (alarmList[0].actionType == actionNeoWUL || alarmList[0].actionType == actionNeoWULchain);
    wul = encoderSetValue(wul, 0, 1, 2, 10, NULL, 0);
    if (returnCode == 9) return;
    displayMessage((String) msgMenuAlarm0wul + " " + (wul ? msgOn : msgOff));

    if (wul)
      alarmList[0].actionType = actionNeoWUL;         // @EB-todo: setting wul chain not implemented in the menu yet
    else
      alarmList[0].actionType = 0;
  #endif

  displayAlarmTime(msgAlarm0Set);
}

void encoderSetLedBrightness() {                      // note: setting the brightness manually will disable the light sensor, if present
                                                      //       setting the brightness to -1 will re enable the light sensor
  int newLedBrightness = ledBrightness;
  int prevLedBrightness = ledBrightness;
  String msgItem = "LB" + fillZero(ledBrightness);
  
  displayMessage(msgMenuLedBrightness);
  encoderDisplayItem(msgItem);

  newLedBrightness = encoderSetValue(newLedBrightness, -1, ls_maxValue, 2, 0, encoderSetLedBrightnessCallback, 0);
  if (returnCode == 9) return;
 
  if (newLedBrightness == -1) {
    ledBrightness = prevLedBrightness;
    ledBrightnessManualSet = false;
  } else {
    if (newLedBrightness < ls_minValue)
      newLedBrightness = ls_minValue;
    ledBrightness = newLedBrightness;
    ledBrightnessManualSet = true;
  }

  matrix.setIntensity(ledBrightness);

  #ifdef ENABLE_LIGHTSENSOR
    if (ledBrightnessManualSet)
      displayMessage((String) msgLedBrightnessSet + " " + (String) ledBrightness);  
    else
      displayMessage((String) msgLedBrightnessSet + " auto");
  #else
    displayMessage((String) msgLedBrightnessSet + " " + (String) ledBrightness);
  #endif
}

void encoderSetLedBrightnessCallback(int value) {
  if (value >= 0)
    matrix.setIntensity(value);
}

void encoderSetNeoBacklight() {
  #ifdef ENABLE_NEOPIXELS
    String msgItem = fillSpace(neoBacklight, 4);
    displayMessage(msgMenuNeoBacklight);
    encoderDisplayItemLong(msgItem);
    neoBacklight = encoderSetValue(neoBacklight, 0, numOfNeoPalette, 0, 1, encoderSetNeoBacklightCallback, 200);
    if (returnCode == 9) return;
    displayMessage((String) msgMenuNeoBacklight + " " + (String) neoBacklight);
  #else
    displayMessage(msgMenuNotAvailable);
  #endif
}

void encoderSetNeoBacklightCallback(int value) {
  #ifdef ENABLE_NEOPIXELS
    neopixelsPresetOn(value);
  #endif
}

void encoderSyncTime() {  
  #ifdef NO_WIFI    
    displayMessage(msgNoWifi);
  #else
    displayMessage(msgMenuSyncTime);
    UI_displayTimeRead = true;
    getTimeFromServer();
  #endif
}

void encoderSetSleepMode() {
  String msgItem = "Sl" + (String) (sleepMode ? msgOn : msgOff);
  displayMessage(msgMenuSleepMode);
  encoderDisplayItemOnOff(msgItem);
  sleepMode = encoderSetValue(sleepMode, 0, 1, 2, 10, NULL, 0);
  if (returnCode == 9) return;
  displayMessage((String) msgMenuSleepMode + " " + (sleepMode ? msgOn : msgOff));
}

void encoderSetSuspendMode() {
  String msgItem = "Su" + (String) (suspendMode ? msgOn : msgOff);
  displayMessage(msgMenuSuspendMode);
  encoderDisplayItemOnOff(msgItem);
  suspendMode = encoderSetValue(suspendMode, 0, 1, 2, 10, NULL, 0);
  if (returnCode == 9) return;
  displayMessage((String) msgMenuSuspendMode + " " + (suspendMode? msgOn : msgOff));
}

void encoderSetdisplayInfo() {
  String msgItem = "Da" + (String) (displayInfo ? msgOn : msgOff);
  displayMessage(msgMenudisplayInfo);
  encoderDisplayItemOnOff(msgItem);
  displayInfo = encoderSetValue(displayInfo, 0, 1, 2, 10, NULL, 0);
  if (returnCode == 9) return;
  displayMessage((String) msgMenudisplayInfo + " " + (displayInfo ? msgOn : msgOff));
}

void encoderSetdisplayInfoDelay() {
  String msgItem = fillSpace(displayInfoInterval, 4);
  displayMessage(msgMenuDisplayInfoInterval);
  encoderDisplayItemLong(msgItem);
  displayInfoInterval = encoderSetValue(displayInfoInterval, 10, 9999, 0, 1, NULL, 200);
  if (returnCode == 9) return;
  displayMessage((String) msgMenuDisplayInfoInterval + " " + (String) displayInfoInterval);
}

void encoderDisplayInfo() {
//  String tmpMsg = msgUIstart0 + (String) UI_ssid + msgUIstart1 + WiFi.softAPIP().toString();
//  tmpMsg += " - Version " + (String) _VERSION;
//displayMessage(tmpMsg);

  String tmpMsg = "ericBclock - version " + (String) _VERSION + " - " + (String) _DATE + " by " + (String) _AUTHOR;
  tmpMsg += " - Wifi SSID " + (String) UI_ssid + " IP " + WiFi.softAPIP().toString();
  tmpMsg += " - lan " + lanIP.toString();
  DEBUGPRINTLN(tmpMsg);
  displayIntro(tmpMsg);
}

void encoderSetTime() {
  int newHour = timeData.tm_hour;
  int newMin = timeData.tm_min;
  int newSec = timeData.tm_sec;
 
  String msgItem = fillZero(newHour) + fillZero(newMin);
  displayMessage(msgMenuSetTime0);
  encoderDisplayItem(msgItem);
  newHour = encoderSetValue(newHour, 0, 23, 0, 0, NULL, 200);
  if (returnCode == 9) return;
  newMin = encoderSetValue(newMin, 0, 59, 2, 0, NULL, 200);
  if (returnCode == 9) return;
  
  msgItem = "  " + fillZero(newSec);
  displayMessage(msgMenuSetTime1);
  encoderDisplayItem(msgItem);
  newSec = encoderSetValue(newSec, 0, 59, 2, 0, NULL, 200);
  if (returnCode == 9) return;

  setTime(newHour, newMin, newSec, timeData.tm_mday, (timeData.tm_mon + 1), (timeData.tm_year + 1900));    // important: offset the year of the tm structure by 1900 and month by 1
  currentTime = now();     
  timeData = *localtime(&currentTime);
  timeManualSet = true;

  msgItem = (String) msgTimeSet + " " + fillZero(newHour) + ":" + fillZero(newMin) + ":" + fillZero(newSec);
  displayMessage(msgItem);  
}

void encoderSetDate() {
  int newYear = timeData.tm_year + 1900;                  // important: offset the year of the tm structure with 1900 !
  int newMon = timeData.tm_mon + 1;                       // important: tm month is from 0 to 11 so add 1 !
  int newMDay = timeData.tm_mday;
  int maxDays;

  String msgItem = fillSpace(newYear, 5);
  displayMessage(msgMenuSetDate0);
  encoderDisplayItemLong(msgItem);
  newYear = encoderSetValue(newYear, 1900, 2345, 0, 1, NULL, 200);
  if (returnCode == 9) return;

  msgItem = fillZero(newMon) + fillZero(newMDay);
  displayMessage(msgMenuSetDate1);
  encoderDisplayItem(msgItem);
  newMon = encoderSetValue(newMon, 1, 12, 0, 0, NULL, 200);
  if (returnCode == 9) return;
  
  maxDays = getDaysInMonth(newYear, newMon);  
  newMDay = encoderSetValue(newMDay, 1, maxDays, 2, 0, NULL, 200);
  if (returnCode == 9) return;

  readCurrentTime();  
  setTime(timeData.tm_hour, timeData.tm_min, timeData.tm_sec, newMDay, newMon, newYear);
  timeManualSet = true;

  msgItem = (String) msgDateSet + " " + fillZero(newMDay) + "-" + fillZero(newMon) + "-" + fillZero(newYear);
  displayMessage(msgItem);  

  checkDST();
  displayMessage((DST ? "DST on" : "DST off"));
}

void encoderTimeDisplayMode() {
  displayMessage(msgMenuTimeDisplayMode);  
  timeDisplayMode = encoderSetValue(timeDisplayMode, 0, (FN_ARRAY_SIZE(timeDisplayModes) - 1), 0, 20, encoderTimeDisplayModeUpdate, 200);
  if (returnCode == 9) return;
  displayMessage((String) msgMenuTimeDisplayMode + " " + timeDisplayModes[timeDisplayMode]);
}

void encoderTimeDisplayModeUpdate(int item) {
  String tmpMsg = (String) item + "|" + timeDisplayModes[item];
  displayMessage(timeDisplayModes[item]);
  displayMessageShort(tmpMsg);    
}

//// disabled

void encoderUIminimal() {
  bool UIminimal = UI_controls_minimal;
  String msgItem = "Um" + (String) (UIminimal ? msgOn : msgOff);
  displayMessage(msgMenuUIminimal);
  encoderDisplayItemOnOff(msgItem);
  UIminimal = encoderSetValue(UIminimal, 0, 1, 2, 10, NULL, 0);
  if (returnCode == 9) return;
  displayMessage((String) msgMenuUIminimal + " " + (UIminimal ? msgOn : msgOff));
  
  if (UIminimal) {
    UI_controls_minimal = true;
    UI_controls_extended_sliders = true;
    UI_dnsServer.stop();
    UI_setup();
  }
}

////

void encoderSave() {
  eepromWrite();
  playBuzzer(21);
  displayMessage((String) msgMenuSaved);
}

void encoderRestart() {
  DEBUGPRINTLN("Menu item restart selected");  
  displayMessage((String) msgMenuRestart);  
  playBuzzer(21);
  ESP.restart();
}

void encoderAnnounce() {
  displayMessage(msgMenuAnnouncement0);  
  announceItem = encoderSetValue(announceItem, 0, (FN_ARRAY_SIZE(announcements) - 1), 0, 20, encoderAnnounceDisplay, 200);

  if (announceItem == (FN_ARRAY_SIZE(announcements) - 1) || returnCode == 9)
    announceMode = false;
  else
    announceMode = true;  
}

void encoderAnnounceDisplay(int item) {
  String tmpMsg = (String) item + "|" + announcements[item];
  displayMessage(announcements[item]);
  displayMessageShort(tmpMsg);  
}

//
// display and set functions
//

void encoderDisplayItem(String item) {
  for (int i = 0; i < 4; i++)
    matrix.drawChar(charPos[i], yOffset, item[i], HIGH, LOW, 1);
  matrix.write();
}

void encoderDisplayItemLong(String item) {
  for (int i = 0; i < 5; i++)
    matrix.drawChar(charPosLong[i], yOffset, item[i], HIGH, LOW, 1);
  matrix.write();
}

void encoderDisplayItemOnOff(String item) {
  for (int i = 0; i < 5; i++)
    matrix.drawChar(charPosOnOff[i], yOffset, item[i], HIGH, LOW, 1);
  matrix.write();
}

int encoderSetValue(int value, int minValue, int maxValue, int startPos, byte inputType, callBackFunctionInt callbackFunction, unsigned long delayTime) {
  // inputType: 0 is number (2 digits), 1 is number (5 digits), 10 is bool (on/off), 20 is text (no display, use callbackfunction)
  // byte returnCode is set 0 when selected, 9 for timeOut

  returnCode = 0;
  unsigned long startTime = millis();
  unsigned long startBlinkTime = 0;
  bool blinkState = true;
  int reResult;
  int newValue, prevValue = value;
  String charValue, onOff;

  if (inputType == 10) {
    minValue = 0;
    maxValue = 1;

    newValue = value;
    if (newValue == -1)
      newValue = 1;
  } else {
    newValue = constrain(value, minValue, maxValue);
  }

  if (inputType == 20 && callbackFunction != NULL)
    callbackFunction(newValue);

  while(1) {
    if (millis() - startBlinkTime > 250) {
      switch (inputType) {
        case 0:
          charValue = fillZero(newValue);
          for (int i = 0; i < 2; i++)
            matrix.drawChar(charPos[startPos + i], yOffset, charValue[i], (blinkState ? HIGH : LOW), LOW, 1);
          break;

        case 1:
          charValue = fillSpace(newValue, 5);
          for (int i = 0; i < 5; i++)
            matrix.drawChar(charPosLong[startPos + i], yOffset, charValue[i], (blinkState ? HIGH : LOW), LOW, 1);
          break;

        case 10:
          onOff = (String) (newValue ? msgOn : msgOff);
          for (int i = 0; i < 3; i++) 
            matrix.drawChar(charPosOnOff[startPos + i], yOffset, onOff[i], (blinkState ? HIGH : LOW), LOW, 1);
          break;

        case 20:
          delay(20);
      }
      
      if (inputType != 20)
        matrix.write();

      blinkState = !blinkState;
      startBlinkTime = millis();
    }

    reResult = readRotEnc();

    if (reResult < 0) {
      newValue--;
      if (newValue < minValue)
        newValue = maxValue;
    }
    else if (reResult > 0) {
      newValue++;
      if (newValue > maxValue)
        newValue = minValue;
    }

    if (reResult != 0) {      
      blinkState = true;
      startTime = millis();
      startBlinkTime = millis() - 250;

      if (callbackFunction != NULL)
        callbackFunction(newValue);
    }

    if (readRotEncSwitch() != false) 
      break;

    if (millis() - startTime > (1000 * menuTimeout)) {
      DEBUGPRINTLN("Menu timeout");
      newValue = prevValue;
      returnCode = 9;      
      break;
    }
  }

  if (inputType == 0) {
    matrix.drawChar(charPos[startPos + 0], yOffset, charValue[0], HIGH, LOW, 1);
    matrix.drawChar(charPos[startPos + 1], yOffset, charValue[1], HIGH, LOW, 1);
    matrix.write();
  }

  if (inputType == 10 && newValue != 0)
    newValue = -1;  

  delay(delayTime);
    
  return newValue;
}

//
// rotary encoder
//

int readRotEnc() {                                                              // returns -1 when turned left/down and +1 when turned right/up
  static int reLastPos = 0;
  int reCurrentPos;
  int reResult = 0;

  RE_encoder.tick();

  reCurrentPos = RE_encoder.getPosition();
  if (reCurrentPos < reLastPos)
    reResult = encLeftResult;
  else if (reCurrentPos > reLastPos)
    reResult = encRightResult;

  reLastPos = reCurrentPos;
  return reResult;
}

int readRotEncSwitch() {                                                        // returns 1 when pressed or 2 when pressed for 1 second
  static int reLastSwitchState = LOW;
  int reSwitchState = digitalRead(RE_SWITCH);

  if (reSwitchState == LOW && reLastSwitchState == HIGH) {
    unsigned long startTime = millis();
    
    while (digitalRead(RE_SWITCH) == LOW) {                                     // wait until the button is released or 1 second has passed
      reLastSwitchState = reSwitchState;
      if (millis() - startTime > 1000)
        break;
    }

    //delay(40);
    
    if (millis() - startTime > 1000)
      return 2;
    else
      return 1;
  }
  
  reLastSwitchState = reSwitchState;
  return false;
}
/*
***************************************************************************
  ebc_alarmclock - neopixel functions
***************************************************************************
  last update 20201115 by ericBcreator
***************************************************************************
*/

#ifdef ENABLE_NEOPIXELS

  void neopixelsFade(byte colorSet, bool firstCall) {
    static unsigned long delayTime = 0, prevCallTime = 0;
    static byte colorCode = 0, numOfColorCodes = 0, fadeStep = 0;
    static float brightnessFactor = 0, rStep, gStep, bStep;
    int neoCurColorRint, neoCurColorGint, neoCurColorBint;
      
    if (firstCall) {      
      colorCode = 0;
      neoCurColorR = 0;
      neoCurColorG = 0;
      neoCurColorB = 0;
      numOfColorCodes = sizeof(neoColorsSunrise) / sizeof(neoColorsSunrise[0]);

      delayTime = ((alarmList[alarmNum].duration * 1000) / numOfColorCodes / neoFadeNumOfSteps);
      
      if (delayTime < 100)
        delayTime = 100; // @EB-todo sanitize

//      DEBUGPRINTLN("delayTime " + (String) delayTime);
    }

    if ((millis() - prevCallTime < delayTime) || (colorCode >= numOfColorCodes))
      return;

    if (fadeStep > neoFadeNumOfSteps || firstCall) {
      if (!firstCall) {
        colorCode++;
        if (colorCode >= numOfColorCodes)
          return;
      }

      fadeStep = 0;
      neoCurBrightness = neoColorsSunrise[colorCode][3];
      brightnessFactor = (float) neoCurBrightness / 255;
  
      byte colorR = (float) (neoColorsSunrise[colorCode][0] * brightnessFactor);
      byte colorG = (float) (neoColorsSunrise[colorCode][1] * brightnessFactor);
      byte colorB = (float) (neoColorsSunrise[colorCode][2] * brightnessFactor);
      
      rStep = (float) (((float) colorR - (float) neoCurColorR) / (float) neoFadeNumOfSteps);
      gStep = (float) (((float) colorG - (float) neoCurColorG) / (float) neoFadeNumOfSteps);
      bStep = (float) (((float) colorB - (float) neoCurColorB) / (float) neoFadeNumOfSteps);

//      DEBUGPRINTLN("Neopixel colorcode " + (String) colorCode + " RGB " + (String) colorR + " " + (String) colorG + " " + (String) colorB + " " + (String) neoCurBrightness);
    }

    neoCurColorR += rStep;
    neoCurColorG += gStep;
    neoCurColorB += bStep;
   
    neoCurColorRint = constrain(neoCurColorR, 0, 255);
    neoCurColorGint = constrain(neoCurColorG, 0, 255);
    neoCurColorBint = constrain(neoCurColorB, 0, 255);

//    DEBUGPRINTLN("Neopixel colorcode " + (String) colorCode + " fadestep " + (String) fadeStep + " RGB " + (String) neoCurColorRint + " " + (String) neoCurColorGint + " " + (String) neoCurColorBint);

    for (int j = 0; j < NUM_OF_NEOPIXELS; j++)
      neopixels[j] = CRGB(neoCurColorRint, neoCurColorGint, neoCurColorBint);
    FastLED.show();
    
    prevCallTime = millis();
    fadeStep++;
  }  

  void neopixelsOn(byte RGB[]) {
    int red   = (RGB[0] * RGB[3] / 255);
    int green = (RGB[1] * RGB[3] / 255);
    int blue  = (RGB[2] * RGB[3] / 255);
    
    for (int j = 0; j < NUM_OF_NEOPIXELS; j++)
      neopixels[j] = CRGB(red, green, blue);
    FastLED.show();
  }

  void neopixelsPresetOn(int preset) {
    if (preset >= 0 && preset <= numOfNeoPalette) {
      neopixelsOn(neoPalette[preset]);
    }
  }
  
  void neopixelsOff() {
    for (int j = 0; j < NUM_OF_NEOPIXELS; j++)
      neopixels[j] = CRGB::Black;      
    FastLED.show();
  }

#endif
/*
***************************************************************************
  ebc_alarmclock - misc functions
***************************************************************************
  last update 20210327 by ericBcreator
***************************************************************************
*/

uint16_t getVersionChecksum() {
  unsigned long checkSum = 0;
  
  #ifdef _VERSION
    String ebcVersion = (String) _VERSION;
    if (ebcVersion.length() != 6)
      DEBUGPRINTLN("Version check: length mismatch");
    else {
      byte checksumChar, counter = 0;
      
      for (int i = 0; i < 6; i++) {
        if (i != 2) {
          checksumChar = ebcVersion[i];
          if (checksumChar == 32)             // convert spaces to 0
            checksumChar = 48;

          //checkSum += (checksumChar - 48) * pow(10, (3 - counter));
          checksumChar = (57 - checksumChar);
          checkSum += checksumChar * pow(10, (4 - counter));
          counter++;
        }
      }

      checkSum %= 65535;
    }
  #endif

  return (uint16_t) checkSum;
}

int delayAndHandle(int delayTime) {
  static unsigned long startTimeHandleEvents = millis();  
  unsigned long startDelayTime = millis();
  int eventResult;
  
  while (millis() - startDelayTime < delayTime) {
    if (millis() - startTimeHandleEvents > handleEventsDelay) { 
      startTimeHandleEvents = millis();    
      
      eventResult = handleEvents();
      if (eventResult)
        return eventResult;        
    }
    
    #ifndef NO_WIFI
      if (UI_enabled) {
        if ((millis() - startUIupdateTime) >= UI_updateDelay) {             // reread the settings after x milliseconds      
          UI_rereadTop();
          startUIupdateTime = millis();
        }
        UI_dnsServer.processNextRequest();
      }
    #endif
    delay(1);         // @EB-todo: needed?
  }

  return 0;
}

int checkAndHandleEvents() {
  readCurrentTime();
  checkAlarms();
  return handleEvents();
}

// returns: 0 no user action, -1 rotary left, 1 rotary right, 11 rotary switch, 12 rotary switch long, 21 PIR motion
int handleEvents() {
  #ifdef ENABLE_PIR
    if (digitalRead(PIN_PIR) != PIR_DEFAULT) {
      digitalWrite(PIN_PIR, PIR_DEFAULT);
      return 21;
    }
  #endif
  
  #ifndef NO_ENCODER
    reEnc = readRotEnc();

    if (reEnc < 0)
      return -1;
    else if (reEnc > 1)
      return 1;

    reSwitch = readRotEncSwitch();

    if (reSwitch == 1)
      return 11;
    else if (reSwitch == 2)
      return 12;
  #endif

  #ifdef OTA_UPDATE                           // @EB-todo
    if ((millis() - OTA_startTime) > OTA_interval) {
      ArduinoOTA.handle();
      OTA_startTime = millis();
    }
  #endif
  
  if (UI_enabled) {                           // @EB-todo
    if ((millis() - startUIupdateTime) >= UI_updateDelay) {             // reread the settings after x milliseconds      
      UI_rereadTop();
      startUIupdateTime = millis();
    }
    UI_dnsServer.processNextRequest();
  }

  return 0;
}

int delayAndCheckEnc(unsigned int delayTime) {
  unsigned long startTime = millis();
  unsigned long startDelayTime;  
  int returnValue = 0;

  if (announceMode) {                         // don't check in announce mode, it will interfere with delayHandleEvents()
    startDelayTime = millis();
    while (millis() - startDelayTime < delayTime) { }
    return returnValue;
  }

  #ifndef NO_WIFI                             // only resync when wifi and timeserver are enabled
    #ifndef DEBUG_NO_TIMESERVER
      if (UI_timeToResync) {
        getTimeFromServer();
        matrix.fillScreen(0);
        UI_timeToResync = false;
      }
    #endif
  #endif
  
  while ((millis() - startTime) < delayTime) {
    if (alarmTriggered)
      checkRecurSkipped();                    // @EB-todo needed?????

    #ifndef NO_WIFI
      if (UI_enabled) {                                                     // handle the UI if the webinterface is enabled
        if ((millis() - startUIupdateTime) >= UI_updateDelay) {             // reread the settings after x milliseconds
//          UI_rereadTab(); // @EB-todo reread
          UI_rereadTop();
          startUIupdateTime = millis();
        }
        UI_dnsServer.processNextRequest();
      }
    #endif

    #ifdef ENABLE_PIR
      if (prevPIRstate == true) {
        if (millis() - PIRstartTime > (PIRdelay * 1000)) {                // wait before checking the PIR to prevent retriggering
          prevPIRstate = false;
        }
      } else {
        if (digitalRead(PIN_PIR) != PIR_DEFAULT) {
          returnValue = 21;
          digitalWrite(PIN_PIR, PIR_DEFAULT);
          prevPIRstate = true;
          PIRstartTime = millis();
          #ifdef DEBUG_PIR
            DEBUGPRINTLN("Motion detected at " + timeString);
          #endif

          if (sleeping)
            endSleepMode(true);
          else      
            endSleepMode(false);
        }
      }
    #endif

    #ifdef ENABLE_LIGHTSENSOR
      checkLightSensor();
    #endif

    #ifndef NO_ENCODER
      reSwitch = readRotEncSwitch();
      reEnc = readRotEnc();
      
      if (reSwitch || reEnc) {
        DEBUGPRINT("Rotary encoder used: ");

        if (reEnc < 0)          returnValue = -1;
        else if (reEnc > 1)     returnValue =  1;
        if (reSwitch == 1)      returnValue = 11;
        else if (reSwitch == 2) returnValue = 12;

        DEBUGPRINTLN(returnValue);

        if (sleeping) {
          endSleepMode(true);
        } else {
          if (alarmTriggered || forceFirstAlarm) {
            if (forceFirstAlarm)
              forceFirstAlarm = false;
            alarmInterrupted();
          } else {
            if (reSwitch == 1) {
              alarmList[0].active = !alarmList[0].active;
              alarmRecTriggered[0] = false;
              eepromChanged();
  
              if (UI_enabled)
                UI_rereadTab(-1);
  
              displayPixelModes();
              matrix.write();
              
              if (alarmList[0].active)
                displayAlarmTime(msgAlarm0Active); 
              else
                displayMessage(msgAlarm0Inactive);
              return 0;
            } else if (reSwitch == 2) {
              //playBuzzer(20);   @EB-todo
              encoderMenu();
            }
          }
        }
      }
    #endif // NO_ENCODER

//    delay(delayAndCheckEncDelay);             // @EB-todo: strange bug: the delay function causes problems when called from the UI webinterface 
                                                //           (the playTone function after saving) so replaced with empty while loop
    
    startDelayTime = millis();
    while (millis() - startDelayTime < delayAndCheckEncDelay) { 
      #ifndef NO_WIFI
        if (UI_enabled)
          UI_dnsServer.processNextRequest();
      #endif
    }

    /*
	nope: don't return early, it will mess up playtone delays...
	if (returnValue)
      return returnValue;                       // event occurred so break the loop and return the value
	*/
  }

  return 0;
}

#ifdef ENABLE_LIGHTSENSOR
  void checkLightSensor() {
    static bool firstCall = true;
    static unsigned long lsStartTime = 0;
    static int prevReadValue = 0;
    int readValue;
    int rangedValue;
    int mappedValue;

    if (ledBrightnessManualSet)                                             // don't use the light sensor when the brightness is set manually
      return;
  
    if (sleeping)
      return;
    
    if (lsStartTime != 0 && ((millis() - lsStartTime) < ls_delay)) {        // delay time before checking the light sensor
      return;
    }

    lsStartTime = millis();
    readValue = analogRead(PIN_LS);

    #ifdef LIGHTSENSOR_AVG                                                  // if defined, average with previous reading
      if (prevReadValue) {
        rangedValue = (readValue + prevReadValue) / 2;
      } else {
        rangedValue = readValue;
      }
    #else
      rangedValue = readValue;
    #endif

    prevReadValue = readValue;

    if (rangedValue < ls_minSensor)                                         // set out of range values to min or max values
      rangedValue = ls_minSensor;
    if (rangedValue > ls_maxSensor)
      rangedValue = ls_maxSensor;
    
    mappedValue = map(rangedValue, ls_minSensor, ls_maxSensor, ls_minValue, ls_maxValue);

    #ifdef LIGHTSENSOR_CURVE                                                // if defined, use slow curve
      mappedValue = int((ls_minMaxRange + .5 - (cos(3.14 / 2 / ls_minMaxRange * mappedValue) * ls_minMaxRange))) + ls_minValue;
    #endif

    #ifdef LIGHTSENSOR_OFFSET
      mappedValue += LIGHTSENSOR_OFFSET;
      mappedValue = constrain(mappedValue, ls_minValue, ls_maxValue);
    #endif

    #ifdef DEBUG_LIGHTSENSOR
      #ifdef LIGHTSENSOR_AVG
        DEBUGPRINT("Brightness read value " + (String) readValue + ", averaged, set in range " + (String) rangedValue + ", mapped to " + (String) mappedValue);
      #else
        DEBUGPRINT("Brightness read value " + (String) readValue + ", set in range " + (String) rangedValue + ", mapped to " + (String) mappedValue);
      #endif
    #endif

    if (firstCall) {
      ledBrightness = mappedValue;
      matrix.setIntensity(ledBrightness);
      #ifdef DEBUG_LIGHTSENSOR
        DEBUGPRINTLN(", set to " + (String) ledBrightness);  
      #endif
      firstCall = false;
      
    } else if (abs(mappedValue - ledBrightness) >= ls_triggerStep) {
      if (mappedValue < ledBrightness)
        ledBrightness--;      
      else
        ledBrightness++;
      matrix.setIntensity(ledBrightness);
      
      #ifdef DEBUG_LIGHTSENSOR
        DEBUGPRINTLN(", set to " + (String) ledBrightness);
      #endif

      if (UI_enabled) {
        UI_rereadLS();
      }
      
    } else {
      #ifdef DEBUG_LIGHTSENSOR
        DEBUGPRINTLN();
      #endif
    }
  }
#endif

void startSleepMode() {
  #ifdef NOPACMAN
    displayMessage(UI_msg_sleepMode);
    matrix.fillScreen(LOW);
  #else
    displayAnimPacMan (false); // FIX: error: too few arguments to function 'void displayAnimPacMan(bool)'
  #endif
  
  sleeping = true;
  DEBUGPRINTLN("Sleep mode started at " + timeString);
  sleepStartedTime = millis();
  displayPixelModes();
  matrix.write();
}
  
void endSleepMode(bool resetInfoTime) {
  if (sleeping) {
    readCurrentTime();
    DEBUGPRINTLN("Sleep mode ended at " + timeString);
    bool prevDST = DST;
    checkDST();
    refreshTimeDisplay = true;
    refreshTempSwapTime = 1;
    sleeping = false;

    #ifndef NO_WIFI
      #ifndef DEBUG_NO_TIMESERVER
        if (DST != prevDST)     // reread the time if the DST changed while sleeping
          getTimeFromServer();
      #endif
    #endif

    #ifndef NOPACMAN
      displayAnimPacMan(true);
    #endif    
  }
  
  sleepStartTime = millis();

  if (resetInfoTime)
    displayInfoStartTime = millis();
}

void setupMatrix() {
  int i = LED_NUM_OF_HOR_DISPLAYS * LED_NUM_OF_VERT_DISPLAYS - 1;
  
  matrix.setIntensity(ledBrightness);
  if (i >= 0) matrix.setRotation(0, LED_MATRIX_ROTATION_0); // set the rotation as defined for each matrix
  if (i >= 1) matrix.setRotation(1, LED_MATRIX_ROTATION_1);
  if (i >= 2) matrix.setRotation(2, LED_MATRIX_ROTATION_2);
  if (i >= 3) matrix.setRotation(3, LED_MATRIX_ROTATION_3);
  if (i >= 4) matrix.setRotation(4, LED_MATRIX_ROTATION_4);
  if (i >= 5) matrix.setRotation(5, LED_MATRIX_ROTATION_5);
  if (i >= 6) matrix.setRotation(6, LED_MATRIX_ROTATION_6);
  if (i >= 7) matrix.setRotation(7, LED_MATRIX_ROTATION_7);
  matrix.fillScreen(LOW);
}

//
// time functions
//

void readCurrentTime() {
  // note: when timeManualSet is true, the time is read from the internal ESP clock, when false from the NTP server
  //       timeManualSet is set true when the time is set with the rotary encoder and false when the time is read with getTimeFromServer()
  
  if (timeManualSet) {
    currentTime = now();
  } else {
    time(&currentTime);     // @EB-todo: connects to timeserver with every call ???
  }
  
  timeData = *localtime(&currentTime);
  
//  timeString = String(ctime(&currentTime));
//  timeString.trim();

  int year = timeData.tm_year;
  if (year < 1900) year += 1900;
    
  timeString = days[timeData.tm_wday] + " "+ fillZero(timeData.tm_mday) + " " + months[timeData.tm_mon]  + " " + (String) year + " ";
  timeString += fillZero(getPMhour()) + ":" + fillZero(timeData.tm_min) + ":" + fillZero(timeData.tm_sec);

  if (ampmMode) {
    if (pm)  timeString += " PM";
    else     timeString += " AM";
  }
}

int getPMhour() {
  int pmHour = timeData.tm_hour;
  pm = false;
  if (pmHour > 11) pm = true;

  if (ampmMode) {         // displaying 12:00-12:59 and 1:00-11:59 AM or PM
    if (pmHour == 0)      pmHour += 12;
    else if (pmHour > 12) pmHour -= 12;    
  }

  return pmHour;
}

bool checkLeapYear (int the_year) {
  if (the_year % 4 != 0) return false;
  if (the_year % 100 == 0 && the_year %400 != 0) return false;
  if (the_year % 400 == 0) return true;
  return true;
}

byte getDaysInMonth (int year, int month) { 
  byte daysInMonth = 31;  
  switch (month) {
    case 2:
      if (checkLeapYear(year))
        daysInMonth = 29;
      else
        daysInMonth = 28;
      break;
    case 4:
    case 6:
    case 9:
    case 11:
      daysInMonth = 30;
      break;
  }
  return daysInMonth;
}

//
// checkDST
//

void checkDST() {
  if (DSTmode == 0) { DST = false; return; }  // DST mode set to off so return false
  if (DSTmode == 1) { DST = true;  return; }  // DST mode set to on so return true

  DST = false;                                // defaults to false

  if (timeManualSet)
    currentTime = now();
  else
    time(&currentTime);
  
  timeData = *localtime(&currentTime);

  int y = timeData.tm_year - 100;             // tm_year returns the year after 1900 so subtract 100 to get the year after 2000
  int x1 = (y + y / 4 - 2) % 7;               // identifies Sunday offset for March
  int x2 = (y + y / 4 + 2) % 7;               // identifies Sunday offset for October
  
  int mon = timeData.tm_mon + 1;              // important: the tm_mon variable ranges from 0 to 11 so add 1 to get 1 to 12 !!

  // DST: begins last Sunday of March 02:00
  if ((mon == 3) && (timeData.tm_mday == (31 - x1)) && (timeData.tm_hour >= 2))
    DST = true;
  if (((mon == 3) && (timeData.tm_mday > (31 - x1))) || (mon  > 3))
    DST = true;

  // DST: ends last Sunday of Oct 02:00
  if ((mon == 10) && (timeData.tm_mday == (31 - x2)) && (timeData.tm_hour >= 2))
    DST = false;
  if (((mon == 10) && (timeData.tm_mday > (31 - x2))) || (mon > 10) || (mon < 3))
    DST = false;
}

//
// div
//

String fillZero(int value) {
  int tmpValue;
  
  if (value < 0)
    return String(value);

  tmpValue = constrain(value, 0, 99);
    
  if (tmpValue < 10)
    return '0' + String(tmpValue);
  else
    return String(tmpValue);
}

String fillSpace(int value, int valLength) {
  String tmpStr = String(value);
  while (tmpStr.length() < valLength)
    tmpStr = " " + tmpStr;
  return tmpStr;
}

//
// buzzer
//

#ifdef NO_BUZZER
  void playBuzzer(int numOfBuzz) {                      // no buzzer so just delay numOfBuzz * 100 ms
    if (numOfBuzz < 11) {
      for (int i = 0; i < numOfBuzz; i++) {
        delayAndCheckEnc(100);
      }
    } else
      delayAndCheckEnc(100);
  }
#else
  void playBuzzer(int numOfBuzz) {
    #ifdef ESP32                                        // ESP32
      ledcSetup(0, 0, 8);
      ledcAttachPin(PIN_BUZZER, 0);
    #endif
    
    if (numOfBuzz < 11) {
      for (int i = 0; i < numOfBuzz; i++) {
        playTone(2000, 50, 50);
      }
    } else {
      switch (numOfBuzz) {
        case 11:                                  // big ben ECDG(-1) G(-1)DEC
          // FIX: error: too few arguments to function 'void playTone(int, int, int)'
          playTone(659, 500, 0); playTone(523, 500, 0); playTone(587, 500, 0); playTone(392, 1000, 500);
          playTone(392, 500, 0); playTone(587, 500, 0); playTone(659, 500, 0); playTone(523, 1000, 500);
          break;

        case 12:                                  // Avicii
          //127 BPM = 1/8 Note: 236.22, 1/16 Note: 118.11, 1/32 Note: 59.055, 1/64 Note: 29.528, 1/128: 14.764 
          //sustain: 1/16+1/32+1/64+1/128 = 221ms //release: 1/128 = 15ms 
          
          playTone(1480, 221, 15); playTone(1319, 221, 15); playTone(1319, 221, (15 + 236)); 
          playTone(1319, 221, 15); playTone(1319, 221, 15); playTone(1319, 221, 15); 
          playTone(1319, 221, 15); playTone(1245, 221, 15); playTone(1245, 221, 15); 
          playTone(1319, 221, 15); playTone(1319, 221, (15 + 236));
          playTone(2217, 221, 15); playTone(1976, 221, 15); playTone(1661, 221, 15);
          playTone(1480, 221, 15); playTone(1319, 221, 15); playTone(1319, 221, (15 + 236));
          playTone(1319, 221, 15); playTone(1319, 221, 15); playTone(1319, 221, 15);
          playTone(1319, 221, 15); playTone(1109, 221, 15); playTone(1109, 221, 15);
          playTone(988, 221, 15);  playTone(988, 221, (15 + 236));
          playTone(2217, 221, 15); playTone(1976, 221, 15); playTone(1661, 221, 15);
          break;
          
        case 20: playTone(2250, 50, 0); break; // UI button clicked
        case 21: for (int i = 0; i < 2; i++) { playTone(2250,  50,  50); }   break;   // save
        case 22: for (int i = 0; i < 2; i++) { playTone(2500,  50,  50); }   break;   // firmware update start
        case 23: for (int i = 0; i < 3; i++) { playTone(2500,  50,  50); }   break;   // firmware update finish
        case 24: for (int i = 0; i < 2; i++) { playTone(1000, 100, 100); }   break;   // firmware update error
        case 31: for (int i = 0; i < 5; i++) { playTone(1750,  50,  50); }   break;   // announcement
      }
    }

    #ifdef ESP32
      ledcDetachPin(PIN_BUZZER);
    #endif
    pinMode(PIN_BUZZER, INPUT);                   // make sure the buzzer is silent ;-)
  }
#endif

void playTone(int freq, int duration, int postDelay) {
  unsigned int startDelayTime;
  
  if (freq > 0) {
    #ifdef ESP32
      ledcWriteTone(0, freq);
    #else
      tone(PIN_BUZZER, freq);
    #endif
  } 

//  startDelayTime = millis();
//  while ((millis() - startDelayTime) < duration) { };
  delayAndCheckEnc(duration);
  
  #ifdef ESP32
    ledcWriteTone(0, 0);
    ledcWrite(0, LOW);
  #else
    noTone(PIN_BUZZER);
  #endif  

  if (postDelay) {
//    startDelayTime = millis();
//    while ((millis() - startDelayTime) < postDelay) { };
    delayAndCheckEnc(postDelay);
  }
}

void testBuzzer() {  
  for (int i = 100; i < 10000; i += 100) {
    DEBUGPRINTLN(i);
    playTone(true, i, 500);
  }
  playTone(false, 0, 0);
}

//
// BME280
//

#ifdef ENABLE_BME280
  void readBME280() {
    BME280_temperature = BME280.readTemperature();                        // get temperature in degree Celsius
    
    if (BME280_fahrenheid) {                                              // convert to Fahrenheid if necessary
      BME280_temperature = (BME280_temperature * 9 / 5 + 32);
    }
      
    BME280_humidity = BME280.readHumidity();                              // get humidity in rH%
    BME280_pressure = BME280.readPressure() / 100;                        // get pressure in Pa   

    #ifdef BME280_SEA_LEVEL_PRESSURE
      BME280_altitude = BME280.readAltitude(BME280_SEA_LEVEL_PRESSURE);   // get altitude in mtr. BME280_SEA_LEVEL_PRESSURE has to be set correctly to get a accurate reading    
    #endif

    BME280_temperature *= BME280_temperatureCalFactor;
    BME280_temperature += BME280_temperatureCalValue;
    BME280_humidity    *= BME280_humidityCalFactor;
    BME280_humidity    += BME280_humidityCalValue;
    BME280_pressure    *= BME280_pressureCalFactor;
    BME280_pressure    += BME280_pressureCalValue;
  }
#endif
/*
***************************************************************************
  ebc_alarmclock - wifi setup with user interface
***************************************************************************
  last update 20210319 by ericBcreator
***************************************************************************
*/

uint16_t UI_ID_sw_info;
uint16_t UI_ID_sw_ssid;
uint16_t UI_ID_sw_password;
uint16_t UI_ID_sw_saveButton;
uint16_t UI_ID_sw_restartButton;

unsigned long UI_sw_startTime;

const int maxNumOfSSIDs = 20;             // @EB-custom: max num of ssids
String ssids[maxNumOfSSIDs];
int numOfssids;

void UI_sw_setupWifi(char * UI_title) {
  UI_sw_scanSSIDs();
  
  WiFi.mode(WIFI_AP);                     // @EB-todo
  WiFi.softAPConfig(UI_apIP, UI_apIP, IPAddress(255, 255, 255, 0));
  #ifdef ESP32
    WiFi.setHostname(UI_ssid_ca);
  #else
    WiFi.hostname(UI_ssid_ca);
  #endif
  DEBUGPRINTLN(UI_ssid_ca);

  WiFi.softAP(UI_ssid, UI_password, UI_channel);
  DEBUGPRINTLN("Access point started");
  
  UI_dnsServer.start (UI_DNS_PORT, "*", UI_apIP);
  DEBUGPRINTLN("DNS server started");

  /// setup elements

  String UI_lb_sw_info = UI_sw_msgInfo0 + (String) (UI_sw_length - 1) + UI_sw_msgInfo1;  
  UI_ID_sw_info = ESPUI.addControl(ControlType::Label, UI_msg_sw_info, UI_lb_sw_info, ControlColor::Wetasphalt, Control::noParent);

  UI_ID_sw_ssid = ESPUI.addControl(ControlType::Select, UI_msg_sw_ssid, "", ControlColor::Peterriver, Control::noParent, &UI_sw_callSelect);
  for (int i = 0; i < numOfssids; i++) {
    charPtr = ssids[i].c_str();
    ESPUI.addControl(ControlType::Option, charPtr, (String) i, ControlColor::Peterriver, UI_ID_sw_ssid);
  }

  if (numOfssids)   // set ssid to first selection
    ssids[0].toCharArray(UI_sw_EEPROM.ssid, UI_sw_length);
    
  UI_ID_sw_password = ESPUI.addControl(ControlType::Text, UI_msg_sw_password, UI_sw_EEPROM.password, ControlColor::Peterriver, Control::noParent, &UI_sw_callText);
  UI_ID_sw_saveButton = ESPUI.addControl(ControlType::Button, UI_msg_sw_saveButton, UI_msg_sw_saveButton, ControlColor::Alizarin, Control::noParent, &UI_sw_callButton);
  UI_ID_sw_restartButton = ESPUI.addControl(ControlType::Button, UI_msg_sw_restartButton, UI_msg_sw_restartButton, ControlColor::Alizarin, Control::noParent, &UI_sw_callButton);

  ///

  UI_dnsServer.start(UI_DNS_PORT, "*", UI_apIP);
  ESPUI.begin(UI_title);
  UI_sw_updatePassword();   // mark with asterisks
  UI_sw_startTime = millis();

  while(1) {                // stay in loop until save is clicked or after a timeout
    UI_dnsServer.processNextRequest();

    if (wifiSetupTimeout) {
      if (((millis() - UI_sw_startTime) / 1000) >= wifiSetupTimeout) {
	    DEBUGPRINTLN("Wifi setup timeout, restarting");
        ESP.restart();
	    }
    }
    
    delay(40);                        // a little delay

    reSwitch = readRotEncSwitch();    // if the encoder switch is long pressed: restart
    if (reSwitch == 2) {
      DEBUGPRINTLN("Rotary encoder switch long press, restarting");
      ESP.restart();
    }
  }
}

void UI_sw_callSelect(Control *sender, int type) {
  UI_sw_startTime = millis();
  if (sender->id == UI_ID_sw_ssid) {
    int value = sender->value.toInt();
    ssids[value].toCharArray(UI_sw_EEPROM.ssid, UI_sw_length);
  }
}

void UI_sw_callText(Control *sender, int type) {
  UI_sw_startTime = millis();
  if (sender->id == UI_ID_sw_password) {
    sender->value.toCharArray(UI_sw_EEPROM.password, UI_sw_length);
    UI_sw_updatePassword();
  }
}

void UI_sw_updatePassword() {
  UI_sw_startTime = millis();
  ESPUI.updateText(UI_ID_sw_password, UI_sw_fillAsterisk(UI_sw_EEPROM.password, (UI_sw_length) - 1));
}

String UI_sw_fillAsterisk(String value, byte valLength) {
  String tmpStr = "";
  while (tmpStr.length() < value.length())
    tmpStr += "*"; 
  return String(tmpStr);
}

void UI_sw_callButton(Control *sender, int type) {
  UI_sw_startTime = millis();
  if (type == B_UP) {
    if (sender->id == UI_ID_sw_saveButton) {
      UI_sw_EEPROM.idStart = '$';
      UI_sw_EEPROM.idEnd = '*';
      UI_sw_writeEEPROM();

//        ESPUI.print(UI_ID_sw_info, UI_sw_msgRestarting);
//        UI_dnsServer.processNextRequest();    
//        displayMessageOutLoop(0, UI_sw_msgRestarting, false);  // @EB-todo
      ESP.restart();
    } else if (sender->id == UI_ID_sw_restartButton) {
      ESP.restart();
    }
  }
}

bool UI_sw_readEEPROM() {
  EEPROM.begin(sizeof(UI_sw_EEPROM));
  EEPROM.get(0, UI_sw_EEPROM);

  if (UI_sw_EEPROM.idStart == '$' && UI_sw_EEPROM.idEnd == '*') {
    return true;
  } else {
    DEBUGPRINTLN("Wifi setup EEPROM identifiers failed " + (String) UI_sw_EEPROM.idStart + " " + (String) UI_sw_EEPROM.idEnd + ", initializing");
    UI_sw_resetEEPROM();
    return false;
  }
}

void UI_sw_resetEEPROM() {
  UI_sw_EEPROM.idStart = ' ';
  for (int i = 0; i < UI_sw_length; i++) {
    UI_sw_EEPROM.ssid[i] = 0;
    UI_sw_EEPROM.password[i] = 0;
  }
  UI_sw_EEPROM.idEnd = ' ';
  UI_sw_writeEEPROM();
}

void UI_sw_writeEEPROM() {  
  EEPROM.begin(eepromBufSize);
  EEPROM.put(0, UI_sw_EEPROM);
  eepromWriteInitAndAlarms();     // @EB-todo needed to keep the init and alarms settings otherwise those settings will be erased!
  EEPROM.commit();

  DEBUGPRINT("EEPROM written:, SSID: " + (String) UI_sw_EEPROM.ssid);
  DEBUGPRINTLN(" password: " + (String) UI_sw_EEPROM.password);
  
  playBuzzer(21);
}

void UI_sw_scanSSIDs() {
  DEBUGPRINT("Scanning for networks...");
  numOfssids = 0;
    
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  int foundSSIDs = WiFi.scanNetworks();
  DEBUGPRINTLN("found " + (String) foundSSIDs + " networks");

  if (foundSSIDs) {
    for (int i = 0; i < foundSSIDs; ++i) {

//      if ((i < maxNumOfSSIDs) && (WiFi.SSID(i).length() < WIFI_CRED_LENGTH)) { // @EB-todo. NOTE: if skipping ssids, ssids[i] won't match anymore!!

      if ((i < maxNumOfSSIDs)) {
        numOfssids++;
        ssids[numOfssids - 1] = WiFi.SSID(i);
      
//      Serial.print(WiFi.RSSI(i));
//      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      }
    }
  }
}
/*
***************************************************************************
  ebc_alarmclock - web interface functions 
***************************************************************************
  last update 20210327 by ericBcreator
***************************************************************************
*/

uint16_t UI_ID_info0;
uint16_t UI_ID_info1;
uint16_t UI_ID_timeZone;
uint16_t UI_ID_DSTmode;
uint16_t UI_ID_ampmMode;
uint16_t UI_ID_timeDisplayMode;
uint16_t UI_ID_displayInfo;
uint16_t UI_ID_displayInfoInterval;
uint16_t UI_ID_suspendMode;
uint16_t UI_ID_sleepMode;
uint16_t UI_ID_sleepDelay;
uint16_t UI_ID_sleepStart;
uint16_t UI_ID_brightnessAuto;
uint16_t UI_ID_brightness;
uint16_t UI_ID_neoBacklight;
uint16_t UI_ID_displayDaylight;
uint16_t UI_ID_displayBME280;
uint16_t UI_ID_BME280_fahrenheid;
uint16_t UI_ID_BME280_tempTimeSwap;
uint16_t UI_ID_resetWifi;
uint16_t UI_ID_restart;
uint16_t UI_ID_alarmNumber;
uint16_t UI_ID_message;
uint16_t UI_ID_alarmActive;
uint16_t UI_ID_pad;
uint16_t UI_ID_hour;
uint16_t UI_ID_min;
uint16_t UI_ID_sec;
uint16_t UI_ID_duration;
uint16_t UI_ID_month;
uint16_t UI_ID_day;
uint16_t UI_ID_weekDay;
uint16_t UI_ID_weekDays;
uint16_t UI_ID_weekendDays;
uint16_t UI_ID_recurringType;
uint16_t UI_ID_recurringInterval;
uint16_t UI_ID_actionType;
uint16_t UI_ID_actionNum;
uint16_t UI_ID_buzzer;
uint16_t UI_ID_fx;
uint16_t UI_ID_bright;
uint16_t UI_ID_testAlarm;
uint16_t UI_ID_alarmsReset;
uint16_t UI_ID_saveToEEPROM;
uint16_t UI_ID_interruptAlarm;
uint16_t UI_ID_resync;
uint16_t UI_ID_announcements;
uint16_t UI_ID_customAnnouncement;
uint16_t UI_ID_announceMode;

uint16_t UI_tabG, UI_tabA, UI_tabAn;

String UI_tmpMsg1, UI_tmpMsg2;

void UI_filesystem() {                 // check / setup the filesystem
  #ifdef UI_USE_FILESYSTEM  
    #ifdef UI_INIT_FILESYSTEM
      fileSystemInitialized = 'N';
    #endif
    
    if (fileSystemInitialized != 'Y') {
      DEBUGPRINTLN("UI - preparing filesystem");
      #ifndef DEBUG_FAST_START
        displayMessage(msgUIfilesystemInit0);
      #endif      
      displayChar(13, fontCharHourglass, HIGH, false, false); // FIX: error: too few arguments to function 'void displayChar(int, unsigned int*, int, bool, bool)'
      matrix.write();
      
      ESPUI.prepareFileSystem();
      
      DEBUGPRINTLN("UI - finished preparing filesystem");
      playBuzzer(20);
      fileSystemInitialized = 'Y';
      #ifndef DEBUG_FAST_START
        displayMessage(msgUIfilesystemInit1);
      #endif
      eepromChanged();
    }
  #endif
}

void UI_setup() {
  //  WiFi.mode(WIFI_AP); // @EB-todo
  WiFi.softAPConfig(UI_apIP, UI_apIP, IPAddress(255, 255, 255, 0));
  #ifdef ESP32
    WiFi.setHostname(UI_ssid_ca);
  #else
    WiFi.hostname(UI_ssid_ca);
  #endif

  DEBUGPRINTLN("UI - Initial size: " + (String) UI_InitialSize);
  DEBUGPRINT("UI - Controls: ");
  if (UI_controls_extended)         DEBUGPRINT("'extended' ");
  if (UI_controls_minimal)          DEBUGPRINT("'minimal' ");
  if (UI_controls_extended_sliders) DEBUGPRINT("'sliders'");
  DEBUGPRINTLN();
    
  DEBUGPRINT("UI - Hostname set to ");
  DEBUGPRINTLN(UI_ssid_ca);
  
  WiFi.softAP(UI_ssid, UI_password, UI_channel);
  DEBUGPRINTLN("UI - Access point started");

  UI_dnsServer.start (UI_DNS_PORT, "*", UI_apIP);
  DEBUGPRINTLN("UI - DNS server started");

  UI_addControlsTop();
  UI_addControlsMain();
  UI_addControlsAlarms();
  UI_addControlsAnnounce();
  DEBUGPRINTLN("UI - Number of controls: " + (String) UI_numOfControls);  

  ESPUI.jsonUpdateDocumentSize = UI_updateSize;
  ESPUI.jsonInitialDocumentSize = UI_InitialSize;

  #ifdef DEBUG_UI_MESSAGES
    ESPUI.setVerbosity(Verbosity::VerboseJSON);
  #endif

  #ifdef UI_USE_FILESYSTEM
    DEBUGPRINTLN("UI - Begin (filesystem)");
    if (UI_secure)      
      ESPUI.beginSPIFFS(UI_title, UI_ssid, UI_password);
    else
      ESPUI.beginSPIFFS(UI_title);
  #else
    DEBUGPRINTLN("UI - Begin");
    if (UI_secure)
      ESPUI.begin(UI_title, UI_ssid, UI_password);
    else
      ESPUI.begin(UI_title);
  #endif

  String tmpMsg = msgUIstart0 + (String) UI_ssid + msgUIstart1 + WiFi.softAPIP().toString();
  tmpMsg += msgUIstart2 + lanIP.toString();
  DEBUGPRINTLN("UI - " + tmpMsg);

  #ifndef DEBUG_FAST_START
    displayMessage(tmpMsg);
  #endif
}

// add controls

uint16_t UI_addSlider(const char *label, int value, int minValue, int maxValue, ControlColor color, uint16_t parentControl, void (*callback)(Control *, int)) {
  uint16_t controlID;
  
  if (UI_controls_extended_sliders) {
    controlID = ESPUI.addControl(ControlType::Slider, label, (String) value, color, parentControl, callback);
    ESPUI.addControl(ControlType::Min, label, String(minValue), ControlColor::None, controlID);
    ESPUI.addControl(ControlType::Max, label, String(maxValue), ControlColor::None, controlID);
  } else {
    controlID = ESPUI.addControl(ControlType::Number, label, (String) value, color, parentControl, callback);
  }
  
  return controlID;
}

void UI_addControlsTop() {
  // tabs  
  UI_tabG = ESPUI.addControl(ControlType::Tab, UI_msg_tab_general, UI_msg_tab_general, ControlColor::Emerald, Control::noParent, &UI_callTab);
  UI_tabA = ESPUI.addControl(ControlType::Tab, UI_msg_tab_alarms, UI_msg_tab_alarms, ControlColor::Emerald, Control::noParent, &UI_callTab);

  #ifdef ANNOUNCE_MODE
    UI_tabAn = ESPUI.addControl(ControlType::Tab, UI_msg_tab_announcements, UI_msg_tab_announcements, ControlColor::Emerald, Control::noParent, &UI_callTab);
  #endif

  // controls above the tabs
  UI_ID_info0 = ESPUI.addControl(ControlType::Label, UI_msg_info0, UI_getInfo(0), ControlColor::None, Control::noParent);
  
  #ifdef DISPLAY_SYSTEM_INFO
    UI_ID_info1 = ESPUI.addControl(ControlType::Label, UI_msg_info1, UI_getInfo(1), ControlColor::None, Control::noParent);
  #endif
  
  UI_ID_saveToEEPROM = ESPUI.addControl(ControlType::Button, UI_msg_saveToEEPROM, UI_msg_click, ControlColor::Emerald, Control::noParent, &UI_callButton);
  UI_ID_resync = ESPUI.addControl(ControlType::Button, UI_msg_resync, UI_msg_click, ControlColor::Emerald, Control::noParent,&UI_callButton);
  UI_ID_interruptAlarm = ESPUI.addControl(ControlType::Button, UI_msg_interruptAlarm, UI_msg_click, ControlColor::Alizarin, Control::noParent, &UI_callButton);
  UI_ID_restart = ESPUI.addControl(ControlType::Button, UI_msg_restart, UI_msg_click, ControlColor::Alizarin, Control::noParent, &UI_callButton);  
}

void UI_addControlsMain() {  
  UI_ID_timeZone = UI_addSlider(UI_msg_timeZone, timeZone, -11, 14, ControlColor::Wetasphalt, UI_tabG, &UI_callNumber);
  UI_ID_DSTmode = ESPUI.addControl(ControlType::Select, UI_msg_DSTmode, (String) DSTmode, ControlColor::Wetasphalt, UI_tabG, &UI_callOption);
  for (int i = 0; i < (FN_ARRAY_SIZE(UI_msg_DSTmodes)); i++) {      
    charPtr = UI_msg_DSTmodes[i].c_str();
    ESPUI.addControl(ControlType::Option, charPtr, (String) i, ControlColor::Wetasphalt, UI_ID_DSTmode);
  }    

  UI_ID_timeDisplayMode = ESPUI.addControl(ControlType::Select, UI_msg_timeDisplayMode, (String) timeDisplayMode, ControlColor::Wetasphalt, UI_tabG, &UI_callOption);
  for (int i = 0; i < (FN_ARRAY_SIZE(UI_msg_timeDisplayModes)); i++) {      
    charPtr = UI_msg_timeDisplayModes[i].c_str();
    ESPUI.addControl(ControlType::Option, charPtr, (String) i, ControlColor::Wetasphalt, UI_ID_timeDisplayMode);
  }    

  UI_ID_ampmMode = ESPUI.addControl(ControlType::Switcher, UI_msg_ampmMode, (String) ampmMode, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
  
  if (UI_controls_minimal) {
    UI_tmpMsg2 = (String) UI_msg_sleepMode + " (" + (String) sleepDelay + " seconds)";
    charPtr = UI_tmpMsg2.c_str();
    UI_ID_sleepMode = ESPUI.addControl(ControlType::Switcher, charPtr, (String) sleepMode, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    
    UI_tmpMsg1 = (String) UI_msg_displayInfo + " (" + (String) displayInfoInterval + " seconds)";
    charPtr = UI_tmpMsg1.c_str();
    UI_ID_displayInfo = ESPUI.addControl(ControlType::Switcher, charPtr, (String) displayInfo, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);    
  } else {
    UI_ID_sleepMode = ESPUI.addControl(ControlType::Switcher, UI_msg_sleepMode, (String) sleepMode, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    UI_ID_sleepDelay = ESPUI.addControl(ControlType::Number, UI_msg_sleepDelay, (String) sleepDelay, ControlColor::Wetasphalt, UI_tabG, &UI_callNumber);
    UI_ID_sleepStart = ESPUI.addControl(ControlType::Button, UI_msg_sleepStart, UI_msg_click, ControlColor::Emerald, UI_tabG, &UI_callButton);  
    UI_ID_suspendMode = ESPUI.addControl(ControlType::Switcher, UI_msg_suspendMode, (String) suspendMode, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    UI_ID_displayInfo = ESPUI.addControl(ControlType::Switcher, UI_msg_displayInfo, (String) displayInfo, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    UI_ID_displayInfoInterval = ESPUI.addControl(ControlType::Number, UI_msg_displayInfoInterval, (String) displayInfoInterval, ControlColor::Wetasphalt, UI_tabG, &UI_callNumber);
  }

  #ifdef DAYLIGHTINFO
    UI_ID_displayDaylight = ESPUI.addControl(ControlType::Switcher, UI_msg_displayDaylight, (String) displayDaylight, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
  #endif

  #ifdef ENABLE_BME280
    UI_ID_displayBME280 = ESPUI.addControl(ControlType::Switcher, UI_msg_displayBME280, (String) BME280_display, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    UI_ID_BME280_fahrenheid = ESPUI.addControl(ControlType::Switcher, UI_msg_BME280_fahrenheid, (String) BME280_fahrenheid, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    
    if (LED_NUM_OF_HOR_DISPLAYS < 8) {      // don't display when using 8 (or more) matrices, the temp will appear on the right 4 matrices
      UI_ID_BME280_tempTimeSwap = ESPUI.addControl(ControlType::Switcher, UI_msg_BME280_tempTimeSwap, (String) BME280_tempTimeSwap, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
    }
  #endif

  #ifdef ENABLE_LIGHTSENSOR
    UI_ID_brightnessAuto = ESPUI.addControl(ControlType::Switcher, UI_msg_brightnessAuto, (String) !ledBrightnessManualSet, ControlColor::Wetasphalt, UI_tabG, &UI_callSwitch);
  #endif 
  
  UI_ID_brightness = UI_addSlider(UI_msg_brightness, ledBrightness, ls_minValue, ls_maxValue, ControlColor::Wetasphalt, UI_tabG, &UI_callNumber);

  #ifdef ENABLE_NEOPIXELS
    UI_ID_neoBacklight = UI_addSlider(UI_msg_neoBacklight, neoBacklight, 0, numOfNeoPalette, ControlColor::Wetasphalt, UI_tabG, &UI_callNumber);
  #endif

  if (UI_controls_extended) {
    UI_ID_resetWifi = ESPUI.addControl(ControlType::Button, UI_msg_resetWifi, UI_msg_click, ControlColor::Alizarin, UI_tabG, &UI_callButton);  
  }
}

void UI_addControlsAlarms() {
  UI_ID_pad = ESPUI.addControl(ControlType::Pad, UI_msg_pad, "", ControlColor::Wetasphalt, UI_tabA, &UI_callPad);
  UI_ID_testAlarm = ESPUI.addControl(ControlType::Button, UI_msg_testAlarm, UI_msg_click, ControlColor::Alizarin, UI_tabA, &UI_callButton);
  
  UI_ID_alarmNumber = ESPUI.addControl(ControlType::Label, UI_msg_alarmNumber, (String) UI_alarmNum, ControlColor::Wetasphalt, UI_tabA);
  UI_ID_alarmActive = ESPUI.addControl(ControlType::Switcher, UI_msg_alarmActive, (String) alarmList[UI_alarmNum].active, ControlColor::Wetasphalt, UI_tabA, &UI_callSwitch);
  UI_ID_message = ESPUI.addControl(ControlType::Text, UI_msg_message, (String) alarmList[UI_alarmNum].message, ControlColor::Wetasphalt, UI_tabA, &UI_callText);
  
  UI_ID_hour = UI_addSlider(UI_msg_hour, alarmList[UI_alarmNum].hour, 0, 23, ControlColor::Peterriver, UI_tabA, &UI_callNumber);
  UI_ID_min = UI_addSlider(UI_msg_min, alarmList[UI_alarmNum].min, 0, 59, ControlColor::Peterriver, UI_tabA, &UI_callNumber);
  UI_ID_sec = UI_addSlider(UI_msg_sec, alarmList[UI_alarmNum].sec, 0, 59, ControlColor::Peterriver, UI_tabA, &UI_callNumber);
  
  if (!UI_controls_minimal) {
    UI_ID_month = UI_addSlider(UI_msg_month, alarmList[UI_alarmNum].month, 0, 12, ControlColor::Peterriver, UI_tabA, &UI_callNumber);
    UI_ID_day = UI_addSlider(UI_msg_day, alarmList[UI_alarmNum].day, 0, 31, ControlColor::Peterriver, UI_tabA, &UI_callNumber);
  }
  
  UI_ID_duration = ESPUI.addControl(ControlType::Number, UI_msg_duration, (String) alarmList[UI_alarmNum].duration, ControlColor::Peterriver, UI_tabA, &UI_callNumber);

  UI_ID_weekDays = ESPUI.addControl(ControlType::Switcher, UI_msg_weekDays, (String) alarmList[UI_alarmNum].weekDays, ControlColor::Turquoise, UI_tabA, &UI_callSwitch);
  UI_ID_weekendDays = ESPUI.addControl(ControlType::Switcher, UI_msg_weekendDays, (String) alarmList[UI_alarmNum].weekendDays, ControlColor::Turquoise, UI_tabA, &UI_callSwitch);
  UI_ID_weekDay = UI_addSlider(UI_msg_weekDay, alarmList[UI_alarmNum].weekDay, 0, 7, ControlColor::Turquoise, UI_tabA, &UI_callNumber);

  if (!UI_controls_minimal) {
    UI_ID_recurringType = ESPUI.addControl(ControlType::Select, UI_msg_recurringType, (String) alarmList[UI_alarmNum].recurringType, ControlColor::Turquoise, UI_tabA, &UI_callNumber);
    for (int i = 0; i < (FN_ARRAY_SIZE(UI_msg_recurringTypes)); i++) {      
      charPtr = UI_msg_recurringTypes[i].c_str();
      ESPUI.addControl(ControlType::Option, charPtr, (String) i, ControlColor::Turquoise, UI_ID_recurringType);    
    }
    UI_ID_recurringInterval = ESPUI.addControl(ControlType::Number, UI_msg_recurringInterval, (String) alarmList[UI_alarmNum].recurringInterval, ControlColor::Turquoise, UI_tabA, &UI_callNumber);
  }
  
  UI_ID_buzzer  = UI_addSlider(UI_msg_buzzer, alarmList[UI_alarmNum].buzzer, 0, 19, ControlColor::Carrot, UI_tabA, &UI_callNumber);  
  UI_ID_bright = ESPUI.addControl(ControlType::Switcher, UI_msg_bright, (String) alarmList[UI_alarmNum].bright, ControlColor::Carrot, UI_tabA, &UI_callSwitch);
  
  UI_ID_fx = ESPUI.addControl(ControlType::Select, UI_msg_fx, (String) alarmList[UI_alarmNum].fx, ControlColor::Carrot, UI_tabA, &UI_callNumber);
  ESPUI.addControl(ControlType::Option, UI_msg_fx0, "0", ControlColor::Carrot, UI_ID_fx);
  ESPUI.addControl(ControlType::Option, UI_msg_fx1, "1", ControlColor::Carrot, UI_ID_fx);
  
  UI_ID_actionType = ESPUI.addControl(ControlType::Select, UI_msg_actionType, (String) alarmList[UI_alarmNum].actionType, ControlColor::Carrot, UI_tabA, &UI_callNumber);
  
  #ifdef ENABLE_NEOPIXELS
    int j = (FN_ARRAY_SIZE(UI_msg_actionTypes));
  #else
    int j = (FN_ARRAY_SIZE(UI_msg_actionTypes)) - 4;
  #endif
  
  for (int i = 0; i < j; i++) {      
    charPtr = UI_msg_actionTypes[i].c_str();
    ESPUI.addControl(ControlType::Option, charPtr, (String) i, ControlColor::Carrot, UI_ID_actionType);    
  }    
  UI_ID_actionNum = UI_addSlider(UI_msg_actionNum, alarmList[UI_alarmNum].actionNum, 0, 40, ControlColor::Carrot, UI_tabA, &UI_callNumber);  
  
  if (UI_controls_minimal)
    UI_numOfControls = UI_ID_actionNum;
  else {
    UI_ID_alarmsReset = ESPUI.addControl(ControlType::Button, UI_msg_alarmsReset, UI_msg_click, ControlColor::Alizarin, UI_tabA, &UI_callButton);
    UI_numOfControls = UI_ID_alarmsReset;
  }
}

void UI_addControlsAnnounce() {
  #ifdef ANNOUNCE_MODE    
    UI_ID_announcements = ESPUI.addControl(ControlType::Select, UI_msg_announcements, "", ControlColor::Sunflower, UI_tabAn, &UI_callOption);

    for (int i = 0; i < (FN_ARRAY_SIZE(announcements) - 1); i++) {      
      charPtr = announcements[i].c_str();
      ESPUI.addControl(ControlType::Option, charPtr, (String) i, ControlColor::Wetasphalt, UI_ID_announcements);
    }    
  
    UI_ID_customAnnouncement = ESPUI.addControl(ControlType::Text, UI_msg_customAnnouncement, (String) customAnnouncement, ControlColor::Sunflower, UI_tabAn, &UI_callText);
    UI_ID_announceMode = ESPUI.addControl(ControlType::Switcher, UI_msg_announceMode, (String) announceMode, ControlColor::Sunflower, UI_tabAn, &UI_callSwitch);

    UI_numOfControls = UI_ID_announceMode;    
  #endif
}

// reread functions

void UI_rereadTop() {
  ESPUI.updateText(UI_ID_info0, UI_getInfo(0));
  ESPUI.updateText(UI_ID_info1, UI_getInfo(1));
}

void UI_rereadMain() {
  ESPUI.updateSlider(UI_ID_timeZone, timeZone);
  ESPUI.updateSelect(UI_ID_DSTmode, (String) DSTmode);
  ESPUI.updateSwitcher(UI_ID_ampmMode, ampmMode);
  ESPUI.updateSelect(UI_ID_timeDisplayMode, (String) timeDisplayMode);
  ESPUI.updateSwitcher(UI_ID_displayInfo, displayInfo);
  ESPUI.updateNumber(UI_ID_displayInfoInterval, displayInfoInterval);  
  ESPUI.updateSwitcher(UI_ID_suspendMode, suspendMode);
  ESPUI.updateSwitcher(UI_ID_sleepMode, sleepMode);
  ESPUI.updateNumber(UI_ID_sleepDelay, sleepDelay);  
  UI_rereadLS();
  #ifdef DAYLIGHTINFO
    ESPUI.updateSwitcher(UI_ID_displayDaylight, displayDaylight);
  #endif
  #ifdef ENABLE_BME280
    ESPUI.updateSwitcher(UI_ID_displayBME280, BME280_display);
    ESPUI.updateSwitcher(UI_ID_BME280_fahrenheid, BME280_fahrenheid);
    ESPUI.updateSwitcher(UI_ID_BME280_tempTimeSwap, BME280_tempTimeSwap);
  #endif  
  #ifdef ENABLE_NEOPIXELS
    ESPUI.updateSlider(UI_ID_neoBacklight, neoBacklight);
  #endif
}

void UI_rereadLS() {
  #ifdef ENABLE_LIGHTSENSOR
    ESPUI.updateSwitcher(UI_ID_brightnessAuto, !ledBrightnessManualSet);
  #endif
  ESPUI.updateSlider(UI_ID_brightness, ledBrightness);
}

void UI_rereadAlarms() {
  String tmpMsg = "";
  if (alarmTriggered && UI_alarmNum == alarmNum)
    tmpMsg = " TRIGGERED";
  else if (forceFirstAlarm)
    tmpMsg = " TESTING";

  if (alarmRepeat)
    tmpMsg += " RECURRING";
    
  ESPUI.print(UI_ID_alarmNumber, (String) (UI_alarmNum + 1) + tmpMsg);
  ESPUI.updateText(UI_ID_message, (String) alarmList[UI_alarmNum].message);
  ESPUI.updateSwitcher(UI_ID_alarmActive, alarmList[UI_alarmNum].active);
  ESPUI.updateSlider(UI_ID_hour, alarmList[UI_alarmNum].hour);
  ESPUI.updateSlider(UI_ID_min, alarmList[UI_alarmNum].min);
  ESPUI.updateSlider(UI_ID_sec, alarmList[UI_alarmNum].sec);
  ESPUI.updateSlider(UI_ID_month, alarmList[UI_alarmNum].month);
  ESPUI.updateSlider(UI_ID_day, alarmList[UI_alarmNum].day);
  ESPUI.updateSlider(UI_ID_weekDay, alarmList[UI_alarmNum].weekDay);
  ESPUI.updateNumber(UI_ID_duration, alarmList[UI_alarmNum].duration);
  ESPUI.updateSwitcher(UI_ID_weekDays, alarmList[UI_alarmNum].weekDays);
  ESPUI.updateSwitcher(UI_ID_weekendDays, alarmList[UI_alarmNum].weekendDays);
  ESPUI.updateSlider(UI_ID_recurringType, alarmList[UI_alarmNum].recurringType);
  ESPUI.updateNumber(UI_ID_recurringInterval, alarmList[UI_alarmNum].recurringInterval);
  ESPUI.updateSlider(UI_ID_buzzer, alarmList[UI_alarmNum].buzzer);
  ESPUI.updateSelect(UI_ID_fx, (String) alarmList[UI_alarmNum].fx);
  ESPUI.updateSwitcher(UI_ID_bright, alarmList[UI_alarmNum].bright);  
  ESPUI.updateSelect(UI_ID_actionType, (String) alarmList[UI_alarmNum].actionType);
  ESPUI.updateSlider(UI_ID_actionNum, alarmList[UI_alarmNum].actionNum);
}

void UI_rereadAnnounce() {
  #ifdef ANNOUNCE_MODE
    ESPUI.updateSelect(UI_ID_announcements, (String) announceItem);
    ESPUI.updateText(UI_ID_customAnnouncement, (String) customAnnouncement);
    ESPUI.updateSwitcher(UI_ID_announceMode, announceMode);
  #endif
}

void UI_rereadTab(int tabCode) { 
  UI_rereadTop();

  if (UI_activeTab == -1 || tabCode == -1) {     // the active tab is undefined so reread all controls or reread all is requested
    UI_rereadMain();
    UI_rereadAlarms();
    UI_rereadAnnounce();
  } else if (UI_activeTab == UI_tabG)
    UI_rereadMain();
  else if (UI_activeTab == UI_tabA)    
    UI_rereadAlarms();
  else if (UI_activeTab == UI_tabAn)
    UI_rereadAnnounce();
}

// callback functions

String UI_getInfo(int type) {
  String tmp;

  if (type == 0) {
    tmp = days[timeData.tm_wday] + " " + fillZero(timeData.tm_mday) + "-" + fillZero(timeData.tm_mon + 1) + "-" + (String) (timeData.tm_year + 1900);
    tmp += " " + fillZero(getPMhour()) + ":" + fillZero(timeData.tm_min)+ ":" + fillZero(timeData.tm_sec);
  
    if (ampmMode) {
      if (pm) tmp += " PM";
      else    tmp += " AM";
    }

    if (sleeping) 
      tmp += " - " + (String) UI_msg_info_sleeping;

    if (timeManualSet) {
      tmp += "<br>" + (String) UI_msg_info_manualSet;
    } else {
      tmp += "<br>" + (String) UI_msg_info_timeZone + " " + (String) timeZone + " ";
      tmp += (DST ? msgDSTon : msgDSToff);
      if (DSTmode == 2) 
        tmp += " (auto)";    
    }
    
    #ifdef DAYLIGHTINFO
      tmp += "<br>" + (String) d2d_City + " : " + daylightInfo;
    #endif  
  
    #ifdef ENABLE_BME280
      char charBuffer[10];  
      sprintf(charBuffer, "%3.1f", BME280_temperature);
      tmp += "<br>" + (String) msgbmeTemperature + ' ' + (String) charBuffer + (BME280_fahrenheid ? "F " : "C ");
      tmp += (String) msgbmeHumidity + ' ' +  (String) int((BME280_humidity + .5)) + "% ";
      tmp += (String) msgbmePressure + ' ' +  (String) int((BME280_pressure + .5)) + " hPa ";
      
      #ifdef BME280_SEA_LEVEL_PRESSURE
        sprintf(charBuffer, "%2.1f", BME280_altitude);
        tmp += (String) charBuffer + " mtr ";
      #endif
    #endif

    #ifndef DISPLAY_SYSTEM_INFO
      tmp += "<br>version " + (String) _VERSION + " - " + (String) _DATE + " by " + (String) _AUTHOR;
    #endif

    if (alarmTriggered)
      tmp += "<br>* Alarm " + (String) (alarmNum + 1) + " triggered *";
    else if (forceFirstAlarm)
      tmp += "<br>* Alarm " + (String) (alarmNum + 1) + " testing *";
  
    if (alarmRepeat)
      tmp += " recurring *";

  } else if (type == 1) {
    tmp+= "ericBclock - " + (String) _LANGUAGE + " - version " + (String) _VERSION + " - " + (String) _DATE + " by " + (String) _AUTHOR + "<br>" + setFeatures;
    tmp+= "-UI controls: " + (String) UI_numOfControls;
  
    if (UI_controls_extended || UI_controls_minimal || UI_controls_extended_sliders) {
      tmp+= " (";
      if (UI_controls_extended) tmp+= "ext";
      if (UI_controls_minimal)  tmp+= "min";
      if (UI_controls_extended_sliders) {
        if (UI_controls_extended || UI_controls_minimal)
          tmp+= "-sli";
        else
          tmp+= "sli";
      }
      tmp+= ")";  
    }
    
    tmp+= "<br>Wifi SSID: " + (String) UI_ssid + " IP: " + WiFi.softAPIP().toString();
    tmp+= "<br>lan: " + lanIP.toString();
  }
  
  return tmp;
}

void UI_callTab(Control *sender, int type) {
  DEBUGPRINTLN("UI - Selected tab " + (String) sender->id);
  UI_activeTab = sender->id;  
  UI_rereadTab (-1); // FIX: error: too few arguments to function 'void UI_rereadTab(int)'
}

void UI_callButton(Control *sender, int type) { 
  if (type == B_UP) {
    if (sender->id == UI_ID_sleepStart && sleeping) {   // when clicking the start sleep mode button and already sleeping, end sleepmode and return to prevent retriggering
      playBuzzer(20); 
      endSleepMode(true);
      return;
    }
    
    endSleepMode(true);
    
    if (sender->id == UI_ID_saveToEEPROM) {
      DEBUGPRINTLN("UI - Save button clicked");
      eepromWrite();
      endSleepMode(false);
	    playBuzzer(21);
//      ESPUI.updateText(UI_ID_info, UI_msg_txt_saved);
      
    } else if (sender->id == UI_ID_resetWifi) {
      DEBUGPRINTLN("UI - Reset wifi button clicked");
      UI_sw_resetEEPROM();
      playBuzzer(20);
//      ESPUI.updateText(UI_ID_info, msgUIwifiReset);      
//      displayMessageOutLoop(0, msgUIwifiReset, false);  // @EB-todo
      ESP.restart();
      
    } else if (sender->id == UI_ID_interruptAlarm) {
      DEBUGPRINTLN("UI - Alarm interrupt button clicked");
      forceFirstAlarm = false;
      UI_alarmToInterrupt = true;
      playBuzzer(20);
      
    } else if (sender->id == UI_ID_testAlarm) {
      DEBUGPRINTLN("UI - Test alarm button clicked " + (String) UI_alarmNum);
      if (alarmTriggered)
        UI_alarmToInterrupt = true;  
      else {
        if (forceFirstAlarm)
          forceFirstAlarm = false;
        else {
          forceFirstAlarm = true;
          alarmNum = UI_alarmNum;
        }
      }
      playBuzzer(20);
      
    } else if (sender->id == UI_ID_alarmsReset) {
      DEBUGPRINTLN("UI - Alarms reset button clicked");
      eepromInitialize(1);
      eepromRead();
//      UI_rereadTab();
//      ESPUI.updateText(UI_ID_info, UI_msg_txt_reset);
      playBuzzer(20);
            
    } else if (sender->id == UI_ID_resync) { 
      DEBUGPRINTLN("UI - Resync time button clicked");
      playBuzzer(20);
      UI_displayTimeRead  = true;
      UI_timeToResync = true;
      
    } else if (sender->id == UI_ID_restart) {
      DEBUGPRINTLN("UI - Restart button clicked");
      playBuzzer(20);
      ESP.restart();
      
    } else if (sender->id == UI_ID_sleepStart) {
      DEBUGPRINTLN("UI - Start sleep mode button clicked");
      playBuzzer(20); 
      
      if (sleeping)
        forceEndSleepMode = true;
      else
        forceStartSleepMode = true;
    }

  }
}

void UI_callText(Control *sender, int type) {
  eepromChanged();
  endSleepMode(true);
  
  if (sender->id == UI_ID_message) {    
    sender->value.toCharArray(alarmList[UI_alarmNum].message, (alarmMsgLength - 2)); 
    ESPUI.updateText(UI_ID_message, (String) alarmList[UI_alarmNum].message);  
  } else if (sender->id == UI_ID_customAnnouncement) { 
    sender->value.toCharArray(customAnnouncement, (customAnnouncementLength - 2)); 
  }
}

void UI_callNumber(Control *sender, int type) {
  #ifdef UI_CONTROLS_EXTENDED_SLIDERS
    // @EB-todo 20201113: ESPUI slider bug, the callback function is called 11 times !
    // so check previous call id and value to prevent retriggering
    static int prevID, prevValue;
    if (prevID == sender->id && prevValue == sender->value.toInt())
      return;
    prevID = sender->id;
    prevValue = sender->value.toInt();
  #endif
  
  eepromChanged();
  endSleepMode(true);
  
  if      (sender->id == UI_ID_timeZone)             { timeZone = UI_validateNum(-11, 14, sender->value); timeManualSet = false; UI_timeToResync = true; }
  else if (sender->id == UI_ID_displayInfoInterval)  { displayInfoInterval = UI_validateNum(30, 32766, sender->value); displayInfoStartTime = millis(); }
  else if (sender->id == UI_ID_sleepDelay)           { sleepDelay = UI_validateNum(0, 32766, sender->value);                                }  
  else if (sender->id == UI_ID_hour)                 { alarmList[UI_alarmNum].hour = UI_validateNum(0, 23, sender->value);                  }
  else if (sender->id == UI_ID_min)                  { alarmList[UI_alarmNum].min = UI_validateNum(0, 59, sender->value);                   }
  else if (sender->id == UI_ID_sec)                  { alarmList[UI_alarmNum].sec = UI_validateNum(0, 59, sender->value);                   }
  else if (sender->id == UI_ID_month)                { alarmList[UI_alarmNum].month = UI_validateNum(0, 12, sender->value);                 }
  else if (sender->id == UI_ID_duration)             { alarmList[UI_alarmNum].duration = UI_validateNum(0, 32766, sender->value);           }
  else if (sender->id == UI_ID_weekDay)              { alarmList[UI_alarmNum].weekDay = UI_validateNum(0, 7, sender->value);                alarmRecTriggered[UI_alarmNum] = false; }
  else if (sender->id == UI_ID_recurringType)        { alarmList[UI_alarmNum].recurringType = UI_validateNum(0, 5, sender->value);          alarmRecTriggered[UI_alarmNum] = false; }
  else if (sender->id == UI_ID_recurringInterval)    { alarmList[UI_alarmNum].recurringInterval = UI_validateNum(0, 32766, sender->value);  alarmRecTriggered[UI_alarmNum] = false; }
  else if (sender->id == UI_ID_buzzer)               { alarmList[UI_alarmNum].buzzer = UI_validateNum(0, 19, sender->value);                }
  else if (sender->id == UI_ID_fx)                   { alarmList[UI_alarmNum].fx = UI_validateNum(0, 1, sender->value);                     }
  else if (sender->id == UI_ID_actionType)           { alarmList[UI_alarmNum].actionType = UI_validateNum(0, FN_ARRAY_SIZE(UI_msg_actionTypes), sender->value); initAction(UI_alarmNum); }

  else if (sender->id == UI_ID_actionNum || sender->id == UI_ID_actionType) {
    bool valid = true;
    if (alarmList[UI_alarmNum].actionType == actionGPIOhigh || alarmList[UI_alarmNum].actionType == actionGPIOlow) {
      switch (sender->value.toInt()) {
        case PIN_CS: case PIN_BUZZER: case RE_PINA: case RE_PINB: case RE_SWITCH: case PIN_LS:
          #ifdef ENABLE_PIR
            case PIN_PIR: 
          #endif
          #ifdef ENABLE_NEOPIXELS
            case PIN_NEOPIXELS:
          #endif
        
          valid = false;
      }      
    } else if (alarmList[UI_alarmNum].actionType == actionNeoOn) {
      #ifdef ENABLE_NEOPIXELS
        alarmList[UI_alarmNum].actionNum = UI_validateNum(0, numOfNeoPalette, sender->value);        
      #endif        
    }
    
    if (valid) {      
      alarmList[UI_alarmNum].actionNum = UI_validateNum(0, 99, sender->value);      
      initAction(UI_alarmNum);        
    } else {
      playBuzzer(20);
      ESPUI.updateSlider(UI_ID_actionNum, alarmList[UI_alarmNum].actionNum);
    }    
  }
  
  else if (sender->id == UI_ID_brightness) {   
    ledBrightness= UI_validateNum(ls_minValue, ls_maxValue, sender->value);
    ledBrightnessManualSet = true;
    matrix.setIntensity(ledBrightness);

    #ifdef ENABLE_LIGHTSENSOR
      ESPUI.updateSwitcher(UI_ID_brightnessAuto, !ledBrightnessManualSet);
    #endif
  }

  else if (sender->id == UI_ID_day) {
    if (alarmList[UI_alarmNum].month)
      alarmList[UI_alarmNum].day = UI_validateNum(0, getDaysInMonth(timeData.tm_year, alarmList[UI_alarmNum].month), sender->value);
    else
      alarmList[UI_alarmNum].day = UI_validateNum(0, 31, sender->value);
  }

  #ifdef ENABLE_NEOPIXELS
    else if (sender->id == UI_ID_neoBacklight) { 
      neoBacklight = UI_validateNum(0, numOfNeoPalette, sender->value);
      neopixelsPresetOn(neoBacklight);
    }
  #endif

//  UI_rereadTab();
}

int UI_validateNum(int minVal, int maxVal, String value) {
  int tmpInt = value.toInt();
//  tmpInt = max(minVal, tmpInt);
//  tmpInt = min(maxVal, tmpInt);

  if (tmpInt < minVal) {
    tmpInt = minVal;
    playBuzzer(21);
  }
  if (tmpInt > maxVal) {
    tmpInt = maxVal;
    playBuzzer(21);
  }
  return tmpInt;
}

void UI_callOption(Control *sender, int value) {
  static int prevAnnounceItem = 0;
  eepromChanged();    
  endSleepMode(true);
  
  if (sender->id == UI_ID_DSTmode) { 
    DSTmode = sender->value.toInt(); 
    timeManualSet = false; 
    UI_timeToResync = true;   
  } else if (sender->id == UI_ID_timeDisplayMode) { 
    timeDisplayMode = sender->value.toInt(); 
    refreshTimeDisplay = true;
    refreshTempSwapTime = 2;
  } 

  #ifdef ENABLE_NEOPIXELS
    if (sender->id == UI_ID_announcements) { 
      announceItem = sender->value.toInt();
      if (announceItem != prevAnnounceItem) {
        customAnnouncement[0] = 0; 
        ESPUI.updateText(UI_ID_customAnnouncement, (String) customAnnouncement);
      }
    }
  #endif
}

void UI_callSwitch(Control *sender, int value) {
  eepromChanged();
  endSleepMode(true);
  
  bool result = (value == S_ACTIVE ? true : false);
  
  if      (sender->id == UI_ID_ampmMode)            { ampmMode = result; getPMhour(); refreshTimeDisplay = true; }
  else if (sender->id == UI_ID_displayInfo)         { displayInfo = result;                                }
  else if (sender->id == UI_ID_suspendMode)         { suspendMode = result;                                }
  else if (sender->id == UI_ID_sleepMode)           { sleepMode = result; endSleepMode(true);              }
  else if (sender->id == UI_ID_brightnessAuto)      { ledBrightnessManualSet = !result;                    }
  else if (sender->id == UI_ID_weekDays)            { alarmList[UI_alarmNum].weekDays = result;            alarmRecTriggered[UI_alarmNum] = false;  }
  else if (sender->id == UI_ID_weekendDays)         { alarmList[UI_alarmNum].weekendDays = result;         alarmRecTriggered[UI_alarmNum] = false;  }
  else if (sender->id == UI_ID_bright)              { alarmList[UI_alarmNum].bright = result;              }
  else if (sender->id == UI_ID_displayDaylight)     { displayDaylight = result;                            }
  else if (sender->id == UI_ID_displayBME280)       { BME280_display = result; BME280_startTime = 0;       }
  else if (sender->id == UI_ID_BME280_fahrenheid)   { BME280_fahrenheid = result; BME280_startTime = 0;    }
  else if (sender->id == UI_ID_BME280_tempTimeSwap) { BME280_tempTimeSwap = result;                        } 
  
  else if (sender->id == UI_ID_announceMode) {
    announceMode = result;
    if (strlen(customAnnouncement)) {
      customAnnounceMsg = true;
    } else 
      customAnnounceMsg = false;
  }
    
  else if (sender->id == UI_ID_alarmActive) {
    alarmList[UI_alarmNum].active = result;
    if (result) {

      if (UI_controls_minimal) {                      // if the alarm is set active and not all attributes are displaying, set them to 0 to avoid unexpected problems
        alarmList[UI_alarmNum].month = 0;
        alarmList[UI_alarmNum].day = 0;
        alarmList[UI_alarmNum].recurringType = 0;
        alarmList[UI_alarmNum].recurringInterval = 0;
      }
      
      alarmRecTriggered[UI_alarmNum] = false;
      initAction(UI_alarmNum);
    } else {
      if (alarmTriggered && alarmNum == UI_alarmNum)
        UI_alarmToInterrupt = true;
    }
  }
  
  else if (sender->id == UI_ID_sleepMode)
    DEBUGPRINTLN(sleepMode ? "UI - Sleep mode activated" : "UI - Sleep mode deactivated");
    
  else if (sender->id == UI_ID_suspendMode)
    DEBUGPRINTLN(suspendMode ? "UI - Suspend mode activated" : "UI - Suspend mode deactivated");
}

void UI_callPad(Control *sender, int value) {
  endSleepMode(true);
  
  switch (value) {
    case P_LEFT_UP:
      if (UI_alarmNum == 0)
        UI_alarmNum = maxAlarms - 1;
      else
        UI_alarmNum--;
      break;
      
    case P_RIGHT_UP:
      UI_alarmNum++;
      UI_alarmNum = UI_alarmNum % maxAlarms;
      break;
      
    case P_FOR_UP:
      UI_alarmNum = 0;
      break;
      
    case P_BACK_UP:
      UI_alarmNum = maxAlarms - 1;
      break;
  }

  UI_rereadAlarms();
}
/*
***************************************************************************
  ebc_alarmclock - debug set alarms
***************************************************************************
  last update 20201129 by ericBcreator
***************************************************************************
*/

// hour 6 min 10 duration 30 buzzer 4 effect 1 bright

void debugPrintAlarmlist() {
  byte tmpMaxAlarms = ((eepromBufSize - sizeof(UI_sw_EEPROM) - sizeof(eepromInit)) / sizeof(alarmList[0]));
  DEBUGPRINTLN();
  DEBUGPRINT("EEPROM size " + (String) eepromBufSize + " bytes, init size " + (String) (sizeof(UI_sw_EEPROM) + sizeof(eepromInit)));
  DEBUGPRINT (", alarm element size " + (String) sizeof(alarmList[0]) + " bytes, max number of alarms " + (String) tmpMaxAlarms);
  DEBUGPRINTLN();

  for (int tmpAlarm = 0; tmpAlarm < maxAlarms; tmpAlarm++) {
    DEBUGPRINT("Alarm " + fillSpace((tmpAlarm + 1), 2) + "    ");
    DEBUGPRINT(alarmList[tmpAlarm].active ? "active  ": "inactive");
    DEBUGPRINT(" set at " + fillZero(alarmList[tmpAlarm].hour) + ":" + fillZero(alarmList[tmpAlarm].min) + ":" + fillZero(alarmList[tmpAlarm].sec) + " ");
    DEBUGPRINT(" month " + fillSpace(alarmList[tmpAlarm].month, 2) + " day " + fillSpace(alarmList[tmpAlarm].day, 2) + " weekday " + (String) alarmList[tmpAlarm].weekDay);
    DEBUGPRINT(" duration " + fillSpace(alarmList[tmpAlarm].duration, 5) + " ");
    DEBUGPRINT("recurring " + (String) alarmList[tmpAlarm].recurringType + " " + fillSpace(alarmList[tmpAlarm].recurringInterval,5) + " ");
    DEBUGPRINT("buzzer " + fillSpace(alarmList[tmpAlarm].buzzer, 2) + " ");
    DEBUGPRINT("fx " + (String) alarmList[tmpAlarm].fx + " ");
    DEBUGPRINT("action " + (String) alarmList[tmpAlarm].actionType + " " + fillSpace(alarmList[tmpAlarm].actionNum, 2) + " ");
    DEBUGPRINT(alarmList[tmpAlarm].weekDays ? "weekdays " : "");
    DEBUGPRINT(alarmList[tmpAlarm].weekendDays ? "weekenddays " : "");
    DEBUGPRINT(alarmList[tmpAlarm].bright ? "bright message: " : "message: ");
    DEBUGPRINT(alarmList[tmpAlarm].message);
    DEBUGPRINTLN();
  }
  DEBUGPRINTLN();
}

void debugSetAlarms() {
  for (int tmpAlarm = 0; tmpAlarm < 8; tmpAlarm++) {
    alarmList[tmpAlarm].hour = timeData.tm_hour;
    alarmList[tmpAlarm].min = timeData.tm_min;
    int tmpSec = timeData.tm_sec + (60+30 * (tmpAlarm + 1));
    debugValidateTime(alarmList[tmpAlarm].hour, alarmList[tmpAlarm].min, tmpSec);
    alarmList[tmpAlarm].sec = tmpSec;

    alarmList[tmpAlarm].duration = defaultAlarmDuration;
    alarmList[tmpAlarm].buzzer = 2;
    alarmList[tmpAlarm].active = true;
    String tmpMsg = "";
  
    switch (tmpAlarm) {		
      case 1 :
        alarmList[tmpAlarm].actionType = actionGPIOhigh;
        alarmList[tmpAlarm].actionNum = 12; // 2 equals LED_BUILTIN 12 equals D6
        alarmList[tmpAlarm].recurringType = 1;
        alarmList[tmpAlarm].recurringInterval = 40;
        alarmList[tmpAlarm].buzzer = 0;
        alarmList[tmpAlarm].duration = 10;
        tmpMsg = "GPIO D6";
        break;
        
      case 2 : 
        alarmList[tmpAlarm].recurringType = 2;
        alarmList[tmpAlarm].recurringInterval = 2;
        alarmList[tmpAlarm].weekendDays = true;
        tmpMsg = "recur 2 min weekend";
        break;
        
      case 3 :
        alarmList[tmpAlarm].recurringType = 1;
        alarmList[tmpAlarm].recurringInterval = 135;
        tmpMsg = "recur 135 sec";
        break;
        
      case 4 :
        alarmList[tmpAlarm].recurringType = 4;
        alarmList[tmpAlarm].recurringInterval = 4;
        tmpMsg = "recur 4 day";
        break;

      case 5 :
        alarmList[tmpAlarm].recurringType = 5;
        alarmList[tmpAlarm].recurringInterval = 3;
        tmpMsg = "recur 3 months";
        break;

      case 6:              
        alarmList[tmpAlarm].recurringType = 2;
        alarmList[tmpAlarm].recurringInterval = 2;
        alarmList[tmpAlarm].day = 6;
        tmpMsg = "Sat recur 2 hour";
        break;
        
      case 7:  
        alarmList[tmpAlarm].weekDay = 7;
        alarmList[tmpAlarm].weekDays = true;
        alarmList[tmpAlarm].recurringType = 1;
        alarmList[tmpAlarm].recurringInterval = 5;
        tmpMsg =  "Sat & weekdays recur 5 min";
        break;
    }

    if (tmpAlarm > 1) {
      tmpMsg = "al " + (String) tmpAlarm + " " + tmpMsg;
      tmpMsg.toCharArray(alarmList[tmpAlarm].message, (alarmMsgLength - 2));
    }
  } 

  alarmList[0].buzzer = 4;
  alarmList[0].fx = 1;
  alarmList[0].bright = true;
  alarmList[0].weekDays = false;
  alarmList[0].weekendDays = false;

#ifndef DEBUG_NO_WRITE_ALARMS_TO_EEPROM
  eepromInitialize(0);
  eepromWrite();
#endif
}

void debugValidateTime(byte &vtHour, byte &vtMin, int &vtSec) {
  while (vtSec > 59) {
    vtMin++;
    vtSec -= 60;
  }
  
  while (vtMin > 59) {
    vtHour = min(vtHour + 1, 23);
    vtMin -= 60;
  }
}
