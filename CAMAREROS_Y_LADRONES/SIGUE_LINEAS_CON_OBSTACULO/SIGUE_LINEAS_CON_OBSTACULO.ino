#include <Compluino_03.h>
#define AVANCE_RAPIDO 100
#define CALIBRACION 40
#define MUESTREO 10
#define TIEMPO_GIRO 400
#define TIEMPO_AVANCE 2000

int distancia = 0;
int control_texto = 0;

Compluino_03 Robot;

void setup()
{
  Robot.inicia();

  Robot.escribe_oled(2, 0, "Siguelineas");
  Robot.escribe_oled(1, 1, "CON_OBSTACULOS");
  Robot.escribe_oled(0, 5, "Pulsa para");
  Robot.escribe_oled(0, 6, "Comenzar");
  Robot.escribe_oled(13, 7, "-->");
  Robot.espera_pulsador();
  Robot.borra_oled();
}

void loop()
{
  distancia = Robot.lee_distancia(0);
  if (control_texto > MUESTREO)
    Robot.escribe_oled_distancia(5,4,T_GRANDE,1);
  
  if (distancia < 5)
  {
   // Robot.para();
    Robot.gira_izquierda(AVANCE_RAPIDO);
    Robot.espera_milisegundos(TIEMPO_GIRO);
    Robot.avanza(AVANCE_RAPIDO);
    Robot.espera_milisegundos(TIEMPO_AVANCE);

    Robot.gira_derecha(AVANCE_RAPIDO);
    Robot.espera_milisegundos(4500);

   while (Robot.lee_suelo_dd())
    Robot.avanza(AVANCE_RAPIDO);
    
    

  }
  else
  {
    if (Robot.lee_suelo_dd() == BLANCO)
    {
      Robot.mueve_motor(PIN_MOTOR_D, AVANCE_RAPIDO);
      if (control_texto > MUESTREO)
        Robot.escribe_oled (15, 7, "B");
      }
      else
      {
        Robot.mueve_motor(PIN_MOTOR_D,CALIBRACION);
        if (control_texto > MUESTREO)
          Robot.escribe_oled (15, 7, "N");
      }
      
      if (Robot.lee_suelo_id() == BLANCO)
      {
        Robot.mueve_motor(PIN_MOTOR_I, AVANCE_RAPIDO);
        if (control_texto > MUESTREO)
          Robot.escribe_oled (1, 7, "B");
      }
      else
      {
        Robot.mueve_motor(PIN_MOTOR_I,CALIBRACION);
        if (control_texto > MUESTREO)
          Robot.escribe_oled (1, 7, "N");
      }
     }
    if (control_texto > MUESTREO)
      control_texto = 0;
    control_texto ++;
}
