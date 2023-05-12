
/*
#define zone2PauseOfDaysMemory         -- x0
#define zone2DurationHoursMemory       -- x1
#define zone2DurationMinutesMemory     -- x2
#define zone2StartHoursMemory          -- x3
#define zone2StartMinutesMemory        -- x4
#define zone2IsTurnedOnMemory          -- x5
*/




class Zone{
  public:
  
    byte zoneNumber = 0;
    
    bool isInWateringProcess = false;//поливается ли эта зона сейчас
    byte pauseOfDays;
    byte currentWeekDay = 200;
    byte daysToWatering = 1;
      //in minutes:
    int duration;
    int startTime;
    int endTime;
    bool isTurnedOn;
    Motor motor;
    //
    Zone(byte, Motor);
    bool isDayChanged();
    bool isStartWateringTime(int currentTime);
    bool isEndWateringTime(int currentTime);
    int timeToWateringStart(int currentTime);
    void startWatering();
    void stopWatering();
    void moveTapExactAngle(int angle);
    void updateSettings();
    void setStartTime(int);
};
