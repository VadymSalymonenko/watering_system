#include "set_help_functions.h"
byte knotSet(byte stringAddress){
    #ifdef DEBUGPRINTS 
    Serial.println("knotSet"); 
    #endif
    byte mode = 0; //menu item currently on the screen
    byte deepOfRecursion = numberOfDigits(userPath);
    byte quantity = mainMenuArray[stringAddress][0].toInt();
    boolean push = 0;
    lcd.clear();
    lcd.print(mainMenuArray[stringAddress][1]);
    
    if(typeOfBackButtonClick = 1 && deepOfRecursion == 0) typeOfBackButtonClick = 0;
    while(true){
      #ifdef DEBUGPRINTS 
      Serial.println("recursion"); 
      #endif
        if(SetupButtonCheck() == 0)break;    
        menuUpdate(mode, mainMenuArray[stringAddress], quantity);

        //////////
        if(userPathMemory > 9){       
            mode = (userPathMemory%10)-1;
            push = 1;
            userPathMemory = userPathMemory/10;
            }else{userPathMemory = 0;}
        /////////
        if(backButtonCheck())break;
        if(setButton.isSingle()|| push == 1){   //if "enter" is pressed
              push = 0;
              if(mainMenuArray[stringAddress][(mode*2+2)].toInt() >= 40){
                  userPath += (mode+1) * int(ceil(pow(10,(deepOfRecursion))));         
                  chooseSet(userPath, mainMenuArray[stringAddress][(mode*2+2)].toInt());
                    #ifdef DEBUGPRINTS 
                    Serial.println("a step f");  
                    #endif             

                  }
            if(mainMenuArray[stringAddress][(mode*2+2)].toInt() < 40) {    
                  userPath += (mode+1) * int(ceil (pow(10,(deepOfRecursion))));
                  knotSet(mainMenuArray[stringAddress][(mode*2+2)].toInt());
                    #ifdef DEBUGPRINTS 
                    Serial.println("b step f");  
                    #endif             

                  }
                    #ifdef DEBUGPRINTS 
                    Serial.println("c step f");  
                    #endif             

              if(typeOfBackButtonClick == 1)break;         
             }
        }
       #ifdef DEBUGPRINTS 
       Serial.println("return 0"); 
       #endif
       return 0;
    } 
void chooseSet(long userPath, byte outValue){
  switch(outValue){
     case 50: 
       switch (userPath%10){
           case 1: saveSetZone(userPath,0,59);break;
           //case 2: saveSet(0,59,tankMinutesMemory);break;
        }
       break;
     case 51:
       switch (userPath%10){
           case 1: saveSetZone(userPath,0,23);break;
           //case 2: saveSet(0,23,tankHoursMemory);break;
        }
       break;
     case 52: 
     Serial.print("userPath == "); 
           Serial.println(userPath);
        switch (userPath%10){
           case 1: saveBoolSetZone(userPath);break;
           case 2: saveBoolSetZone(userPath);break;
           case 4: saveSet(rainSensorIsTurnedOnMemory_cell);break;
           ; 
           default: break;
        }
        break;
     case 53: saveSet(tankModeMemory_cell);break; // boolean saveSet
     case 54: dateSaveSet(); break;
     case 55: saveSetZone(userPath,1,14); break;  //14 - maximum number of days to pause
     case 56: timeSaveSet();break;
     case 57: tankCustomTimeSet(); break;//
     case 58: handControlOfMotor(userPath); break;//
     case 59: calibrationMotor(userPath); break;//
     case 60: systemStatusShow(); break;//
     case 61: systemLogsShow(); break;
     case 62: setMotorPosition(userPath, CLOSE); break;
     case 63: setMotorPosition(userPath, OPEN); break;

  }
    #ifdef DEBUGPRINTS 
    Serial.println("choose set f"); 
    #endif               
}
//------------------------------
//Help functions
int read_potent (int interval){
     int value  =  analogRead(Poten_pin);
     return constrain(map(value, 0, 1024, 0, interval + 1),0,interval);
  } 
int read_potent (int lowerLimit,int upperLimit){
     int value  =  analogRead(Poten_pin);
     return constrain(map(value, 0, 1024, lowerLimit, upperLimit+1),lowerLimit,upperLimit);
  }
