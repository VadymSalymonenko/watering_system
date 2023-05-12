

boolean is_time_to_reload(){
   timeTempCheck(); // RTC.read(t);
    if(is_day_changed(t.Day)){  days_to_reload--;  }
    if(days_to_reload == 0 && t.Hour == hour_of_reload){
        return true; 
    }
    if(days_to_reload > days_between_reloads){days_to_reload = 0;}    
    return false;
}
void reload(){
    EEPROM.update(reload_marker_cell,1);
    EEPROM.update(tank_reload_cell,isInFillingTankProcess);
    EEPROM.update(zone1_reload_cell,zone1.isInWateringProcess*100+zone1.daysToWatering);
    EEPROM.update(zone2_reload_cell,zone2.isInWateringProcess*100+zone2.daysToWatering);  // storing zone information
    EEPROM.update(zone3_reload_cell,zone3.isInWateringProcess*100+zone3.daysToWatering);
    
    /*         saving an array of logs and a counter of days                   */
    EEPROM.update(millis_days_cell,millis_days);
    for(byte i = 0; i < size_of_log_list; i++){
        for(byte j = 0; j<4; j++){
            EEPROM.update(start_log_array_cell + (i*4) + j,logList[i][j]);
        }  
    }
    days_to_reload = days_between_reloads;
    delay(400);  
    digitalWrite(rele_transistor_pin,HIGH);
    delay(400);  
     digitalWrite(reset_rele,HIGH);

}
boolean is_day_changed(byte clock_dow){
    if(current_day == 8) current_day = clock_dow;
    if(current_day != clock_dow) {
        current_day = clock_dow;
        return true;
    }
    return false;
}
void reload_recovery(){
    timeTempCheck(); //RTC.read(t);
    EEPROM.update(reload_marker_cell,0); 
    isInFillingTankProcess = EEPROM.read(tank_reload_cell);
    zone1.isInWateringProcess = (floor(EEPROM.read(zone1_reload_cell)/100));
    zone2.isInWateringProcess = (floor(EEPROM.read(zone2_reload_cell)/100));
    zone3.isInWateringProcess = (floor(EEPROM.read(zone3_reload_cell)/100));

    zone1.daysToWatering = (EEPROM.read(zone1_reload_cell)%100);
    zone2.daysToWatering = (EEPROM.read(zone2_reload_cell)%100);
    zone3.daysToWatering = (EEPROM.read(zone3_reload_cell)%100);

    zone1.currentWeekDay = t.Day;
    zone2.currentWeekDay = t.Day;
    zone3.currentWeekDay = t.Day;

/*         reading an array of logs and a counter of days                   */
    millis_days = EEPROM.read(millis_days_cell);
    for(byte i = 0; i < size_of_log_list; i++){
        for(byte j = 0; j<4; j++){
           logList[i][j] = EEPROM.read(start_log_array_cell + (i*4) + j);
        }  
    } 

}
void deep_sleep(){
    timeTempCheck(); // RTC.read(t);
    if(zone1.isInWateringProcess) zone1.stopWatering();
    if(zone2.isInWateringProcess) zone2.stopWatering();
    if(zone3.isInWateringProcess) zone3.stopWatering();
    if(isInFillingTankProcess) stopFillingTank();

    EEPROM.update(sleep_marker_cell,1);
    EEPROM.update(sleep_day_cell,t.Day);

    for(int i = 0; i >= 0; i++){
    if(i == sleep_battery_check*60*6) {
        if(voltage_check() >= min_battery_voltage + 50) {recovery_after_sleep(); break;}
        i = 0;
    }
    delay(100); power.sleepDelay(10000); delay(100);
    }
}
void recovery_after_sleep(){
    byte sleep_duration = 0;
    byte day_of_start_sleep = (EEPROM.read(sleep_day_cell));
    RTC.read(t);
    if(t.Day < day_of_start_sleep){
        sleep_duration = (31 - day_of_start_sleep + t.Day);
    }else{
        sleep_duration = t.Day - day_of_start_sleep;
    }

    if(zone1.daysToWatering <= sleep_duration){
        zone1.daysToWatering = 0;
        zone1.currentWeekDay = t.Day;
    }else{
        zone1.daysToWatering = zone1.daysToWatering - sleep_duration;
    }
    if(zone2.daysToWatering <= sleep_duration){
        zone2.daysToWatering = 0;
        zone2.currentWeekDay = t.Day;
    }else{
        zone2.daysToWatering = zone2.daysToWatering - sleep_duration;
    }
    if(zone3.daysToWatering <= sleep_duration){
        zone3.daysToWatering = 0;
        zone3.currentWeekDay = t.Day;
    }else{
        zone3.daysToWatering = zone3.daysToWatering - sleep_duration;
    }
     EEPROM.update(sleep_marker_cell,0);
 
}
int main_battery_check(){
    if(millis() - last_time_of_check2 >= pause_battery_check){
        last_time_of_check2 = millis();                 // проверка напряжения батареи и возможный принудительный сон.
        if(voltage_check() >= 741) {
            pause_battery_check = 4*60*60*1000;
        }else{
            pause_battery_check = 2*60*60*1000;
        }
        if(voltage_check() <= min_battery_voltage) {deep_sleep();}
    }
}      
int voltage_check(){
    digitalWrite(rele_transistor_pin,HIGH);
    delay(100); 
    digitalWrite(battery_rele,HIGH);
    delay(100);
    int voltage_sum = (analogRead(battery_signal) + analogRead(battery_signal)) /2;
    for(byte i = 1; i <=9; i++){
        if(analogRead(battery_signal) - 50 > (voltage_sum/i) || 
           analogRead(battery_signal) + 50 < (voltage_sum/i)  ){
            voltage_sum += analogRead(battery_signal);
            delay(100);
        }
    }
    digitalWrite(battery_rele,LOW);
    delay(100);
    digitalWrite(rele_transistor_pin,LOW);
    delay(100); 
    return (voltage_sum / 10);
}
