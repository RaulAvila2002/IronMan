//#include "Defines.h"


#define LUCESaPAGADAS   0
#define LUCESeNCENDIDAS 1

void secuencia (void);


class Leds{
    long Tick;                      // milliseconds
    int State;                      // State
    unsigned long previousMillis;   // will store last time
    int fsec;

  public:

  Leds (int estado, long tick, int fsec){
      Tick = tick;
      State = estado;
      previousMillis = 0;
    }
  
};

Leds Luces(LUCESaPAGADAS,100,0);
