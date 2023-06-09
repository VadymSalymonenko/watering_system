boolean SetupButtonCheck(byte);
int getCurrentTime();

void moveTap(Motor, long);
int calculateMotorDirection(bool actionToDo, Motor motor);
int checkPositionOfMotor(Motor motor);
void openTap(Motor);
void closeTap(Motor);

boolean isTankFull();
boolean isTankEmpty();
bool isFillingTankTime(int);
void startFillingTank();
void stopFillingTank();
void timeTempCheck();
void rtc_reboot();

void addLog(byte, byte, byte, byte);
void clearLogList();
void reset_EEPROM_to_default_settings();