byte numberOfDigits(long num){
    byte digits = 0;
    for(;num>0; digits++){
      num/=10;
    }
    return digits;
}
void LCDprintResult (byte firstValue, byte secondValue, byte setCursorColumn,byte setCursorInLine){
  // outputs result - time
    lcd.clear ();
    lcd.print("save ");
    if(firstValue >=10){
        lcd.setCursor(setCursorInLine,setCursorColumn);
        lcd.print(firstValue);
    }
    if(firstValue <=9){
        lcd.setCursor(setCursorInLine + 1,setCursorColumn);
        lcd.print(firstValue);
        lcd.setCursor(setCursorInLine,setCursorColumn);
        lcd.print("0");
    }
    lcd.setCursor(setCursorInLine + 2,setCursorColumn); 
    lcd.print(".");
    if(secondValue <=9){
        lcd.setCursor(setCursorInLine + 3,setCursorColumn); 
        lcd.print("0");
        lcd.setCursor(setCursorInLine + 4,setCursorColumn);
        lcd.print(secondValue);
    } 
    if(secondValue >=10){ 
        lcd.setCursor(setCursorInLine + 3,setCursorColumn);
        lcd.print(secondValue);
    }
}
//MenuPrinters-----------------
void menuInitialize(byte mode, String items[]){
    lcd.clear();
    lcd.print(items[mode]);
}
boolean menuUpdate(byte &mode, String items[], byte quantity){
   if(quantity > 1){;
   if(mode!=read_potent(quantity-1)){  //If the user moved the potentiometer - update the screen
       mode = read_potent(quantity-1);
       lcd.clear();
       lcd.print(items[mode*2+1]);
       return 1;   
   }
   }
   return 0;
   
}
//----------------------------
//value set menu printers-----
void printValue(byte value, byte line, byte column){
    if(value >=10){
        lcd.setCursor(line,column);
        lcd.print(value);
    }
    if(value <=9){
        lcd.setCursor(line + 1,column);
        lcd.print(value);
        lcd.setCursor(line,column);
        lcd.print("0");
    }
  }
void printValue_3(byte value, byte line, byte column){
    if(value >=100){
        lcd.setCursor(line,column);
        lcd.print(value%100);
        printValue((byte)(value-(value%100)*100), line+1,column);
    }else{
        lcd.setCursor(line,column);
        lcd.print("0");
        printValue((byte)value, line+1,column);
    }
}
void printFormattedValue(byte &value,byte lowerLimit,byte upperLimit){
  if(value != read_potent(lowerLimit,upperLimit)){
    value = read_potent(lowerLimit,upperLimit);
    printValue(value,5,1);
    }
  }
  
// overload printValue for boolean value
void printValue(bool value, byte line, byte column){
  if(backButtonCheck()){return;}
  lcd.setCursor(line,column);
  if(value==0) lcd.print("ON ");
  if(value==1) lcd.print("OFF");
}
//overload printFormattedValue for boolean value
void printFormattedValue(bool &value){
  if(backButtonCheck()){return;}
  if(value != read_potent(1)){
    value = read_potent(1);
    printValue(value,5,1);
  }
}
//----------------------------
// Setters
void saveSet(byte lowerLimit,byte upperLimit, byte memoryCell){
  #ifdef DEBUGPRINTS 
  Serial.println("saveSet start"); 
  #endif
  byte value = read_potent(lowerLimit,upperLimit);
  bool savePermit = 1;
  printValue(value,5,1); // initialization
  while(true){
    printFormattedValue(value,lowerLimit,upperLimit);
    if(setButton.isSingle())break;
    if(backButtonCheck()==1){savePermit =0;break;}
  }
  if(savePermit == 1){ 
    lcd.clear();
    lcd.print("saved: ");
    printValue(value,7,0);
    EEPROM.update(memoryCell,value);
    //if(memoryCell == tankMinutesMemory || memoryCell == tankHoursMemory)EEPROM.update(tankModeMemory_cell,1); // даже для Вадима это костыль. Нужно переделать меню
    while(true){
       if(backButtonCheck()==1)break;
       }
   }  
}
int boolsaveSetReturn(){
    bool value = read_potent(1);
    bool savePermit = 1;
    printValue(value,5,1);
    while(true){
        #ifdef DEBUGPRINTS 
        Serial.println("bool save Set return"); 
        #endif
        if(backButtonCheck()==1){savePermit =0;break;}
        printFormattedValue(value);
        if(setButton.isSingle()) break;
    }
    if(savePermit == 1){  
        lcd.clear();
        lcd.print("saved: ");
        printValue(value,7,0);
    //  Serial.println("save");  
      //  EEPROM.update(memoryCell,!value);
      return !value;
        while(true){
           if(backButtonCheck()==1)break;
        }
     }
     return -1;
   #ifdef DEBUGPRINTS 
   Serial.println("bool save Set return final"); 
   #endif
}

