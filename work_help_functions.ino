#include "work_help_functions.h"
boolean SetupButtonCheck (){
  //Serial.println("check main button -----");
  boolean check1 = 0;
  boolean check2 = 0;
  while(true){
      check1 = !digitalRead(SETUP_BUTTON);
      delay(5);
      check2 = !digitalRead(SETUP_BUTTON);
      if(check1 == check2) return check2;
  }
};
void moveTap(Motor motor, long steps){//надо ли аж long для steps?
  Stepper stepper(STEPS_PER_REV, motor.pin1, motor.pin2, motor.pin3, motor.pin4);
  digitalWrite(rele_transistor_pin,HIGH);
  delay(100);
  digitalWrite(rele_pin,Open_Rele_signal);
  stepper.setSpeed(500);
  stepper.step(steps);
  digitalWrite(motor.pin1,LOW);
  digitalWrite(motor.pin2,LOW);
  digitalWrite(motor.pin3,LOW);
  digitalWrite(motor.pin4,LOW);
  digitalWrite(rele_pin, !Open_Rele_signal);
  delay(100);
  digitalWrite(rele_transistor_pin,LOW);
}
void openTap(Motor motor){
  moveTap(motor, MOTOR_STEPS_TO_OPEN);
}
void closeTap(Motor motor){
  moveTap(motor, MOTOR_STEPS_TO_CLOSE);
}
//Tank:
Motor tankMotor(motor0_pin1, motor0_pin3, motor0_pin2, motor0_pin4);
void startFillingTank(){
  openTap(tankMotor);
  isInFillingTankProcess = true;
}
void stopFillingTank(){
  closeTap(tankMotor);
  isInFillingTankProcess = false;
}
boolean isTankFull(){
  boolean check1,check2,check3 = 0;
  while(true){
    delay(80);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check1 = digitalRead(woter_pin_1);//в нано сигнал был инвертирован
    delay(20);   
    digitalWrite(transistor_pin, LOW);
    if(SetupButtonCheck() == 1) return check1;
    delay(50); power.sleepDelay(4750); delay(50);
    if(SetupButtonCheck() == 1) return check1;
    delay(50); power.sleepDelay(4750); delay(50);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check2 = digitalRead(woter_pin_1); //в нано сигнал был инвертирован
    delay(20);   
    digitalWrite(transistor_pin, LOW);
    if(SetupButtonCheck() == 1) return check2;   
    delay(50); power.sleepDelay(4750); delay(50);
    if(SetupButtonCheck() == 1) return check2;
    delay(50); power.sleepDelay(4750); delay(50);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check3 = digitalRead(woter_pin_1);//в нано сигнал был инвертирован
    delay(20);   
    digitalWrite(transistor_pin, LOW);   
    if(check1 == check2 && check2== check3) return check1;
  }
}
boolean isTankEmpty(){
  boolean check1,check2,check3 = 0;
  while(true){
    delay(100);    
    digitalWrite(transistor_pin, HIGH);
    delay(500);  
    check1 = digitalRead(woter_pin_2);//в нано сигнал был инвертирован
    delay(20);
    digitalWrite(transistor_pin, LOW);   
    if(SetupButtonCheck() == 1) return check1; 
    delay(50); power.sleepDelay(4750); delay(50);
    if(SetupButtonCheck() == 1) return check1;
    delay(50); power.sleepDelay(4750); delay(50);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check2 = digitalRead(woter_pin_2); //в нано сигнал был инвертирован
    delay(20);
    digitalWrite(transistor_pin, LOW);  
    if(SetupButtonCheck() == 1) return check2; 
    delay(50); power.sleepDelay(4750); delay(50);
    if(SetupButtonCheck() == 1) return check2;
    delay(50); power.sleepDelay(4750); delay(50);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check3 = digitalRead(woter_pin_2);//в нано сигнал был инвертирован
    delay(20);
    digitalWrite(transistor_pin, LOW);    
    if(check1 == check2 && check2== check3) return !check1;
  }
}
bool isFillingTankTime(int currentTime){
//Возвращает true если был выбран режим afterWatering, или если текущее время совпадает с указаным в формате customTime
//Проверка режима:
  bool tankMode = EEPROM.read(tankModeMemory_cell);
  if(tankMode==1) return true;//режим afterWatering
//проверка времени:
  int startTime = EEPROM.read(tankStartHoursMemory_cell)*60 + EEPROM.read(tankStartMinutesMemory_cell);
  int stopTime = EEPROM.read(tankStopHoursMemory_cell)*60 + EEPROM.read(tankStopMinutesMemory_cell);
  //Проверка на содержание текущего времени в промежутке:
  if(stopTime>=startTime)return (startTime<=currentTime)&&(currentTime<=stopTime);
  if(stopTime<startTime){
    startTime-=stopTime;
    currentTime-=stopTime;
    if(currentTime<=0) currentTime+=MINUTES_IN_DAY;
    return (startTime<=currentTime);
  }
}
void timeTempCheck(){
    /*  RTC.read(t);
      if(t.Hour == 0 && t.Minute == 0 && t.Year < 2021){
          int difference = millis() - last_millis_check;
          unsigned long new_millis_sec = ((current_hour*60 + current_min)*60 + current_sec + (difference/1000));
          if( new_millis_sec <= 86400){
              byte hour1 = new_millis_sec/3600;
              byte min1 = (new_millis_sec - hour1*3600)/60;
              byte second1 = (new_millis_sec - hour1*3600 - min1*60);            
              //delay(100);
              t.Hour = hour1;
              t.Minute = min1;
              t.Second = second1;
              t.Day = current_date;
              t.Month = current_month;
              t.Year = CalendarYrToTm(current_year);
              RTC.write(t);
              //rtc.setTime(hour1, min1, second1); 
              //rtc.setDate(current_date, current_month, current_year);
              //delay(100);
          }else{
              //delay(100);
              t.Hour = 23;
              t.Minute = 59;
              t.Second = 58;
              t.Day = current_date;
              t.Month = current_month;
              t.Year = CalendarYrToTm(current_year);
              RTC.write(t);
              //rtc.setTime(23, 59, 58); 
              //rtc.setDate(current_date, current_month, current_year);   
              //delay(100);           
          } 
      }else{
         /* current_hour = t.Hour;  part_part1
          current_min = t.Minute;
          current_sec = t.Second;
          current_date = t.Day;
          current_month = t.mon;
          current_year = t.Year;
          last_millis_check = millis();
          current_dow = t.Day; */   
          //if(current_hour == 23 && current_min == 55){rtc_reboot();}
          //if(current_hour == 18 && current_min == 55){rtc_reboot();}
          //if(current_hour == 12 && current_min == 55){rtc_reboot();}
          //if(current_hour == 6 && current_min == 55){rtc_reboot();}
      //}
      //delay(500);   */
    RTC.read(t);
}
void rtc_reboot(){ 
    /*byte hour1 = t.Hour;
   // byte min1 = t.Minute;
   // byte second1 = t.Second;
   // byte date1 = t.Day;
    //byte month1 = t.mon;
   // byte year1 = t.Year;
    RTC.read(t);
    delay(100);
    RTC.write(t);
    //rtc.setTime(hour1, min1, second1); 
    //rtc.setDate(date1, month1, year1);
    delay(100);*/
}

