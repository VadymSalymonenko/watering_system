#include "defines.h"
#include "settings.h"
#include "work.h"
#include "Zone.h"
#include "set_help_functions.h"
#include "reload.h"


Motor motor1(1, motor1_pin1, motor1_pin3, motor1_pin2, motor1_pin4, A8, true);
Motor motor2(2, motor2_pin1, motor2_pin3, motor2_pin2, motor2_pin4);
Motor motor3(3, motor3_pin1, motor3_pin3, motor3_pin2, motor3_pin4);
Motor motor4(4, motor4_pin1, motor4_pin3, motor4_pin2, motor4_pin4);
Motor tankMotor(0, motor0_pin1, motor0_pin3, motor0_pin2, motor0_pin4);


Zone zone1(1,motor1);
Zone zone2(2,motor2);
Zone zone3(3,motor3);
Zone zone4(4,motor4);

unsigned long last_time_of_check = 0;
unsigned long last_time_of_check2 = 0;
unsigned long pause_battery_check = 4*60*60*1000;

boolean temp1 = 1;



void updateMotor(Motor *motor11){
    motor11->connectedPotent = EEPROM.read((motor11->motorNumber*10) + 201);
    EEPROM.get((motor11->motorNumber*10) + 202, motor11->openPosition);
    EEPROM.get((motor11->motorNumber*10) + 204, motor11->closePosition);
}

void setup(){
    Serial.begin(115200);

    #ifdef sda_scl_have_not_button_in_menu  
    lcd.begin ();
    lcd.clear ();
    lcd.setCursor(0,0);
    lcd.print("    loading...  ");
    #endif
      Serial.println("test");

    SETUP = !digitalRead(SETUP_BUTTON); 

    Serial.print("Setup ->> ");
    Serial.println(SETUP);
    clearLogList();
  
  setButton.setTickMode(AUTO);
  setButton.setDebounce(100);
  setButton.setTimeout(1500);

  backButton.setTickMode(AUTO);
  backButton.setDebounce(100);
  backButton.setTimeout(1500);
  
  pinMode(SETUP_BUTTON,INPUT_PULLUP);
  pinMode (woter_pin_1,INPUT_PULLUP);
  pinMode (woter_pin_2,INPUT_PULLUP);
  pinMode (rele_pin,OUTPUT); 
  pinMode (battery_rele,OUTPUT); 
  pinMode (reset_rele,OUTPUT); 
  pinMode (rele_transistor_pin,OUTPUT); 
  pinMode (transistor_pin,OUTPUT);
  digitalWrite(rele_pin,!Open_Rele_signal);
  power.setSleepMode(POWERDOWN_SLEEP);

    digitalWrite(rele_transistor_pin,HIGH);
    delay(100); 
    digitalWrite(battery_rele,HIGH);
    delay(100);
    #ifdef DEBUGPRINTS
    Serial.print("battery = ");
    Serial.print((5000L * analogRead(battery_signal))/1024);
    Serial.println("mV");
    delay(50);
    Serial.print("battery = ");
    Serial.print((5000L * analogRead(battery_signal))/1024);
    Serial.println("mV");
    delay(50);
    Serial.print("battery = ");
    Serial.print((5000L * analogRead(battery_signal))/1024);
    Serial.println("mV");
    #endif
    digitalWrite(rele_transistor_pin,LOW);  
   
      /* t.Hour = 16;
   t.Minute = 17;
   t.Second = 0;
   t.Day = "SUNDAY";
   t.Month = "MARCH";
   t.Year = CalendarYrToTm(2022);
   RTC.write(t); */ 
   //stopFillingTank();
  RTC.read(t);
 
  if(EEPROM.read(reload_marker_cell) == 1){reload_recovery();}
  if(EEPROM.read(sleep_marker_cell) == 1){recovery_after_sleep();}
  
  
  millis_days = 1;
  last_day1 = t.Day;
  addLog(millis_days, t.Hour, t.Minute, 1);
  //reset_EEPROM_to_default_settings();
 // reload();


  updateMotor(&tankMotor);
  updateMotor(&motor1);
  updateMotor(&motor2);
  updateMotor(&motor3);
}
void loop(){
  timeTempCheck();  //RTC.read(t);
    if(SetupButtonCheck()== 0){
        if(millis() - last_time_of_check >= 1000*60*40){
            last_time_of_check = millis();                 // checking the expected reboot
            if(is_time_to_reload())reload();
        }
        main_battery_check();
    }
    if(SetupButtonCheck()== 1) {
        if(isInFillingTankProcess) {
            delay(100); 
            lcd.begin();
            lcd.clear(); 
            lcd.print("  please, wait  ");     
            stopFillingTank();
        }

        settings();
        zone1.updateSettings();
        zone2.updateSettings();
        zone3.updateSettings(); 

    }
    if(SetupButtonCheck()== 0)workCircle();
}
