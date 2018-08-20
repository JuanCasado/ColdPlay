#include <Compluino_03.h>
#include <Pixy.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define PINZA 15
#define PIXY 14

#define PINZA_ABIERTA 0
#define PINZA_CERRADA 34                                                             
#define CAMARA_ALTA 90
#define CAMARA_MEDIA 116
#define CAMARA_BAJA 150

#define CARRERA_D 7
#define CARRERA_I 2

Pixy pixy;
Compluino_03 robot;
Adafruit_PWMServoDriver servo = Adafruit_PWMServoDriver(0x40);
void setup() 
{
  robot.inicia();
  pixy.init();
  servo.begin();
  servo.setPWMFreq(60);
  Serial.begin (38400);
  robot.escribe_oled (3, 4, ">Coldplay<");
  moverServo(PINZA,PINZA_ABIERTA);
  moverServo(PIXY,CAMARA_MEDIA);

  pinMode(CARRERA_D,INPUT);
  pinMode(CARRERA_I,INPUT);
}

void loop()
{
  if (tienePieza())
    robot.enciende(RGB);
  else
    robot.apaga(RGB);
}

void moverServo(int numero, int angulo)
{
  angulo = constrain(angulo,0,180);
  servo.setPWM(numero,0,map(angulo,0,180,172,565));
}

boolean tienePieza ()
{
  return digitalRead(CARRERA_D) && digitalRead(CARRERA_I);
}


