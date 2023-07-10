bool isInFillingTankProcess = 0;




void workCircle();
//-----------//
class Motor{
  public:
    byte motorNumber = 0;

    boolean connectedPotent = false; 
    byte pin1, pin2, pin3, pin4, potenPin;
    int openPosition = 370;
    int closePosition = 170;
    Motor(){};
    Motor(byte motorNumber_c, byte pin1_c, byte pin2_c, byte pin3_c, byte pin4_c){
      motorNumber = motorNumber_c;
      pin1 = pin1_c;
      pin2 = pin2_c;
      pin3 = pin3_c;
      pin4 = pin4_c;
    }
    Motor(byte motorNumber_c, byte pin1_c, byte pin2_c, byte pin3_c, byte pin4_c, byte potenPin_c, boolean connectedPotent_c){
      motorNumber = motorNumber_c;
      pin1 = pin1_c;
      pin2 = pin2_c;
      pin3 = pin3_c;
      pin4 = pin4_c;
      potenPin = potenPin_c;
      connectedPotent = connectedPotent_c;
    }
};
