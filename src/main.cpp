/**
 * @file main.cpp
 * @author Raul Avila (avilarauleduardo@gmal.com)
 * @brief
 * @version 0.1
 * @date 2024-09-24
 * @details verificar version 2024-10-14 
 * @copyright Copyright (c) 2024
 *
 */
#include <Arduino.h>

#include "Defines.h"
#include "Boton.h"
#include "Sonido.h"
#include "Control.h"
#define DEBUG 1

#define SALIDA_PECHO 0
#define SALIDA_MANOS 1
#define SALIDA_OJOS 2

#define IRONmANdESHAB 0
#define IRONmANrEPROD 1

// #define DEBUG         1
#define MSG_DEBUG              \
  Serial.print("[");           \
  Serial.print(currentMillis); \
  Serial.print("] ")

#define LED 13

#define OCUPADO 0
#define DESOCUPADO 1

// SALIDAS
#define PECHO 6
#define OJOS 11
#define MANOS 10

char status_sec, salida_luces;
char flag_sec = 0;

// how bright the LED is
int P_brightness = 0;
int M_brightness = 0;
int O_brightness = 0;
int fadeAmount = 5; // how many points to fade the LED by

unsigned long currentMillisSec, previousMillisSec, previousMillisBusy, currentMillisBusy;
long TickSec;

class Personaje
{

  long Tick;                    // milliseconds
  int State;                    // State
  unsigned long previousMillis; // will store last time

public:
  Personaje(int estado, long tick)
  {
    Tick = tick;
    State = estado;
    previousMillis = 0;
  }

  void Setup(void)
  {
    status_sec = DESHABILITADO;
    busy_ant = 0;
    pinMode(PECHO, OUTPUT);
    pinMode(OJOS, OUTPUT);
    pinMode(MANOS, OUTPUT);
    Serial.begin(115200);
    Btn.Setup();
    Mp3.Setup();
    Ir.Setup();
  }

  int GetState()
  {
    return State;
  }

  void PrenderLeds(void)
  {
    digitalWrite(PECHO, HIGH); // Apaga todos los digitos
    digitalWrite(MANOS, HIGH);
    digitalWrite(OJOS, HIGH);
  }

  void ApagarLeds(void)
  {
    digitalWrite(PECHO, LOW); // Apaga todos los digitos
    digitalWrite(MANOS, LOW);
    digitalWrite(OJOS, LOW);
  }

