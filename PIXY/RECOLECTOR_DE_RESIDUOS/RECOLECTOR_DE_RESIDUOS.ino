#include <Compluino_03.h>
#include <Pixy.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define PINZA 15
#define PIXY 14

#define PINZA_ABIERTA 0
#define PINZA_CERRADA 35
#define CAMARA_ALTA 90
#define CAMARA_MEDIA 115
#define CAMARA_BAJA 165

#define TARGET_CAMARA 100
#define MARGEN_CAMARA 20
#define TARGET_CUENTA_PINZA 4
#define TARGET_M 140
#define MARGEN_M 8
#define MARGEN_MEDIDAS 40

#define INTEGRAL_MAX_X 100
#define INTEGRAL_MAX_Y_C 20
#define INTEGRAL_MAX_Y_M 50
#define VEL_MAX 100
#define VEL_GIRO_PIEZA 30
#define VEL_GIRO_CONTENEDOR 40
#define MITAD 155
#define MARGEN_X 3
#define MARGEN_X_CONTENEDOR 15

#define TIEMPO_PIXY 55
#define TIEMPO_ACCION 250

#define PX 0.2
#define IX 0.3
#define PY_C 0.16
#define IY_C 1.2
#define PY_M 2.5
#define IY_M 0.8
#define KG 0.6

#define CARRERA_D 7
#define CARRERA_I 2

#define ESPERAR 0
#define BUSCAR_PIEZA 1
#define IR_A_PIEZA 2
#define COGER_PIEZA 3
#define IR_A_CONTENEDOR 4
#define BUSCAR_CONTENEDOR 5
#define SOLTAR_PIEZA 6
#define FIN 7
#define OPS 8
#define ATRAS 9

#define AMARILLO 0
#define VERDE 1
#define AZUL 2
#define MAX_SIGNATURE 7
#define MAX_SIGNATURE_USED 3

#define MAX_DATA 5
#define X 0
#define Y 1
#define H 2
#define W 3
#define V 4

#define MIN_VEL_CONTENEDOR 65

#define MAX_SEARCHING 100
#define MAX_OPS 20

#define MAX_CAMBIO_CONTENEDOR 6
#define MAX_CAMBIO_PIEZA 4

#define MAX_FIN 3

#define MAX_CORRIGIENDO 80

Pixy pixy;
Compluino_03 robot;
Adafruit_PWMServoDriver servo = Adafruit_PWMServoDriver(0x40);

int errorX = 0;
int integralX = 0;
int errorABSX = 0;
int errorY_C = 0;
int errorY_C_ABS = 0;
int integralY_C = 0;
int errorY_M = 0;
int integralY_M = 0;
int object_y = 0;
int posicion_camara = 0;
int posicion_pinza = 1;
long tiempo_anterior_pixy = 0;
long tiempo_anterior_accion = 0;
long tiempo_actual = 0;
int16_t blocks;

int way = 100;
boolean nuevo_giro = true;
boolean nextWay = (rand() % 2);

unsigned int searchHeight = 0;

int cuentaPinza = 0;
int cuentaCamara = 0;

int accion_en_curso = ESPERAR;
int accion_anterior = ESPERAR;
boolean vemos_pieza = false;
boolean vemos_contenedor = false;
boolean vemos_algo = false;
unsigned int signature = 0;

boolean amarillo = true;
boolean azul = true;
boolean verde = true;

int times_searching = 0;
int cuenta_ops = 0;
boolean what_ops = false;

int vel_contenedor = 100;
boolean fue_a_contenedor = false;
int cambio_contenedor = 0;
boolean fue_a_pieza = false;
int cambio_pieza = 0;

int end_fin = 0;

boolean tenemos_pieza = false;
boolean vemos_suelo = false;

int correccion_md = 0;
int correccion_mi = 0;
int correccion = 0;

boolean suelo_izq = false;
boolean suelo_der = false;

unsigned int datos [MAX_SIGNATURE][MAX_DATA];
int veces = 0;

int pieza_respecto_pinza = 0;
int t_busqueda = 0;
int n_blocks = 0;

int corrigiendo = 0;

void setup()
{
  robot.inicia();
  pixy.init();
  servo.begin();
  servo.setPWMFreq(60);
  Serial.begin (115200);
  pinMode(CARRERA_D, INPUT);
  pinMode(CARRERA_I, INPUT);

  reset_datos();
  robot.escribe_oled (3, 4, ">Coldplay<");
  delay (1000);
  moverServo(PINZA, PINZA_ABIERTA);
  moverServo(PIXY, CAMARA_MEDIA);
  delay (1000);
  moverServo(PINZA, PINZA_ABIERTA);
  moverServo(PIXY, CAMARA_ALTA);
  delay (1000);
  robot.enciende(RGB);
  moverServo(PIXY, CAMARA_BAJA);
  moverServo(PINZA, PINZA_CERRADA);
  delay (2000);
  robot.apaga(RGB);
  moverServo(PIXY, CAMARA_ALTA);
  moverServo(PINZA, PINZA_ABIERTA);
  delay (2000);
  robot.doble_beep();
  blocks = pixy.getBlocks(20);
  delay (1000);
  blocks = pixy.getBlocks(20);
  moverServo(PIXY, CAMARA_MEDIA);
  delay (1000);
  tiempo_anterior_pixy = millis();
  tiempo_anterior_accion = millis();
}

void loop()
{
  switch (accion_en_curso = que_hacer())
  {
    case BUSCAR_PIEZA:
      buscar_pieza();
      break;
    case IR_A_PIEZA:
      ir_a_pieza();
      break;
    case COGER_PIEZA:
      coger_pieza();
      break;
    case IR_A_CONTENEDOR:
      ir_a_contenedor();
      break;
    case BUSCAR_CONTENEDOR:
      buscar_contenedor();
      break;
    case SOLTAR_PIEZA:
      soltar_pieza();
      break;
    case OPS:
      ops();
      break;
    case ATRAS:
      atras();
      break;
    case FIN:
      fin();
      break;
  }
  if (accion_en_curso != ESPERAR)
  {
    //mostrar();
    //mostrar_debug ();
    reset_datos();
    accion_anterior = accion_en_curso;
  }
}

void fin ()
{
  robot.mueve(-40);
  if (end_fin < MAX_FIN)
  {
    moverServo(PIXY, CAMARA_ALTA);
    moverServo(PINZA, PINZA_ABIERTA);
    robot.enciende(RGB);
    robot.doble_beep();
    delay (500);
    moverServo(PIXY, CAMARA_BAJA);
    moverServo(PINZA, PINZA_CERRADA);
    robot.apaga(RGB);
    robot.doble_beep();
    delay (500);
    end_fin++;
  }
  else
  {
    robot.para();
    delay (5000);
    moverServo(PIXY, CAMARA_MEDIA);
    moverServo(PINZA, PINZA_ABIERTA);
  }
}