// saveSet overload for boolean value
void saveSet(byte memoryCell){
    bool value = read_potent(1);
    bool savePermit = 1;
    printValue(value,5,1);
    while(true){
        #ifdef DEBUGPRINTS 
        Serial.println("save Set bool"); 
        #endif
        if(backButtonCheck()==1){savePermit =0;break;}
        printFormattedValue(value);
        if(setButton.isSingle()) break;
    }
    if(savePermit == 1){  
        lcd.clear();
        lcd.print("saved: ");
        printValue(value,7,0);
    //  Serial.println("save");  
        EEPROM.update(memoryCell,!value);
        while(true){
           if(backButtonCheck()==1)break;
        }
     }
   #ifdef DEBUGPRINTS 
   Serial.println("save Set bool final"); 
   #endif
}
// version of saveSet with return value instead of writing to memory
byte saveSetReturn(byte lowerLimit,byte upperLimit){
    byte value = 61;//value = -1;???
    bool savePermit = 1;
    
    while(true){
        printFormattedValue(value,lowerLimit,upperLimit);
        if(setButton.isSingle())break;
        if(backButtonCheck()==1){savePermit =0;break;}
    }
    if(savePermit == 1){  
        lcd.clear();
        lcd.print("saved: ");
        printValue(value,7,0);
    //  Serial.println("save");
            return value;       
    }
    return 100;
}
void dateSaveSet(){
  bool savePermit = 1;
  byte DaysValue, MonthValue, YearsValue = 0;
  lcd.clear();
  lcd.print("date: ??.??.20  ");
  lcd.setCursor(13,1); lcd.print("^^");
  while(savePermit){
    if(YearsValue != read_potent(0,50)){
        YearsValue = read_potent(0,50);
        printValue(YearsValue,14,0);}
    if(setButton.isSingle())break; 
    if(backButtonCheck()==1){savePermit =0;return;}
  }
  lcd.setCursor(9,1); lcd.print("^^     ");
  while(savePermit){
    if(MonthValue != read_potent(1,12)){
        MonthValue = read_potent(1,12);
        printValue(MonthValue,9,0);}
    if(setButton.isSingle())break;
    if(backButtonCheck()==1){savePermit =0;return;}
  }
  byte maxDay = daysInMonth(MonthValue, YearsValue);//determine the number of days in this month
  lcd.setCursor(6,1); lcd.print("^^    ");
  while(savePermit){
    if(DaysValue != read_potent(1,maxDay)){
        DaysValue = read_potent(1,maxDay);
        printValue(DaysValue,6,0);}
    if(setButton.isSingle())break;
    if(backButtonCheck()==1){savePermit =0;return;}
    }
  if(savePermit == 1){   
    lcd.setCursor(6,1);lcd.print("  ");
    lcd.setCursor(0,0);lcd.print("saved");
    t.Day = DaysValue;
    t.Month = MonthValue;
    t.Year = CalendarYrToTm(2000+YearsValue);
    RTC.write(t);
    //rtc.setDate(DaysValue, MonthValue, 2000+YearsValue);
      while(true){
    if(backButtonCheck()==1){break;}
      }
  }
  //Сохранение в память
}
byte daysInMonth(byte currentMonth, byte currentYear){
  switch(currentMonth){
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      return 31; break;
    case 4: case 6: case 9: case 11:
      return 30; break;
    case 2:
      if((currentYear%100==0)&&(currentYear%400!=0)) return 28;
      if(currentYear%4==0) return 29;
      return 28;
      break;
  }
}
void saveMotorPosition(Motor &currMotor, boolean actionType, int value){ 
    if(actionType == OPEN){
        currMotor.openPosition = value;
        EEPROM.put((currMotor.motorNumber * 10) + 202, value);
    }else{
        currMotor.closePosition = value;
        EEPROM.put((currMotor.motorNumber * 10) + 204, value);
    }
}
void setMotorPosition(long userPath, boolean actionType){
  bool savePermit = 1;

  int positionResult = -1;
  int zoneOrTankNumber = -1;

  if(userPath/1000 == 0){  zoneOrTankNumber = 0;
  }else{zoneOrTankNumber = (userPath/10)%10;}

  
  if(zoneOrTankNumber == -1){
  lcd.clear();
  lcd.print("user path error");
    while(savePermit){
      if(backButtonCheck()==1){savePermit =0;return;}
    }
  }

  
  Serial.print("setMotorPosition == ");
  Serial.println(userPath);

  lcd.clear();
  lcd.print("curr point save?");
  lcd.setCursor(6,1); 
  lcd.print("loading");
  switch(zoneOrTankNumber){
    case 0: positionResult = checkPositionOfMotor(tankMotor); break;
    case 1: positionResult = checkPositionOfMotor(zone1.motor); break;
    case 2: positionResult = checkPositionOfMotor(zone2.motor); break;
    case 3: positionResult = checkPositionOfMotor(zone3.motor); break;
  } 
  lcd.setCursor(6,1); 
  if(positionResult == -1){
    lcd.print("disabled");
  }else{
    lcd.print("       ");
    lcd.setCursor(6,1); 
    lcd.print(positionResult);
  }
  delay(50);
  while(savePermit){
    if(setButton.isSingle())break; 
    if(backButtonCheck()==1){savePermit =0;return;}
  }
  if(savePermit == 1){  
    switch(zoneOrTankNumber){
      case 0: saveMotorPosition(tankMotor, actionType, positionResult); break;
      case 1: saveMotorPosition(zone1.motor, actionType, positionResult); break;
      case 2: saveMotorPosition(zone2.motor, actionType, positionResult); break;
      case 3: saveMotorPosition(zone3.motor, actionType, positionResult); break;
    } 
    lcd.setCursor(6,1);lcd.print("      ");
    lcd.setCursor(0,0);lcd.print("saved           ");
    while(true){
      if(backButtonCheck()==1){break;}
    }
  }
}

