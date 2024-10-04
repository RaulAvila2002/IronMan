

#define BTNdESHAB     0
#define BTNhABNDO     1
#define BTNhAB        2
#define BTNdESHABNDO  3

#define ESTADO_HABmAX BTNdESHABNDO+1

//ENTRADAS
#define BOT_ENC A1


class Boton
{
    long Tick;                      // milliseconds
    int State;                      // State
    unsigned long previousMillis;   // will store last time

  public:

    Boton(int estado, long tick)
    {
      Tick = tick;
      State = estado;
      previousMillis = 0;
    }

    void Setup(void) {
      pinMode(BOT_ENC, INPUT);

    }

    int GetState() {
      return State;
    }

    void SetState(int estado){
      State = estado;
    }
    
    void Update(int estado)
    {
      // check to see if it's time to change
      unsigned long currentMillis = millis();
      switch (estado) {

        case BTNdESHAB:
          if (currentMillis - previousMillis > Tick) {
        
            previousMillis = currentMillis;
            if (!digitalRead(BOT_ENC)) {
              State = BTNhABNDO;
            #if DEBUG
              MSG_DEBUG;
              Serial.println("DESHABILITADO PRIMER APRETADO");
              #endif
            }
          }
        break;

        case BTNhABNDO:
          if (currentMillis - previousMillis > Tick) {
            previousMillis = currentMillis;
            if (!digitalRead(BOT_ENC)) {
              State = BTNhABNDO;
              #if DEBUG
                MSG_DEBUG;
                Serial.println("BOTON ENCENDIDO APRETADO");
              #endif
            }
            else {
              State = BTNhAB;
          
            }
          }
          break;

        case BTNhAB:
          if (currentMillis - previousMillis > Tick) {
            previousMillis = currentMillis;
            if (!digitalRead(BOT_ENC)) {
              State = BTNdESHABNDO;
              #if DEBUG
                MSG_DEBUG;
                Serial.println("BOTON ENCENDIDO DESHABILITANDO");
              #endif
            }
          }
          break;

        case BTNdESHABNDO:
          if (currentMillis - previousMillis > Tick) {
            previousMillis = currentMillis;
            if (!digitalRead(BOT_ENC)) {
              State = BTNdESHABNDO;
              #if DEBUG
                MSG_DEBUG;
                Serial.println("BOTON ENCENDIDO APRETADO");
              #endif
            }
            else {
              State = BTNdESHAB;
                            #if DEBUG
                MSG_DEBUG;
                Serial.println("BOTON ENCENDIDO LIBERADO");
              #endif
            }
          }
          break;
      }
    }
};
Boton Btn(BTNdESHAB, 50);


/* ----------------------------------------------
   - Funcion:   BotonHabilita               -
   - Descripcion:               -

void BotonHabilita(void) {
  if (1) {
    switch (estadoBotHab) {

      case DESHABILITADO:
        if (!digitalRead(BOT_HAB)) {
          estadoBotHab = HABILITANDO;
        }
        else {
          estadoBotHab = DESHABILITADO;
        }
        break;

      case HABILITANDO:
        if (!digitalRead(BOT_HAB)) {
          estadoBotHab = HABILITANDO;


          //digitalWrite(LED, HIGH);

        }
        else {
          estadoBotHab = HABILITADO;
          mp3_play (1);
          digitalWrite(PECHO, HIGH);     // Apaga todos los digitos
          digitalWrite(MANOS, HIGH);
          digitalWrite(OJOS, HIGH);
          delay(50);
        }
        break;

      case HABILITADO:
        if (!digitalRead(BOT_HAB)) {
          estadoBotHab = DESHABILITANDO;
        }
        else  {
          estadoBotHab = HABILITADO;
        }
        break;

      case DESHABILITANDO:
        if (!digitalRead(BOT_HAB)) {
          estadoBotHab = DESHABILITANDO;
        }
        else  {
          estadoBotHab = DESHABILITADO;
          mp3_stop ();
          digitalWrite(PECHO, LOW);     // Apaga todos los digitos
          digitalWrite(MANOS, LOW);
          digitalWrite(OJOS, LOW);
          delay(50);
        }
        break;
    }
  }
}
   ---------------------------------------------- */
