#include <Compluino_03.h>
#define TIEMPO_MIN 100
#define TIEMPO_MAX 300
Compluino_03 Robot;

char velocidad_i = 0;
char velocidad_d = 0;
bool hay_datos = 0;
unsigned long tiempo = 0;

void setup() {
  Robot.inicia();
  Robot.enciende(RGB_AMARILLO);  // El robot se encuentra parado
  Robot.escribe_oled (3, 4, ">ColdPlay<");
  tiempo = millis();

  Serial.begin(38400);

}

void loop() {
  if (Serial.available()> 1)
  {
   velocidad_i = Serial.read() - 100;  
   velocidad_d = Serial.read() - 100;
   hay_datos = 1;  
  }
  
  if ((hay_datos and (TIEMPO_MIN < (millis() - tiempo))) or  (TIEMPO_MAX > (millis() - tiempo)))
  {
    Robot.mueve_motor(PIN_MOTOR_D, velocidad_d);
    Robot.mueve_motor(PIN_MOTOR_I, velocidad_i);
    tiempo = millis();
    hay_datos = 0;
  }
}
