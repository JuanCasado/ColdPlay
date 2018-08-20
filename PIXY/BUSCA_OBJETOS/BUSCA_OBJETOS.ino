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
#define CAMARA_MEDIA 120
#define CAMARA_BAJA 140

#define TARGET_CAMARA_ALTA 100
#define TARGET_CAMARA_MEDIA 195
#define TARGET_CAMARA_BAJA 195
#define MARGEN_CAMARA_ALTA 20
#define MARGEN_CAMARA_MEDIA 5
#define MARGEN_CAMARA_BAJA 8
#define TARGET_CUENTA_PINZA 1
#define TARGET_CAMBIO_CAMARA 1

#define INTEGRAL_MAX_X 100
#define VEL_MAX 100
#define VEL_GIRO 30
#define MITAD 155
#define MARGEN_X 10

#define TIEMPO_PIXY 50

#define MAX_SIGNATURE 7

#define PX 0.3
#define IX 0.4
#define PY_ALTA 1
#define PY_MEDIA 1
#define PY_BAJA 0.45
#define IY 0.4

#define CARRERA_D 7
#define CARRERA_I 2

Pixy pixy;
Compluino_03 robot;
Adafruit_PWMServoDriver servo = Adafruit_PWMServoDriver(0x40);

int errorX = 0;
int integralX = 0;
int errorABSX = 0;
int errorY = 0;
int integralY = 0;
int errorABSY = 0;
int indice = 0;
int object_y = 0;
int posicion_camara = 0;
int posicion_pinza = 1;
long tiempo_anterior = 0;
long tiempo_actual = 0;
int16_t blocks;

int way = 100;
boolean nuevo_giro = true;
boolean nextWay = (rand()%2);

int searchHeight = 0;

int cuentaPinza = 0;
int cuentaCamara = 0;

void setup()
{
  robot.inicia();
  pixy.init();
  servo.begin();
  servo.setPWMFreq(60);
  Serial.begin (38400);
  
  moverServo(PINZA,PINZA_ABIERTA);
  moverServo(PIXY,CAMARA_MEDIA);
  posicion_camara = CAMARA_MEDIA;

  robot.escribe_oled (3, 4, ">Coldplay<");
  pinMode(CARRERA_D,INPUT);
  pinMode(CARRERA_I,INPUT);
  
}

void loop()
{
  tiempo_actual = millis();
  if (tienePieza ())
  {
    robot.para();
  }
  else
  {
    if ((tiempo_actual - tiempo_anterior) > TIEMPO)
    {
      tiempo_anterior = tiempo_actual;
      if (tomar_datos())
      {
        if (errorABSX > MARGEN_X)
        { 
          if (errorX > 0)
          {
            integralX+=3;
          }
          else
          {
            integralX-=3;
          }
          integralX = constrain (integralX, -INTEGRAL_MAX_X, INTEGRAL_MAX_X);
          robot.gira(constrain(errorX*PX+integralX*IX, -VEL_MAX, VEL_MAX));
        }
        else if (posicion_camara == CAMARA_MEDIA)
        {
          if (errorABSY < MARGEN_CAMARA_MEDIA)
          {
            integralX = 0;
            integralY = 0;
            robot.para();
            moverServo(PINZA,PINZA_ABIERTA);
            if (cuentaCamara > TARGET_CAMBIO_CAMARA)
            {
              moverServo(PIXY,CAMARA_BAJA);
              cuentaCamara = 0;
            }
            cuentaCamara ++;
          } 
          else
          { 
            integralX = 0;
            cuentaCamara = 0;
            moverServo(PINZA,PINZA_ABIERTA);
            if (errorY > 0)
            {
              integralY+=3;
            }
            else
            {
              integralY-=3;
            }
            robot.mueve (constrain(errorY*PY_MEDIA+integralY*IY, -VEL_MAX, VEL_MAX));
          }
        }
        else
        {
          if (errorABSY < MARGEN_CAMARA_BAJA)
          {
            integralX = 0;
            integralY = 0;
            if (cuentaPinza > TARGET_CUENTA_PINZA)
            {
              moverServo(PINZA,PINZA_CERRADA);
              robot.para();
            }
            else
            {
              robot.para();
              cuentaPinza++;
            }
          } 
          else
          { 
            if (cuentaPinza <= TARGET_CUENTA_PINZA)
            {
              integralX = 0;
              if (errorY > 0)
              {
                integralY+=2;
              }
              else
              {
                integralY-=2;
              }
              robot.mueve (constrain(errorY*PY_BAJA+integralY*IY, -VEL_MAX, VEL_MAX));
              moverServo(PINZA,PINZA_ABIERTA);
              cuentaPinza = 0;
            }
          }
        }
      }
      else
      {      
        robot.gira(way);
        moverServo(PIXY,CAMARA_MEDIA);
        moverServo(PINZA,PINZA_ABIERTA);
      }
    }
  }
}

boolean tomar_datos ()
{
  blocks = pixy.getBlocks();
  if (blocks)
  {
    buscaIndice();
    object_y = pixy.blocks[indice].y + pixy.blocks[indice].height;
   robot.borra_oled();
    switch (posicion_camara)
    {
      case CAMARA_ALTA:
        robot.escribe_oled (3, 3,"ALTA");
        errorY = TARGET_CAMARA_ALTA - object_y;
        break;
      case CAMARA_MEDIA:
        robot.escribe_oled (3, 3,"MEDIA");
        errorY = TARGET_CAMARA_MEDIA - object_y;
        break;
      case CAMARA_BAJA:
        robot.escribe_oled (3, 3,"BAJA");
        errorY = TARGET_CAMARA_BAJA - object_y;
        break;
    }
    errorABSY = abs(errorY);
    
    errorX = MITAD - pixy.blocks[indice].x;
    errorABSX = abs(errorX);

    robot.escribe_oled_int (3, 5, errorY);
    robot.escribe_oled_int (3, 7, pixy.blocks[indice].signature);
  }
  else
  {
    if (nuevo_giro)
    {
      integralX = 0;
      way = (nextWay? VEL_GIRO : -VEL_GIRO);
      nextWay = !nextWay;
    }
    robot.borra_oled();
    robot.escribe_oled (3, 3,"BUSCA");
    robot.escribe_oled_int (3, 5,way);
  }
  nuevo_giro = blocks;
  return blocks;
}

void moverServo(int numero, int angulo)
{
  angulo = constrain(angulo,0,180);
  if (numero == PIXY)
  {
    if (posicion_camara != angulo)
    {
      posicion_camara = angulo;
      servo.setPWM(numero,0,map(angulo,0,180,172,565));
    }
  }
  else if (numero == PINZA)
  {
    if (posicion_pinza != angulo)
    {
      posicion_pinza = angulo;
      servo.setPWM(numero,0,map(angulo,0,180,172,565));
    }
  }
}

void buscaIndice ()
{
  indice = 0;
  for (int j = 0; j < blocks; j++)
  {
    if (pixy.blocks[j].signature < MAX_SIGNATURE)
    {
      if (pixy.blocks[j].height > searchHeight)
      {
        indice = j;
        searchHeight = pixy.blocks[j].height;
      }
    }
  }
  searchHeight = 0;
}

boolean tienePieza ()
{
  return digitalRead(CARRERA_D) || digitalRead(CARRERA_I);
}

