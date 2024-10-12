//#include "Defines.h"
#include "IRremote.h"
    
#define RECEIVER  A3

#define ESPERA    0
#define RECIBIDO  1

IRrecv irrecv(RECEIVER);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'

void translateIR(void);

class Control
{
    long Tick;                      // milliseconds
    int State;                      // State
    unsigned long previousMillis;   // will store last time
    int Tecla;
  
  public:
  
  Control (int estado, long tick)
    {
      Tick = tick;
      State = estado;
      previousMillis = 0;
      Tecla = 0;
    }

    void Setup (void){
      irrecv.enableIRIn(); // Start the receiver
    }

    int GetState() {
      return State;
    }
  
    int GetTecla(){
      return Tecla;
    }
   
    void Update (int estado){

      unsigned long currentMillis = millis();
         
          switch (estado){
            
            case ESPERA:
              if (currentMillis - previousMillis > Tick) { 
                previousMillis = currentMillis;
                if (irrecv.decode(&results)) {
                  irrecv.resume();
                  State = RECIBIDO;
                  Tecla= results.value ; //Serial.println(results.value, HEX); 
                }
              }    
            break; 

            case RECIBIDO:
              if (currentMillis - previousMillis > Tick) {
                previousMillis = currentMillis;
                State = RECIBIDO;        
              }  
            break;                  
          }
      }
};

Control  Ir(ESPERA, 50);
