#include <Compluino_03.h>
#define T_BRAZO 10
#define C_GIRO 10
Compluino_03 Robot;

bool on = 0;

char velocidad = 0;
char velocidad_abs = 0;
char superior = 85;
char inferior = 55;
bool sentido = 0;
bool movimiento = 0;
bool hay_datos = 0;

char giro_brazo = 0;
bool hay_datos_brazo = 0;
bool direccion_brazo = 0;
unsigned long t_brazo = 0;

bool hay_giro = 0;
bool sent = 0;
char cantidad_giro = 0;
char cantidad_giro_abs = 0;
unsigned long t_giro = 0;

void setup()
{
  Robot.inicia();
  Robot.enciende(RGB_AMARILLO);  // El robot se encuentra parado
  Robot.escribe_oled (3, 4, ">ColdPlay<");
  t_brazo = millis();
  t_giro = millis();

  Serial.begin(38400);
}

void loop()
{
  while (Serial.available())
  {
    switch (Serial.read() & B11011111)
    {
      //MOVER BRAZO
      case 'Q':
        hay_datos_brazo = 1;
        direccion_brazo = !direccion_brazo;
        if (direccion_brazo)
          Robot.enciende(RGB_ROJO);
        else
          Robot.enciende(RGB_VERDE);
        break;

      //GIRO
      case 'G':
        while (Serial.available() < 1);
        cantidad_giro = Serial.read();
        cantidad_giro -= 100;
        cantidad_giro_abs = abs(cantidad_giro);
        if (cantidad_giro < 20)
          hay_giro = 0;
        else
          hay_giro = 1;
        sent = 0;
        break;

      //CAMBIAR LA VELOCIDAD
      case 'V':
        while (Serial.available() < 1);
        velocidad = Serial.read();
        velocidad -= 100;
        velocidad_abs = abs(velocidad);
        hay_datos = 1;

        //PARAR
        if (velocidad_abs < 20)
          movimiento = 0;
        //AVANZAR
        else if(velocidad > 0)
        {
          sentido = 1;
          movimiento = 1;
        }
        //RETROCEDER
        else
        {
          sentido = 0;
          movimiento = 1;
        }
        break;

      //PARAR
      case 'P':
        hay_datos = 1;
        movimiento = 0;
        break;

      //CAMBIAR ALTURA DE BRAZO
      case 'L':
        if (superior == 105)
        {
          superior = 85;
          inferior = 55;
        }
        else
        {
          superior = 105;
          inferior = 35;
        }
        break;

      //STOP BRAZO
      case 'B':
        hay_datos_brazo = 0;
        break;

      //ENCENDER/APAGAR ROBOT
      case 'X':
        on = !on;
        break;
    }
  }


    //MOVER BRAZOS
    if (((millis() - t_brazo) > T_BRAZO) && hay_datos_brazo)
    {
      if (direccion_brazo)
      {
        giro_brazo--;
        if (giro_brazo < inferior)
          hay_datos_brazo = 0;
      }
      else
      {
        giro_brazo++;
        if (giro_brazo > superior)
          hay_datos_brazo = 0;
      }
      Robot.pos_servo(giro_brazo);
    }

    //SI HAY GIRO GIRAMOS
    if (hay_giro)
    {
      if (!sent)
      {
        Robot.gira(cantidad_giro);
        sent = 1;
      }
    }
    //SI NO HAY GIRO NOS MOVEMOS
    else if (hay_datos)
    {
      if (movimiento)
      {
        if (sentido)
          Robot.avanza(velocidad_abs);
        else
          Robot.retrocede(velocidad_abs);
      }
      else
        Robot.para();
      hay_datos = 0;
    }
  
}

