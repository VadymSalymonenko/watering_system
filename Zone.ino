//Реализация методов класса Zone:
Zone::Zone(byte ZoneNumber, Motor motor_c){
    zoneNumber = ZoneNumber;
    updateSettings();
    motor = motor_c;
}
bool Zone::isDayChanged(){
  timeTempCheck();
  if(currentWeekDay==t.Day) return false;
  currentWeekDay = t.Day;
  return true;
}
bool Zone::isStartWateringTime(int currentTime){
  if(!isTurnedOn){return false;}//если зона выключена, возвращаем значения, которые не влияют на работу программы
  if(isDayChanged() && daysToWatering != 0) { daysToWatering--;}
  if(isInWateringProcess){return false;}
  if((timeToWateringStart(currentTime)<TIME_BACKLASH)&&(daysToWatering==0)){ return true;}
  return false;
}
bool Zone::isEndWateringTime(int currentTime){
  if(abs(currentTime-endTime)<TIME_BACKLASH) return true;
  return false;
}
int Zone::timeToWateringStart(int currentTime){
  if(!isTurnedOn) return 1430;//если зона выключена, возвращаем значения, которые не влияют на работу программы
  int timeDifference = startTime - currentTime;
  if(timeDifference<0) timeDifference+=MINUTES_IN_DAY;
  return timeDifference;
}
void Zone::startWatering(){
  openTap(motor);
  isInWateringProcess = true;
}
void Zone::stopWatering(){
  closeTap(motor);
  isInWateringProcess = false;
  daysToWatering = pauseOfDays;
}
void Zone::moveTapExactAngle(int angle){
  moveTap(motor,angle);
}

void Zone::setStartTime(int newStartTime){
    startTime = newStartTime;
    endTime = newStartTime+duration;
  if(endTime>=MINUTES_IN_DAY) endTime-=MINUTES_IN_DAY; 
}

void Zone::updateSettings(){
    pauseOfDays = EEPROM.read(zoneNumber*10);
    duration = EEPROM.read(zoneNumber*10 + 1)*60 + EEPROM.read(zoneNumber*10 + 2);
    startTime = EEPROM.read(zoneNumber*10 + 3)*60 + EEPROM.read(zoneNumber*10 + 4);
    isTurnedOn = EEPROM.read(zoneNumber*10 + 5);
    endTime = startTime+duration;
    if(endTime>=MINUTES_IN_DAY) endTime-=MINUTES_IN_DAY;
}
