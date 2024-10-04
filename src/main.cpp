/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-09-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <Arduino.h>

#include "Defines.h"
#include "Boton.h"
#include "Sonido.h"
#include "Control.h"

//SALIDAS
#define PECHO     9
#define OJOS      5
#define MANOS     6

char num_sec;
char flag_sec = 0;

// how bright the LED is
int P_brightness = 0;
int M_brightness = 0;
int O_brightness = 0;
int fadeAmount = 5;    // how many points to fade the LED by

class Personaje {
  
  long Tick;                       // milliseconds
  int State;                      // State
  unsigned long previousMillis;   // will store last time

  public:
  
    Personaje(int estado, long tick){
      Tick = tick;
      State = estado;
      previousMillis = 0;
    }

    void Setup(void) {
      num_sec = DESHABILITADO;
      busy_ant=0;
      pinMode(PECHO, OUTPUT);
      pinMode(OJOS, OUTPUT);
      pinMode(MANOS, OUTPUT);  
      Serial.begin (115200);
      Btn.Setup();
      Mp3.Setup();
      Ir.Setup();
    }

    int GetState() {
      return State;
    }

    void PrenderLeds(void){
      digitalWrite(PECHO, HIGH);     // Apaga todos los digitos
      digitalWrite(MANOS, HIGH);
      digitalWrite(OJOS, HIGH);        
    }

    void ApagarLeds(void){
    digitalWrite(PECHO, LOW);     // Apaga todos los digitos
    digitalWrite(MANOS, LOW);
    digitalWrite(OJOS, LOW);      
    }

    void Update(int estado){
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis > Tick) {
        previousMillis = currentMillis;
        switch (estado) {
          
          case IRONmANdESHAB:
            if(Btn.GetState()== BTNhAB) { 
              Mp3.Play(1);
              Btn.SetState(BTNdESHAB);
              State = IRONmANrEPROD;
              digitalWrite(PECHO, HIGH);     // Apaga todos los digitos
              digitalWrite(MANOS, HIGH);
              digitalWrite(OJOS, HIGH);               
              #if DEBUG
                MSG_DEBUG;
                Serial.println("PPAL - > REPRODUCIENDO");
              #endif           
            }
          break;
  
          case IRONmANrEPROD:
            if(Btn.GetState()== BTNhAB) { 
              Mp3.Stop();
              State = IRONmANdESHAB;
              digitalWrite(PECHO, LOW);     // Apaga todos los digitos
              digitalWrite(MANOS, LOW);
              digitalWrite(OJOS, LOW);              
              #if DEBUG
                MSG_DEBUG;
                Serial.println("PPAL - DEJAR DE REPRODUCIR ");
              #endif
            }        
            break;
        }  
      }
    }
};

Personaje IronMan(DESHABILITADO,100);

void secuencia (void){
  switch(num_sec){
    
      case DESHABILITADO:
        if (flag_sec) {
          num_sec = HABILITADO;
        }
        else {
          num_sec = DESHABILITADO;
        }
        break;

      case HABILITADO:
        if (!flag_sec){
           num_sec = DESHABILITADO; 
           mp3_stop ();
           digitalWrite(PECHO, LOW);     // Apaga todos los digitos
           digitalWrite(MANOS, LOW);
           digitalWrite(OJOS, LOW);
           P_brightness = 0;
           M_brightness = 0;
           O_brightness = 0;
           delay(100);       
           busy=1;           
         }
        else {
       
         analogWrite(PECHO, P_brightness);     // 
         analogWrite(MANOS, M_brightness);
         analogWrite(OJOS, O_brightness);
        
         P_brightness = P_brightness + fadeAmount;
         if (P_brightness <= 0 || P_brightness >= 255) {
            fadeAmount = -fadeAmount;
         }
         
         M_brightness = M_brightness + fadeAmount;
         if (M_brightness <= 0 || M_brightness >= 255) {
            fadeAmount = -fadeAmount;
         }
         
         O_brightness = O_brightness + fadeAmount;
         if (O_brightness <= 0 || O_brightness >= 255) {
            fadeAmount = -fadeAmount;
         }                  
        delay(10);
        }
      break;
  }

}



void setup(){
  IronMan.Setup();
}

void loop(){
  Btn.Update(Btn.GetState());
  Mp3.Update(Mp3.GetState());
  // Ir.Update(Ir.GetState()); 
  IronMan.Update(IronMan.GetState());
  //IronMan.IrRecive(Ir.GetTecla());
  if (irrecv.decode(&results)) {// have we received an IR signal?
    Serial.println(results.value, HEX);  //UN Comment to see raw values
    translateIR();
    irrecv.resume(); // receive the next value
  }
  secuencia ();
  busy = digitalRead(BUSY_PIN);

  if (busy == 1 && busy_ant == 0) {
    mp3_stop ();
    digitalWrite(PECHO, LOW);     // Apaga todos los digitos
    digitalWrite(MANOS, LOW);
    digitalWrite(OJOS, LOW);
    P_brightness = 0;
    M_brightness = 0;
    O_brightness = 0;
    flag_sec = 0;
    num_sec = DESHABILITADO;
  }


//    if (busy==0 && busy_ant==1) {
//          flag_sec = 1;
//          num_sec = HABILITADO;
//    }
  busy_ant = busy;
}