void timeSaveSet(){
  bool savePermit = 1;
  byte minutesValue, hoursValue = 0;
  lcd.clear();
  lcd.print("time:   .??");
  lcd.setCursor(6,1);
  lcd.print("^^");
  while(savePermit){
    if(hoursValue != read_potent(0,23)){
        hoursValue = read_potent(0,23);
        printValue(hoursValue,6,0);}
    if(setButton.isSingle())break; 
    if(backButtonCheck()==1){savePermit =0;return;}
  }
  lcd.setCursor(6,1); lcd.print("   ^^");
  while(savePermit){
    if(minutesValue != read_potent(0,59)){
        minutesValue = read_potent(0,59);
        printValue(minutesValue,9,0);}
    if(setButton.isSingle())break;
    if(backButtonCheck()==1){savePermit =0;return;}
  }
  if(savePermit == 1){   
    lcd.setCursor(6,1);lcd.print("  ");
    lcd.setCursor(0,0);lcd.print("saved");
    t.Hour = hoursValue;
    t.Minute = minutesValue;
    t.Second = 0;
    //rtc.setTime(hoursValue,minutesValue,0);
    RTC.write(t);
      while(true){
    if(backButtonCheck()==1){break;}
      }
  }
}
void tankCustomTimeSet(){
  bool savePermit = 1;
  byte startMinutesValue, startHoursValue = 0;
  byte stopMinutesValue, stopHoursValue = 0;
  lcd.clear();
  lcd.print("   :??--??:??");
  lcd.setCursor(1,1); lcd.print("^^");
  while(savePermit){//стартовые часы
    if(startHoursValue != read_potent(0,23)){
      startHoursValue = read_potent(0,23);
      printValue(startHoursValue,1,0);
    }
    if(setButton.isSingle())break; 
    if(backButtonCheck()==1){savePermit=0;return;}
  }
  lcd.setCursor(1,1); lcd.print("   ^^");
  while(savePermit){//стартовые минуты
    if(startMinutesValue != read_potent(0,59)){
        startMinutesValue = read_potent(0,59);
        printValue(startMinutesValue,4,0);}
    if(setButton.isSingle())break;
    if(backButtonCheck()==1){savePermit =0;return;}
  }
  lcd.setCursor(4,1); lcd.print("    ^^");
  while(savePermit){//финишные часы
    if(stopHoursValue != read_potent(0,23)){
      stopHoursValue = read_potent(0,23);
      printValue(stopHoursValue,8,0);
    }
    if(setButton.isSingle())break; 
    if(backButtonCheck()==1){savePermit=0;return;}
  }
  lcd.setCursor(8,1); lcd.print("   ^^");
  while(savePermit){//финишные минуты
    if(stopMinutesValue != read_potent(0,59)){
        stopMinutesValue = read_potent(0,59);
        printValue(stopMinutesValue,11,0);}
    if(setButton.isSingle())break;
    if(backButtonCheck()==1){savePermit =0;return;}
  }

  if(savePermit == 1){   
    lcd.setCursor(9,1);lcd.print("  ");
    lcd.setCursor(0,0);lcd.print("saved");
    EEPROM.update(tankStartHoursMemory_cell, startHoursValue);
    EEPROM.update(tankStartMinutesMemory_cell, startMinutesValue);
    EEPROM.update(tankStopHoursMemory_cell, stopHoursValue);
    EEPROM.update(tankStopMinutesMemory_cell, stopMinutesValue);
      while(true){
    if(backButtonCheck()==1){break;}
      }
  }
  #ifdef DEBUGPRINTS 
  Serial.println("1 step f");   
  #endif            
}
//saveSet for zone
void saveSetZone(long userPath,byte lowerLimit,byte upperLimit){
    byte value = 0;
    byte parameter = (int((userPath)/1000))%10;
    byte zoneNum = ((userPath)/10)%10;
    value = saveSetReturn(lowerLimit,upperLimit);
    if(value != 100){
        switch (parameter){
            case 1:  //duration
                if(upperLimit==59){EEPROM.update(zoneNum*10 + 2,value);
                }else{             EEPROM.update(zoneNum*10 + 1,value);}
            break;
            case 2://start time
                if(upperLimit==59){EEPROM.update(zoneNum*10 + 4,value);
                }else{             EEPROM.update(zoneNum*10 + 3,value);}
            break;
            case 3://pause of days
                EEPROM.update(zoneNum*10,value);
        }
        while(true){
            if(backButtonCheck()==1)break; 
      
        }
    }
}
// boolean saveSet for zone
void saveBoolSetZone(long userPath){
   boolean value = false;
    if(userPath == 132){
      value = boolsaveSetReturn();  // saving the tap type for the tank (on/off potentiometer)
      if(value != -1){
        EEPROM.update(0 * 10 + 201,value);
        tankMotor.connectedPotent = value;
      }
    }

    if((userPath%1000)/100 == 1){
      saveSet((((userPath)/10)%10) * 10 + 5); // saving the mode of irrigation zones (on off)
    }
    if((userPath%1000)/100 == 4){
      value = boolsaveSetReturn();   // save tap type (on/off potentiometer)
      if(value != -1){
        EEPROM.update((((userPath)/10)%10) * 10 + 201, value);
        switch((((userPath)/10)%10)){
            case 1: zone1.motor.connectedPotent = value; break;
            case 2: zone2.motor.connectedPotent = value; break;
            case 3: zone3.motor.connectedPotent = value; break;
        }
      }
    }
}

