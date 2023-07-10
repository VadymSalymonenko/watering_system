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
void moveTap(Motor motor, long steps){
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


int getAverageReading(int pin) {
  const int numReadings = 10;
  int readingSum = 0; 
  for (int i = 0; i < numReadings; i++) {
    readingSum += analogRead(pin); 
    delay(1); 
  }
  int averageReading = readingSum / numReadings; 
  return averageReading; 
}


int calculateMotorDirection(bool actionToDo, Motor motor) {
  int targetPosition1;
  int minPosition;
  int maxPosition;
  if (actionToDo == OPEN) {
    targetPosition1 = motor.openPosition;
      if (motor.openPosition < motor.closePosition) {
        minPosition = targetPosition1 - 30;
        maxPosition = targetPosition1;         
      }else{
        minPosition = targetPosition1;
        maxPosition = targetPosition1 + 30;          
      }
  }
  if (actionToDo == CLOSE) {
    targetPosition1 = motor.closePosition;
      if (motor.openPosition < motor.closePosition) {
        minPosition = targetPosition1;
        maxPosition = targetPosition1 + 30;         
      }else{
        minPosition = targetPosition1 - 30;
        maxPosition = targetPosition1;          
      }
  }
  digitalWrite(motor.pin1,LOW);
  digitalWrite(motor.pin2,LOW);
  digitalWrite(motor.pin3,LOW);
  digitalWrite(motor.pin4,LOW);
  delay(5);
  int currentPosition1 = getAverageReading(motor.potenPin);

  Serial.print("currentPosition1 == ");
  Serial.println(currentPosition1);

  
    if (currentPosition1 < minPosition) {
      return 1;
    } else if (currentPosition1 > maxPosition) {
      return -1;
    } else {
      return 0;
    }   
}
int checkPositionOfMotor(Motor motor){
    if(motor.connectedPotent == false)return -1;
    digitalWrite(rele_transistor_pin,HIGH);
    delay(100);
    digitalWrite(rele_pin,Open_Rele_signal);
    delay(100);

    digitalWrite(motor.pin1,LOW);
    digitalWrite(motor.pin2,LOW);
    digitalWrite(motor.pin3,LOW);
    digitalWrite(motor.pin4,LOW);
    delay(5);
    int currentPosition1 = getAverageReading(motor.potenPin);

    delay(100);
    digitalWrite(rele_pin, !Open_Rele_signal);
    delay(100);
    digitalWrite(rele_transistor_pin,LOW); 
    return currentPosition1;
}

void moveTapWithPoten(Motor motor, boolean actionToDo){
  if(motor.openPosition == motor.closePosition) return;
  
  Stepper stepper(STEPS_PER_REV, motor.pin1, motor.pin2, motor.pin3, motor.pin4);
  digitalWrite(rele_transistor_pin,HIGH);
  delay(100);
  digitalWrite(rele_pin,Open_Rele_signal);
  delay(100);
  
  stepper.setSpeed(500);
  int tapAction = calculateMotorDirection(actionToDo, motor);
  
  while(tapAction){
    
    if(tapAction == 1){
      stepper.step(-250);
    }else{
      stepper.step(250);
    }
    tapAction = calculateMotorDirection(actionToDo, motor);
  }

  
  digitalWrite(motor.pin1,LOW);
  digitalWrite(motor.pin2,LOW);
  digitalWrite(motor.pin3,LOW);
  digitalWrite(motor.pin4,LOW);
  delay(100);
  digitalWrite(rele_pin, !Open_Rele_signal);
  delay(100);
  digitalWrite(rele_transistor_pin,LOW);
}

void openTap(Motor motor){
  if(motor.connectedPotent == false){
    moveTap(motor, MOTOR_STEPS_TO_OPEN);
  }else{
    moveTapWithPoten(motor, OPEN);
  }
}
void closeTap(Motor motor){
    if(motor.connectedPotent == false){
    moveTap(motor, MOTOR_STEPS_TO_CLOSE);
  }else{
    moveTapWithPoten(motor, CLOSE);
  }
}
//Tank:
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
    check1 = digitalRead(woter_pin_1);// signal was inverted in nano
    delay(20);   
    digitalWrite(transistor_pin, LOW);
    if(SetupButtonCheck() == 1) return check1;
    delay(50); power.sleepDelay(4750); delay(50);
    if(SetupButtonCheck() == 1) return check1;
    delay(50); power.sleepDelay(4750); delay(50);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check2 = digitalRead(woter_pin_1); // signal was inverted in nano
    delay(20);   
    digitalWrite(transistor_pin, LOW);
    if(SetupButtonCheck() == 1) return check2;   
    delay(50); power.sleepDelay(4750); delay(50);
    if(SetupButtonCheck() == 1) return check2;
    delay(50); power.sleepDelay(4750); delay(50);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check3 = digitalRead(woter_pin_1);// signal was inverted in nano
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
    check1 = digitalRead(woter_pin_2);// signal was inverted in nano
    delay(20);
    digitalWrite(transistor_pin, LOW);   
    if(SetupButtonCheck() == 1) return check1; 
    delay(50); power.sleepDelay(4750); delay(50);
    if(SetupButtonCheck() == 1) return check1;
    delay(50); power.sleepDelay(4750); delay(50);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check2 = digitalRead(woter_pin_2); // signal was inverted in nano
    delay(20);
    digitalWrite(transistor_pin, LOW);  
    if(SetupButtonCheck() == 1) return check2; 
    delay(50); power.sleepDelay(4750); delay(50);
    if(SetupButtonCheck() == 1) return check2;
    delay(50); power.sleepDelay(4750); delay(50);
    digitalWrite(transistor_pin, HIGH);
    delay(500);
    check3 = digitalRead(woter_pin_2);// signal was inverted in nano
    delay(20);
    digitalWrite(transistor_pin, LOW);    
    if(check1 == check2 && check2== check3) return !check1;
  }
}
bool isFillingTankTime(int currentTime){
//Returns true if the afterWatering mode was selected, or if the current time matches the one specified in the customTime format
//Mode check:
  bool tankMode = EEPROM.read(tankModeMemory_cell);
  if(tankMode==1) return true;//режим afterWatering
// time check:
  int startTime = EEPROM.read(tankStartHoursMemory_cell)*60 + EEPROM.read(tankStartMinutesMemory_cell);
  int stopTime = EEPROM.read(tankStopHoursMemory_cell)*60 + EEPROM.read(tankStopMinutesMemory_cell);
  //Check if the current time is in the interval:
  if(stopTime>=startTime)return (startTime<=currentTime)&&(currentTime<=stopTime);
  if(stopTime<startTime){
    startTime-=stopTime;
    currentTime-=stopTime;
    if(currentTime<=0) currentTime+=MINUTES_IN_DAY;
    return (startTime<=currentTime);
  }
}
void timeTempCheck(){
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
    EEPROM.update(tankModeMemory_cell,0);                // 1 - after watering
    for(byte i = 1; i <= QUANTITY_OF_ZONES; i++){
        EEPROM.update(i*10,2);                           //pause of days
        EEPROM.update(i*10+1,1);                         //duration hours
        EEPROM.update(i*10+2,30);                        //duration minutes
        EEPROM.update(i*10+3,20);                        //start time hours
        EEPROM.update(i*10+4,0);                         //start time minutes
        EEPROM.update(i*10+5,0);                         //is turned on
    }
        for(byte i = 0; i < QUANTITY_OF_MOTORS; i++){
        EEPROM.update(i*10 + 201,1);                     // motor connectedPotent
        EEPROM.put(i*10 + 202,370);                      // motor openPosition
        EEPROM.put(i*10 + 204,170);                      // motor closePosition

    }
}
 

  