void addLog(byte dayAction, byte hourAction, byte minAction, byte codeAction){
    if(logList[0][0] == 255 && logList[0][1] == 255 && logList[0][2] == 255){
        logList[0][0] = dayAction; 
        logList[0][1] = hourAction; 
        logList[0][2] = minAction; 
        logList[0][3] = codeAction; 
        return;
    }else{
        moveLogList();
        logList[0][0] = dayAction; 
        logList[0][1] = hourAction; 
        logList[0][2] = minAction; 
        logList[0][3] = codeAction;
    }
  
}
void moveLogList(){
    for(int i = size_of_log_list-2; i >=0; i--){
        if(logList[i][1] == 255) continue;
        logList[i+1][0] = logList[i][0];
        logList[i+1][1] = logList[i][1];
        logList[i+1][2] = logList[i][2];
        logList[i+1][3] = logList[i][3];
    }  
}
void clearLogList(){
    for(byte i = 0; i < size_of_log_list; i++){
        for(byte j = 0; j < 4; j++)logList[i][j] = 255;
    }
}

void reset_EEPROM_to_default_settings(){
    EEPROM.update(rainSensorIsTurnedOnMemory_cell,0);
    EEPROM.update(tankStartHoursMemory_cell,0);
    EEPROM.update(tankStartMinutesMemory_cell,30);
    EEPROM.update(tankStopHoursMemory_cell,5);
    EEPROM.update(tankStopMinutesMemory_cell,0);
    EEPROM.update(tankModeMemory_cell,0); // 1 - after watering
    for(byte i = 1; i <= quantity_of_zones; i++){
        EEPROM.update(i*10,2);                           //pause of days
        EEPROM.update(i*10+1,1);                         //duration hours
        EEPROM.update(i*10+2,30);                        //duration minutes
        EEPROM.update(i*10+3,20);                        //start time hours
        EEPROM.update(i*10+4,0);                         //start time minutes
        EEPROM.update(i*10+5,0);                         //is turned on
    }
}
 

  