void handControlOfMotor(int userPath){
    #ifdef DEBUGPRINTS 
    Serial.println("handControlOfMotor"); 
    #endif
    byte currentModeHandControl = 0;
    byte currentZone = 0;
    byte lastStatus = 99;
    
    currentZone = (userPath%100)/10;
    lcd.setCursor(0,0); lcd.print("    open tap");
    lcd.setCursor(0,1); lcd.print("    close tap");
        
    while(true){
    currentModeHandControl = read_potent(0,1);
    if(lastStatus != currentModeHandControl){
        if(currentModeHandControl == 0){lcd.setCursor(0,0); lcd.print("->"); lcd.setCursor(0,1); lcd.print("  ");}
        if(currentModeHandControl == 1){lcd.setCursor(0,0); lcd.print("  "); lcd.setCursor(0,1); lcd.print("->");}
        lastStatus = currentModeHandControl;
    }
    if(setButton.isSingle()){
       if(currentModeHandControl == 0  ){
           switch(currentZone){
               case 1: 
                   if(zone1.isInWateringProcess){warningHandControl(0); return;} 
                   handControlOpen(currentZone); return; break;
               case 2:
                   if(zone2.isInWateringProcess){warningHandControl(0); return;} 
                   handControlOpen(currentZone); return; break;
               case 3:
                   if(zone3.isInWateringProcess){warningHandControl(0); return;} 
                   handControlOpen(currentZone); return; break;
      /*         case 4:
                   if(zone4.isInWateringProcess){warningHandControl(0); return;} 
                   handControlOpen(currentZone); return; break;*/

          }
       }
       if(currentModeHandControl == 1  ){
           switch(currentZone){
               case 1: 
                   if(!zone1.isInWateringProcess){warningHandControl(1); return;} 
                   handControlClose(currentZone); return; break;
               case 2:
                   if(!zone2.isInWateringProcess){warningHandControl(1); return;} 
                   handControlClose(currentZone);
                   return;
                   break;
               case 3:
                   if(!zone3.isInWateringProcess){warningHandControl(1); return;} 
                   handControlClose(currentZone);
                   return;
                   break;
      /*         case 4:
                   if(!zone4.isInWateringProcess){warningHandControl(1); return;} 
                   handControlClose(currentZone);
                   return;
                   break;*/ 
           }
       }
    }
    if(backButtonCheck()==1){break;}
    }
}
void warningHandControl(boolean type){
    if(type == 0){
    lcd.clear();
    lcd.print("  this zone is");
    lcd.setCursor(0,1);
    lcd.print("  already open");
    }
    if(type == 1){
    lcd.clear();
    lcd.print("  this zone is");
    lcd.setCursor(0,1);
    lcd.print(" already closed");
    }
    while(true){
        if(backButtonCheck()==1){break;}
    }
}
void handControlOpen(int currentZone){   
    lcd.clear();
    lcd.print("    opens");
    delay(5);
    switch(currentZone){
        case 1: zone1.startWatering(); break;
        case 2: zone2.startWatering(); break;
        case 3: zone3.startWatering(); break;
        case 4: zone4.startWatering(); break;
    }
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("    done");
    while(true){
        if(backButtonCheck()==1){break;}
    }
}
void handControlClose(int currentZone){
    lcd.clear();
    lcd.print("    closes");
    delay(5);
    switch(currentZone){
        case 1: zone1.stopWatering(); break;
        case 2: zone2.stopWatering(); break;
        case 3: zone3.stopWatering(); break;
        case 4: zone4.stopWatering(); break;
    }
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("    done");
    while(true){
        if(backButtonCheck()==1){break;}
    }
}
void calibrationMotor(int userPath){
    #ifdef DEBUGPRINTS 
        Serial.println("calibrationMotor"); 
    #endif
    byte currentModeCalibrationMotor = 0;
    byte currentZoneOrTank = 0;
    byte lastStatus = 99;
    int stepCoefficient = 0;


    currentZoneOrTank = (userPath%100)/10;
    if(userPath == 32)currentZoneOrTank = 0; // if path is 32, then we calibrate the tank. tank - 0; zone_1 - 1; zone_2 - 2...
    lcd.setCursor(0,0); lcd.print("  +5   +10   +25");
    lcd.setCursor(0,1); lcd.print("  -5   -10   -25");
        
    while(true){
        currentModeCalibrationMotor = read_potent(0,5);
        if(lastStatus != currentModeCalibrationMotor){
            if(currentModeCalibrationMotor == 0){stepCoefficient = 5;   lcd.setCursor(0,0); lcd.print(">"); lcd.setCursor(5,0); lcd.print(" ");}
            if(currentModeCalibrationMotor == 1){stepCoefficient = 10;  lcd.setCursor(5,0); lcd.print(">"); lcd.setCursor(0,0); lcd.print(" "); lcd.setCursor(11,0); lcd.print(" ");}
            if(currentModeCalibrationMotor == 2){stepCoefficient = 25;  lcd.setCursor(11,0); lcd.print(">"); lcd.setCursor(5,0); lcd.print(" "); lcd.setCursor(0,1); lcd.print(" ");}
            if(currentModeCalibrationMotor == 3){stepCoefficient = -5;  lcd.setCursor(0,1); lcd.print(">"); lcd.setCursor(11,0); lcd.print(" "); lcd.setCursor(5,1); lcd.print(" ");}
            if(currentModeCalibrationMotor == 4){stepCoefficient = -10; lcd.setCursor(5,1); lcd.print(">"); lcd.setCursor(0,1); lcd.print(" "); lcd.setCursor(11,1); lcd.print(" ");}
            if(currentModeCalibrationMotor == 5){stepCoefficient = -25; lcd.setCursor(11,1); lcd.print(">"); lcd.setCursor(5,1); lcd.print(" ");} 
            lastStatus = currentModeCalibrationMotor;
        }
        if(setButton.isSingle()){            
            if(currentZoneOrTank  == 0){ 
                if(isInFillingTankProcess == true){ warningCalibrationMotor(); return;}
                moveTap(tankMotor, stepCoefficient*364); // 364 motor steps for 1 degree of crane rotation
            }
            if(currentZoneOrTank  == 1){
                    if(zone1.isInWateringProcess){warningCalibrationMotor(); return;}
                    actionCalibrationMotor();
                    zone1.moveTapExactAngle(stepCoefficient*364); 
                    lastStatus = 99;
                    actionInitCalibrationMotor();
            }
            if(currentZoneOrTank  == 2){
                    if(zone2.isInWateringProcess){warningCalibrationMotor(); return;}
                    actionCalibrationMotor();
                    zone2.moveTapExactAngle(stepCoefficient*364);
                    lastStatus = 99;
                    actionInitCalibrationMotor();
            }
            if(currentZoneOrTank  == 3){
                    if(zone3.isInWateringProcess){warningCalibrationMotor(); return;}
                    actionCalibrationMotor();
                    zone3.moveTapExactAngle(stepCoefficient*364);
                    lastStatus = 99;
                    actionInitCalibrationMotor();
            }
            if(currentZoneOrTank  == 4){
                    if(zone4.isInWateringProcess){warningCalibrationMotor(); return;}
                    actionCalibrationMotor();
                    zone4.moveTapExactAngle(stepCoefficient*364);
                    lastStatus = 99;
                    actionInitCalibrationMotor();
            }
  
            }   
        if(backButtonCheck()==1){break;}
    }
}
void systemStatusShow(){
    byte currentInfo = read_potent(1,30);
    byte last_info_mode = 255;

    zone1.updateSettings();
    zone2.updateSettings();
    zone3.updateSettings();
    
    while(true){
        currentInfo = read_potent(1,30);
        if(last_info_mode == currentInfo) continue;
        last_info_mode = currentInfo;
        lcd.clear();
        printValue(currentInfo,0,0);
        lcd.setCursor(2,0);
        lcd.print(": ");
        lcd.setCursor(5,0);
        
        switch(currentInfo){
             case 1:  lcd.print("T ="); printValue(t.Hour,9,0);                                                                    //point 1 -- current time
                      lcd.setCursor(11,0); lcd.print(":");
                      printValue(t.Minute,12,0); break;
             case 2:  lcd.print("millis ="); lcd.setCursor(0,1); lcd.print(millis()); break;                                       //point 2 -- millis
             case 3:  lcd.print(" ---- = ");lcd.setCursor(13,0); lcd.print("?");  break;                                           //point 3 -- no info
             case 4:  lcd.print("tankS = "); lcd.setCursor(13,0); lcd.print(tank_status);  break;                                  // tank status
             
             case 5:  lcd.print("z11>"); lcd.setCursor(9,0);lcd.print(zone1.isInWateringProcess); break;                           // zone1 isInWateringProcess
             case 6:  lcd.print("z12>"); lcd.setCursor(9,0);lcd.print(zone1.timeToWateringStart(t.Hour*60 + t.Minute)); break;     // zone1 timeToWateringStart
             case 7:  lcd.print("z13>"); lcd.setCursor(9,0);lcd.print(zone1.endTime); break;                                       // zone1 endTime
             case 8:  lcd.print("z14>"); lcd.setCursor(9,0);lcd.print(zone1.daysToWatering); break;                                // zone1 daysToWatering
             case 9:  lcd.print("z15>"); lcd.setCursor(9,0);lcd.print(zone1.startTime); break;                                     // zone1 startTime
             case 10: lcd.print("z16>"); lcd.setCursor(9,0);lcd.print(zone1.duration); break;                                      // zone1 duration
             case 11: lcd.print("z17>"); lcd.setCursor(9,0);lcd.print(zone1.pauseOfDays); break;                                   // zone1 pauseOfDays
             case 12: lcd.print("z18>"); lcd.setCursor(9,0);lcd.print(zone1.isTurnedOn); break;                                    // zone1 isTurnedOn
              
             case 13: lcd.print("z21>"); lcd.setCursor(9,0);lcd.print(zone2.isInWateringProcess); break;                           // zone2 isInWateringProcess
             case 14: lcd.print("z22>"); lcd.setCursor(9,0);lcd.print(zone2.timeToWateringStart(t.Hour*60 + t.Minute)); break;     // zone2 timeToWateringStart
             case 15: lcd.print("z23>"); lcd.setCursor(9,0);lcd.print(zone2.endTime); break;                                       // zone2 endTime
             case 16: lcd.print("z24>"); lcd.setCursor(9,0);lcd.print(zone2.daysToWatering); break;                                // zone2 daysToWatering
             case 17: lcd.print("z25>"); lcd.setCursor(9,0);lcd.print(zone2.startTime); break;                                     // zone2 startTime
             case 18: lcd.print("z26>"); lcd.setCursor(9,0);lcd.print(zone2.duration); break;                                      // zone2 duration
             case 19: lcd.print("z27>"); lcd.setCursor(9,0);lcd.print(zone2.pauseOfDays); break;                                   // zone2 pauseOfDays
             case 20: lcd.print("z28>"); lcd.setCursor(9,0);lcd.print(zone2.isTurnedOn); break;                                    // zone2 isTurnedOn
             
             case 21: lcd.print("z31>"); lcd.setCursor(9,0);lcd.print(zone3.isInWateringProcess); break;                           // zone3 isInWateringProcess
             case 22: lcd.print("z32>"); lcd.setCursor(9,0);lcd.print(zone3.timeToWateringStart(t.Hour*60 + t.Minute)); break;     // zone3 timeToWateringStart
             case 23: lcd.print("z33>"); lcd.setCursor(9,0);lcd.print(zone3.endTime); break;                                       // zone3 endTime
             case 24: lcd.print("z34>"); lcd.setCursor(9,0);lcd.print(zone3.daysToWatering); break;                                // zone3 daysToWatering
             case 25: lcd.print("z35>"); lcd.setCursor(9,0);lcd.print(zone3.startTime); break;                                     // zone3 startTime
             case 26: lcd.print("z36>"); lcd.setCursor(9,0);lcd.print(zone3.duration); break;                                      // zone3 duration
             case 27: lcd.print("z37>"); lcd.setCursor(9,0);lcd.print(zone3.pauseOfDays); break;                                   // zone3 pauseOfDays
             case 28: lcd.print("z38>"); lcd.setCursor(9,0);lcd.print(zone3.isTurnedOn); break;                                    // zone3 isTurnedOn
             
             case 29: lcd.print("dTr>"); printValue((byte)days_to_reload,9,0); break;                                              // days_to_reload
             case 30: lcd.print("mDays>"); printValue((byte)millis_days,9,0); break;                                               // millis_days
             /*when adding new items, increase the reading values from the potentiometer*/ 
        }
        if(backButtonCheck()==1){break;}
    }    
}

