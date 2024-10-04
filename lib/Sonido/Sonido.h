

#include <SoftwareSerial.h>

#include <DFPlayer_Mini_Mp3.h>




#define DESHABILITADO   0
#define HABILITANDO     1
#define HABILITADO      2
#define DESHABILITANDO  3

#define BUSY_PIN    3
#define VOLiNICIAL  24

SoftwareSerial mySerial(7,8); // RX, TX

char volumen = 24;
char busy, busy_ant;

class Sonido {
    int volumen;
    long Tick;                       // milliseconds
    int State;                      // State
    unsigned long previousMillis;   // will store last time

  public:
    Sonido (int estado, long tick) {
      Tick = tick;
      State = estado;
      previousMillis = 0;
    }

    void Setup (){
      mySerial.begin (9600);
      pinMode(BUSY_PIN,INPUT);
      mp3_set_serial (mySerial);  //set Serial for DFPlayer-mini mp3 module
      delay(2);  //wait 1ms for mp3 module to set volume
      mp3_set_volume (VOLiNICIAL);
      //mp3_set_device(SD);
      mp3_stop ();
    }
    
    int GetState() {
      return State;
    }
    
    void Play (int pista) {
      mp3_play(pista);
    }
    
    void Stop () {
      mp3_stop();
    }

    void Random(){
      mp3_random_play ();
    }

   
    int Busy(){
      return digitalRead(BUSY_PIN);
    }
    
    void Update(int estado){
      
    }

    void SetVol(int vol){
      mp3_set_volume(vol);
    }

};

Sonido Mp3 (DESHABILITADO,100);
