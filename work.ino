#include "work_help_functions.h"

byte tank_status = 0;
byte last_tank_status = 0;

void workCircle (){
  
  timeTempCheck(); // RTC.read(t);
  //  Serial.print("time -> ");
   // Serial.print(t.Hour);
  //  Serial.print(":");
  //  Serial.print(t.Minute);   
 //   Serial.print(":");
  //  Serial.println(t.Second);


//Получение текущего времени:
    int currentTime = t.Hour*60 + t.Minute;//в минутах
    if(t.Day != last_day1){ millis_days++; last_day1 = t.Day;}
    #ifdef DEBUGPRINTS
    Serial.print("---current time--- ");
    Serial.print(t.Hour);
    Serial.print(".");
    Serial.print(t.Minute);
    Serial.print(".");
    Serial.print(t.Second);
    Serial.println(";");
    Serial.println(isFillingTankTime(currentTime));
    Serial.println("---zone1---");
    Serial.print("timeToWateringStart > ");
    Serial.println(zone1.timeToWateringStart(currentTime));
    Serial.print("isInWateringProcess > ");
    Serial.println(zone1.isInWateringProcess);
    Serial.print("isEndWateringTime > ");
    Serial.println(zone1.isEndWateringTime(currentTime));
    Serial.print("EndWateringTime > ");
    Serial.println(zone1.endTime);
    Serial.print("daysToWatering > ");
    Serial.println(zone1.daysToWatering);
    Serial.print("startTime > ");
    Serial.println(zone1.startTime);
    Serial.print("duration > ");
    Serial.println(zone1.duration);

    Serial.println("---zone2---");
    Serial.print("timeToWateringStart > ");
    Serial.println(zone2.timeToWateringStart(currentTime));
    Serial.print("isInWateringProcess > ");
    Serial.println(zone2.isInWateringProcess);
    Serial.print("isEndWateringTime > ");
    Serial.println(zone2.isEndWateringTime(currentTime));
    Serial.print("EndWateringTime > ");
    Serial.println(zone2.endTime);
    Serial.print("daysToWatering > ");
    Serial.println(zone2.daysToWatering);
    Serial.print("startTime > ");
    Serial.println(zone2.startTime);
    Serial.print("duration > ");
    Serial.println(zone2.duration);

    Serial.println("---zone3---");
    Serial.print("timeToWateringStart > ");
    Serial.println(zone3.timeToWateringStart(currentTime));
    Serial.print("isInWateringProcess > ");
    Serial.println(zone3.isInWateringProcess);
    Serial.print("isEndWateringTime > ");
    Serial.println(zone3.isEndWateringTime(currentTime));
    Serial.print("EndWateringTime > ");
    Serial.println(zone3.endTime);
    Serial.print("daysToWatering > ");
    Serial.println(zone3.daysToWatering);
    Serial.print("startTime > ");
    Serial.println(zone3.startTime);
    Serial.print("duration > ");
    Serial.println(zone3.duration);
    #endif
//Обновление статуса бака:
if(isTankFull()){
    tank_status = 2;
    if(last_tank_status != 2){
        addLog(millis_days, t.Hour, t.Minute, 2);
        last_tank_status = tank_status;
    }
    if(SetupButtonCheck() == 1) return;
    #ifdef DEBUGPRINTS 
    Serial.println("tank = 2"); 
    #endif
}else{
    if(isTankEmpty()){
        tank_status = 0;
        if(last_tank_status != 0){
            addLog(millis_days, t.Hour, t.Minute, 3);
            last_tank_status = tank_status;
        }
     #ifdef DEBUGPRINTS 
     Serial.println("tank = 0"); 
     #endif
    }else{
        tank_status = 1;
        last_tank_status = tank_status;
        #ifdef DEBUGPRINTS 
        Serial.println("tank = 1"); 
        #endif
    }
    if(SetupButtonCheck() == 1) return;
}

//Проверка зон:
  if(tank_status != 0 && isInFillingTankProcess == false){
    #ifdef DEBUGPRINTS
    Serial.println("check zone"); 
    #endif
    if(zone1.isStartWateringTime(currentTime)){zone1.startWatering(); addLog(millis_days, t.Hour, t.Minute, 4);}
    if(zone2.isStartWateringTime(currentTime)){zone2.startWatering(); addLog(millis_days, t.Hour, t.Minute, 5);}
    if(zone3.isStartWateringTime(currentTime)){zone3.startWatering(); addLog(millis_days, t.Hour, t.Minute, 6);}
    //if(zone4.isStartWateringTime(currentTime)){zone4.startWatering();}
  };
  if((zone1.isEndWateringTime(currentTime))&&(zone1.isInWateringProcess)) {zone1.stopWatering(); addLog(millis_days, t.Hour, t.Minute, 7);  }
  if((zone2.isEndWateringTime(currentTime))&&(zone2.isInWateringProcess)) {zone2.stopWatering(); addLog(millis_days, t.Hour, t.Minute, 8);  }
  if((zone3.isEndWateringTime(currentTime))&&(zone3.isInWateringProcess)) {zone3.stopWatering(); addLog(millis_days, t.Hour, t.Minute, 9);  }
//Проверка бака:
  if(isInFillingTankProcess == true){
    #ifdef DEBUGPRINTS 
    Serial.println("poputka zackrutb back 1"); 
    #endif
    if((zone1.timeToWateringStart(currentTime)<5)
    || (zone2.timeToWateringStart(currentTime)<5)
    || (zone3.timeToWateringStart(currentTime)<5)
    || (isFillingTankTime(currentTime) == false)
    || (tank_status == 2)){
        stopFillingTank();
        addLog(millis_days, t.Hour, t.Minute, 11);
        }
  }
  if((isFillingTankTime(currentTime))&&(tank_status != 2) && isInFillingTankProcess == false){
    #ifdef DEBUGPRINTS 
    Serial.println("poputka nabratb back 1"); 
    #endif
    if((!zone1.isInWateringProcess)&&(!(zone1.timeToWateringStart(currentTime)<30))
    && (!zone2.isInWateringProcess)&&(!(zone2.timeToWateringStart(currentTime)<30))
    && (!zone3.isInWateringProcess)&&(!(zone3.timeToWateringStart(currentTime)<30))){
      startFillingTank();
      addLog(millis_days, t.Hour, t.Minute, 10);
      }
  }
  if(tank_status == 0 && isInFillingTankProcess == false){
    #ifdef DEBUGPRINTS 
    Serial.println("mr Back is empty. So we need a 1,5 of beer!"); 
    #endif
    if(zone1.isInWateringProcess){zone1.stopWatering(); addLog(millis_days, t.Hour, t.Minute, 12);}
    if(zone2.isInWateringProcess){zone2.stopWatering(); addLog(millis_days, t.Hour, t.Minute, 13);}
    if(zone3.isInWateringProcess){zone3.stopWatering(); addLog(millis_days, t.Hour, t.Minute, 14);}
    // тут можно сделать принудительный длинный сон. сделать небольшой цикл, который будет опрашивать только кнопку и датчик дождя и ждать время набора бака.
    if(isFillingTankTime(currentTime)){ startFillingTank(); addLog(millis_days, t.Hour, t.Minute, 15);}
  }
  
  //delay(50); power.sleepDelay(60000); delay(50);//можно sleepDelay заменить на супер-дупер сон
}
