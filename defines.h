#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <GyverButton.h>
#include <Stepper.h>
#include "GyverPower.h"

const float STEPS_PER_REV = 32; // Number of steps per internal motor revolution 
 
const float GEAR_RED = 64;   //  Amount of Gear Reduction
 
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;   // Number of steps per geared output rotation

#define quantity_of_zones 3


#define TIME_BACKLASH 3
#define MINUTES_IN_DAY 1440

#define FULLSTEP 4
#define HALFSTEP 8

// #define tankMotorPin1  40     // Blue   - 28BYJ48 pin 8
// #define tankMotorPin2  41     // Pink   - 28BYJ48 pin 9
// #define tankMotorPin3  42    // Yellow - 28BYJ48 pin 10
// #define tankMotorPin4  43    // Orange - 28BYJ48 pin 11

#define motor0_pin1  22     // Blue   - 28BYJ48 pin 4
#define motor0_pin2  23     // Pink   - 28BYJ48 pin 5
#define motor0_pin3  25     // Yellow - 28BYJ48 pin 6
#define motor0_pin4  24     // Orange - 28BYJ48 pin 13

#define motor1_pin1  26     // Blue   - 28BYJ48 pin 4
#define motor1_pin2  27     // Pink   - 28BYJ48 pin 5
#define motor1_pin3  29     // Yellow - 28BYJ48 pin 6
#define motor1_pin4  28     // Orange - 28BYJ48 pin 13
                        
#define motor2_pin1  30     // Blue   - 28BYJ48 pin 4
#define motor2_pin2  31     // Pink   - 28BYJ48 pin 5
#define motor2_pin3  33     // Yellow - 28BYJ48 pin 6
#define motor2_pin4  32     // Orange - 28BYJ48 pin 13
                       
#define motor3_pin1  34     // Blue   - 28BYJ48 pin 4
#define motor3_pin2  35     // Pink   - 28BYJ48 pin 5
#define motor3_pin3  37     // Yellow - 28BYJ48 pin 6
#define motor3_pin4  36     // Orange - 28BYJ48 pin 13

#define motor4_pin1  38     // Blue   - 28BYJ48 pin 4
#define motor4_pin2  39     // Pink   - 28BYJ48 pin 5
#define motor4_pin3  41     // Yellow - 28BYJ48 pin 6
#define motor4_pin4  40     // Orange - 28BYJ48 pin 13



#define MOTOR_STEPS_TO_OPEN (-15 * 2048 - 1000) // -12 * 2048 - 2000
#define MOTOR_STEPS_TO_CLOSE (15 * 2048 + 1000) //  12 * 2048 + 2000
//Stepper steppermotor2(STEPS_PER_REV, motorPin1, motorPin3, motorPin2, motorPin4);
//Stepper steppermotor(STEPS_PER_REV, motorPin5, motorPin7, motorPin6, motorPin8);

#define SETUP_BUTTON 10    // 12


#define sleep_battery_check 2 // pause for checking the battery voltage during deep sleep (hours)
#define min_battery_voltage 654
#define CE_pin 48         // A2
#define I_O_pin 46         // A1
#define SCLK_pin 44         // A0

#define Poten_pin A4       // A6
#define Dat_humidity A7     // A7
#define battery_signal A5     // --
#define setButton_pin 8   // D2
#define backButton_pin 9  // --
#define woter_pin_1 6    // UP  (A3)
#define woter_pin_2 7    // DOWN (7)
#define rele_pin 5 // --
#define battery_rele 12 // --
#define reset_rele 11 // --
#define transistor_pin 4 // --
#define rele_transistor_pin 13 // --


#define sda_scl_have_not_button_in_menu
#define DEBUGPRINTS

#define days_between_reloads 14
#define hour_of_reload 4    // --
#define size_of_log_list 30    


#define rainSensorIsTurnedOnMemory_cell 0

#define tankStartHoursMemory_cell 1
#define tankStartMinutesMemory_cell 2
#define tankStopHoursMemory_cell 3
#define tankStopMinutesMemory_cell 4
#define tankModeMemory_cell 5

#define reload_marker_cell 100    // --
#define tank_reload_cell 101
#define zone1_reload_cell 102    // --
#define zone2_reload_cell 103    // --
#define zone3_reload_cell 104    // --

#define sleep_marker_cell 150    // --
#define sleep_day_cell 151    // --
#define millis_days_cell 152    // --
#define start_log_array_cell 160    


tmElements_t t;
LiquidCrystal_I2C lcd(0x27, 16, 2);
GButton setButton(setButton_pin);
GButton backButton(backButton_pin);

int time_pause_before_watering = 10; // через скільки хв перед поливом закрить кран на набор води в бак
int dovShuna_Nabora_vodu = 300;   // скільки вода після отк крана зможе набираться в бак у минутах
boolean Open_Rele_signal = 1;    // якщо реле низького уровня = 0, а якщо високого то = 1

boolean SETUP = 0;
byte logList[size_of_log_list][4];   // array for the log of all actions
byte millis_days = 0;                // counting days since launch, which is necessary for the array of logs
byte last_day1 = 0;                  // counting days since launch, which is necessary for the array of logs


byte current_hour = 0;
byte current_min = 0;
byte current_sec = 0;
byte current_date = 0;
byte current_dow = 0;
byte current_month = 0;
byte current_year = 0;
byte last_millis_check = 0;


//////
  