void translateIR(){
  switch (results.value)
  {
    case 0xFFA25D:
      Serial.println(" 1              ");
      mp3_play (1);
//      flag_sec = 1;
//      num_sec = HABILITADO;
//      P_brightness = 0;
//      M_brightness = 0;
//      O_brightness = 0;
      digitalWrite(PECHO, HIGH);     // Apaga todos los digitos
      digitalWrite(MANOS, HIGH);
      digitalWrite(OJOS, HIGH);      
      break;

    case 0xFF629D:
      Serial.println(" 2              ");
      mp3_play (2);
      digitalWrite(PECHO, HIGH);     // Apaga todos los digitos
      digitalWrite(MANOS, HIGH);
      digitalWrite(OJOS, HIGH);
      break;

    case 0xFFE21D:
      Serial.println(" 3              "); 
      mp3_play (3);
      digitalWrite(PECHO, HIGH);     // Apaga todos los digitos
      digitalWrite(MANOS, HIGH);
      digitalWrite(OJOS, HIGH);
      break;

    case 0xFF22DD:
      Serial.println(" 4              ");
      mp3_play (4);
      digitalWrite(PECHO, HIGH);     // Apaga todos los digitos
      digitalWrite(MANOS, HIGH);
      digitalWrite(OJOS, HIGH);
      break;

    case 0xFF02FD:
      Serial.println(" 5              ");
      mp3_play (5);
      digitalWrite(PECHO, HIGH);     // Apaga todos los digitos
      digitalWrite(MANOS, HIGH);
      digitalWrite(OJOS, HIGH);
      break;

    case 0xFFC23D:
      Serial.println(" 6              ");
      mp3_play (6);
      digitalWrite(PECHO, HIGH);     // Apaga todos los digitos
      digitalWrite(MANOS, HIGH);
      digitalWrite(OJOS, HIGH);
      break;

    case 0xFFE01F:
      Serial.println(" 7              ");
      mp3_play (7);
      flag_sec = 1;
      num_sec = HABILITADO;
      P_brightness = 100;
      M_brightness = 0;
      O_brightness = 50;
      break;

    case 0xFFA857:
      Serial.println(" 8             ");

      break;

    case 0xFF906F:
      Serial.println(" 9             ");
      break;

    case 0xFF6897:
      Serial.println(" *              ");

      break;

    case 0xFF9867:
      Serial.println(" 0              ");
      mp3_stop ();
      digitalWrite(PECHO, LOW);     // Apaga todos los digitos
      digitalWrite(MANOS, LOW);
      digitalWrite(OJOS, LOW);
      flag_sec = 0;
      num_sec = DESHABILITADO;
      break;

    case 0xFFB04F:
      Serial.println(" #            ");
      break;

    case 0xFF30CF:
      Serial.println(" 1              ");
      mp3_play (1);
      flag_sec = 1;
      num_sec = HABILITADO;
      P_brightness = 0;
      M_brightness = 0;
      O_brightness = 0;
      break;

    case 0xFF18E7:
      Serial.println(" VOL+           ");
      if (volumen == 30) mp3_set_volume (30);
      else mp3_set_volume (++volumen);
      break;

    case 0xFF7A85:
      Serial.println(" 3              ");
      mp3_play (3);
      digitalWrite(PECHO, HIGH);     // Apaga todos los digitos
      digitalWrite(MANOS, HIGH);
      digitalWrite(OJOS, HIGH);
      break;

    case 0xFF10EF:
      Serial.println(" LEFT              ");
      digitalWrite(PECHO, HIGH);     // Apaga todos los digitos
      //digitalWrite(MANOS, HIGH);
      //digitalWrite(OJOS, HIGH);
      break;

    case 0xFF38C7:
      Serial.println(" OK              ");

      digitalWrite(PECHO, HIGH);     // Apaga todos los digitos
      digitalWrite(MANOS, HIGH);
      digitalWrite(OJOS, HIGH);
      break;

    case 0xFF5AA5:
      Serial.println(" RIGHT              ");
      digitalWrite(MANOS, HIGH);
      break;

    case 0xFF42BD:
      Serial.println(" 7              ");
      mp3_play (7);
      flag_sec = 1;
      num_sec = HABILITADO;
      P_brightness = 100;
      M_brightness = 0;
      O_brightness = 50;
      break;

    case 0xFF4AB5:
      Serial.println(" VOL-           ");
      if (volumen == 0) mp3_set_volume (0);
      else mp3_set_volume (--volumen);
      break;

    case 0xFF52AD:
      Serial.println(" 9    ff          ");

      break;

  }

} 
//END translateIR