void systemLogsShow(){
    byte currentLog = read_potent(1,30);
    byte last_Log = 255;
    while(true){
        currentLog = read_potent(1,30);
        if(last_Log == currentLog) continue;
        last_Log = currentLog;
        lcd.clear();
        printValue(currentLog,0,0);
        lcd.setCursor(2,0);
        lcd.print(": ");
        lcd.setCursor(0,1);

        if(logList[currentLog-1][1] != 255){
            printValue_3(logList[currentLog-1][0],0,1);    //log output on the bottom line of the screen
            lcd.setCursor(3,1);
            lcd.print("-");
            printValue(logList[currentLog-1][1],4,1); 
            lcd.print("-");
            printValue(logList[currentLog-1][2],7,1);
            lcd.print("-");
            printValue_3(logList[currentLog-1][3],10,1);
        }else{
            lcd.setCursor(7,1);
            lcd.print("----");
        }
        if(backButtonCheck()==1){break;}
    }
}




void warningCalibrationMotor(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    warning!");
    lcd.setCursor(0,1);
    lcd.print("  tap is open!");
    while(true){
        if(backButtonCheck()==1){break;}
    }
}
void actionInitCalibrationMotor(){
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("  +5   +10   +25");
    lcd.setCursor(0,1); lcd.print("  -5   -10   -25");
}
void actionCalibrationMotor() {
    lcd.clear(); 
    lcd.print("in progress");
}
boolean backButtonCheck(){
   if(backButton.isSingle()){
       userPathMemory = userPath;
       userPath = 0;
       typeOfBackButtonClick = 1;
       return 1;}
   if(backButton.isHolded() || (SetupButtonCheck() == 0)){
       typeOfBackButtonClick = 1;
       userPath = 0;
       return 1;
       }else{return 0;}
  }