  void Update(int estado)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > Tick)
    {
      previousMillis = currentMillis;
      switch (estado)
      {

      case IRONmANdESHAB:
        if (Btn.GetState() == BTNhAB)
        {
          Mp3.Play(1);
          Btn.SetState(BTNdESHAB);
          State = IRONmANrEPROD;
          digitalWrite(PECHO, HIGH); // Apaga todos los digitos
          digitalWrite(MANOS, HIGH);
          digitalWrite(OJOS, HIGH);
#if DEBUG
          MSG_DEBUG;
          Serial.println("PPAL - > REPRODUCIENDO");
#endif
        }
        break;

      case IRONmANrEPROD:
        if (Btn.GetState() == BTNhAB)
        {
          Mp3.Stop();
          State = IRONmANdESHAB;
          digitalWrite(PECHO, LOW); // Apaga todos los digitos
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

Personaje IronMan(DESHABILITADO, 100);

void secuencia(void)
{
  currentMillisSec = millis();
  if (currentMillisSec - previousMillisSec > TickSec)
  {
    previousMillisSec = currentMillisSec;
    switch (status_sec)
    {

    case DESHABILITADO:
      if (flag_sec)
      {
        status_sec = HABILITADO;
        // flag_sec = 0;
      }
      else
      {
        status_sec = DESHABILITADO;
      }
      break;

    case HABILITADO:
      if (!flag_sec)
      {
        status_sec = DESHABILITADO;
        mp3_stop();
        digitalWrite(PECHO, LOW); // Apaga todos los digitos
        digitalWrite(MANOS, LOW);
        digitalWrite(OJOS, LOW);
        P_brightness = 0;
        M_brightness = 0;
        O_brightness = 0;
      }
      else
      {

        switch (salida_luces)
        {
        case SALIDA_PECHO:
          P_brightness = P_brightness + fadeAmount;
          if (P_brightness <= 0 || P_brightness >= 255)
          {
            fadeAmount = -fadeAmount;
          }

          analogWrite(PECHO, P_brightness); //

          delay(30);
          salida_luces = SALIDA_MANOS;
          break;

        case SALIDA_MANOS:
          M_brightness = M_brightness + fadeAmount;
          if (M_brightness <= 0 || M_brightness >= 255)
          {
            fadeAmount = -fadeAmount;
          }
          analogWrite(MANOS, M_brightness);

          delay(30);
          salida_luces = SALIDA_OJOS;
          break;

        case SALIDA_OJOS:
          O_brightness = O_brightness + fadeAmount;
          if (O_brightness <= 0 || O_brightness >= 255)
          {
            fadeAmount = -fadeAmount;
          }
          analogWrite(OJOS, O_brightness);

          delay(30);
          salida_luces = SALIDA_PECHO;
          break;

        default:
          break;
        }
      }
      break;
    }
  }
}

void setup()
{
  delay(1000);
  IronMan.Setup();
  previousMillisSec = 0;
  previousMillisBusy = 0;
  TickSec = 10;
  salida_luces = SALIDA_PECHO;
}

void loop()
{
  Btn.Update(Btn.GetState());
  Mp3.Update(Mp3.GetState());
  // Ir.Update(Ir.GetState());
  IronMan.Update(IronMan.GetState());
  //  IronMan.IrRecive(Ir.GetTecla());
  if (irrecv.decode(&results))
  {                                     // have we received an IR signal?
    Serial.println(results.value, HEX); // UN Comment to see raw values
    translateIR();
    irrecv.resume(); // receive the next value
  }
  secuencia();

  if (millis() - previousMillisBusy > 5000)
  {

    previousMillisBusy = millis();

    busy = Mp3.Busy();
    if (!busy)
    {
      busy_ant = busy;
    }
    else
    {
      busy_ant = 1;

      mp3_stop();

      digitalWrite(PECHO, LOW); // Apaga todos los digitos
      digitalWrite(MANOS, LOW);
      digitalWrite(OJOS, LOW);
      P_brightness = 0;
      M_brightness = 0;
      O_brightness = 0;
      flag_sec = 0;
      status_sec = DESHABILITADO;
    }
  }
}

void translateIR()
{
  switch (results.value)
  {
  case 0xFFA25D:
  case 0xE318261B:
    Serial.println(" 1              ");
    mp3_play(1);
    //      flag_sec = 1;
    //      status_sec = HABILITADO;
    //      P_brightness = 0;
    //      M_brightness = 0;
    //      O_brightness = 0;
    digitalWrite(PECHO, HIGH); // Apaga todos los digitos
    digitalWrite(MANOS, HIGH);
    digitalWrite(OJOS, HIGH);
    break;

  case 0xFF629D:
  case 0x511DBB:
    Serial.println(" 2              ");
    mp3_play(2);
    digitalWrite(PECHO, HIGH); // Apaga todos los digitos
    digitalWrite(MANOS, HIGH);
    digitalWrite(OJOS, HIGH);
    break;

  case 0xFFE21D:
  case 0xEE886D7F:
    Serial.println(" 3              ");
    mp3_play(3);
    digitalWrite(PECHO, HIGH); // Apaga todos los digitos
    digitalWrite(MANOS, HIGH);
    digitalWrite(OJOS, HIGH);
    break;

  case 0xFF22DD:
  case 0x52A3D41F:
    Serial.println(" 4              ");
    mp3_play(4);
    digitalWrite(PECHO, HIGH); // Apaga todos los digitos
    digitalWrite(MANOS, HIGH);
    digitalWrite(OJOS, HIGH);
    break;

  case 0xFF02FD:
  case 0xD7E84B1B:
    Serial.println(" 5              ");
    mp3_play(5);
    digitalWrite(PECHO, HIGH); // Apaga todos los digitos
    digitalWrite(MANOS, HIGH);
    digitalWrite(OJOS, HIGH);
    break;

  case 0xFFC23D:
  case 0x20FE4DBB:

    Serial.println(" 6              ");
    mp3_play(6);
    digitalWrite(PECHO, HIGH); // Apaga todos los digitos
    digitalWrite(MANOS, HIGH);
    digitalWrite(OJOS, HIGH);
    break;

  case 0xFFE01F:
  case 0xF076C13B:
    Serial.println(" 7              ");
    mp3_play(7);
    flag_sec = 1;
    busy_ant = 0;
    status_sec = HABILITADO;
    P_brightness = 100;
    M_brightness = 0;
    O_brightness = 50;
    break;

  case 0xFFA857:
  case 0xA3C8EDDB:
    Serial.println(" 8             ");
    mp3_play(8);
    flag_sec = 1;
    // status_sec = HABILITADO;
    P_brightness = 100;
    M_brightness = 0;
    O_brightness = 50;
    break;

  case 0xFF906F:
  case 0xE5CFBD7F:
    Serial.println(" 9             ");
    break;

  case 0xFF6897:
  case 0xC101E57B:
    Serial.println(" *              ");
    mp3_random_play();
    break;

  case 0xFF9867:
  case 0x97483BFB:
    Serial.println(" 0              ");
    mp3_stop();
    digitalWrite(PECHO, LOW); // Apaga todos los digitos
    digitalWrite(MANOS, LOW);
    digitalWrite(OJOS, LOW);
    flag_sec = 0;
    // status_sec = DESHABILITADO;
    break;

  case 0xFFB04F:
  case 0xF0C41643:
    Serial.println(" #            ");

    break;

  case 0xFF18E7:
  case 0x3D9AE3F7:
    Serial.println(" VOL+           ");
    if (volumen == 30)
      mp3_set_volume(30);
    else
      mp3_set_volume(++volumen);
    break;


  case 0xFF10EF:
  case 0x8C22657B:
    Serial.println(" LEFT              ");
    digitalWrite(PECHO, HIGH); // Apaga todos los digitos
    // digitalWrite(MANOS, HIGH);
    // digitalWrite(OJOS, HIGH);
    break;

  case 0xFF38C7:
  case 0x488F3CBB:
    Serial.println(" OK              ");

    digitalWrite(PECHO, HIGH); // Apaga todos los digitos
    digitalWrite(MANOS, HIGH);
    digitalWrite(OJOS, HIGH);
    break;

  case 0xFF5AA5:
  case 0x449E79F:
    Serial.println(" RIGHT              ");
    digitalWrite(MANOS, HIGH);
    break;

  case 0xFF4AB5:
  case 0x1BC0157B:
    Serial.println(" VOL-           ");
    if (volumen == 0)
      mp3_set_volume(0);
    else
      mp3_set_volume(--volumen);
    break;

  case 0xFFFFFF:
    Serial.println("Reset");
  }
}
// END translateIR
