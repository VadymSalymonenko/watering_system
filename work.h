bool isInFillingTankProcess = 0;//глобальная переменная




void workCircle();
//-----------//
class Motor{
  public:
    byte pin1, pin2, pin3, pin4;
    Motor(){};
    Motor(byte pin1_c, byte pin2_c, byte pin3_c, byte pin4_c){
      pin1 = pin1_c;
      pin2 = pin2_c;
      pin3 = pin3_c;
      pin4 = pin4_c;
    }
};
