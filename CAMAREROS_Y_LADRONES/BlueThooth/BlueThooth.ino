#define POT_V A0
#define POT_G A1
#define T_LECTURA_A 200
#define T_ESCRITURA 500
#define PRECISION_A 8

unsigned long t_lectura_a = 0;
unsigned long t_escritura = 0;
char contador = 0;

int vel_p = 0;
char velocidad = 0;
char vel_anterior = 1;

int giro_p = 0;
char giro = 0;
char giro_anterior = 1;

void setup()
{
  Serial.begin(38400);

  t_lectura_a = millis();
  t_escritura = millis();
}

void loop()
{
  
    //READ ANALOGICO
  if ((millis() - t_lectura_a) > T_LECTURA_A)
  {
    vel_p += analogRead(POT_V);
    giro_p += analogRead(POT_G);

    if (vel_p < 50)
      vel_p = 0;
    if (giro_p < 50)
      giro_p = 0;

    contador ++;
    Serial.println(vel_p);
    Serial.println(giro_p);
    if (contador == PRECISION_A)
    {
      contador = 0;
      vel_p = vel_p >> 3;
      giro_p = giro_p >> 3;
      giro = map(giro_p, 0, 1023, 0, 200);
      velocidad = map(vel_p, 0, 1023, 0, 200);
    Serial.println("final");
    Serial.println(vel_p);
    Serial.println(giro_p);
      vel_p = 0;
      giro_p = 0;
    }
    t_lectura_a = millis();
  }

  //SEND
  if ((millis() - t_escritura) > T_ESCRITURA)
  {
    //VELOCIDAD
       if (vel_anterior != velocidad)
       {
         Serial.write('V');
         Serial.write(velocidad);
         vel_anterior = velocidad;
       }
    //GIRO
      if (giro_anterior != giro)
      {
        Serial.write('G');
        Serial.write(giro);
        giro_anterior = giro;
      }
  t_escritura = millis();
  }
}
