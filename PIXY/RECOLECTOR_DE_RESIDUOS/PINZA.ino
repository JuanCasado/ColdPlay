void moverServo(int numero, int angulo)
{
  angulo = constrain(angulo, 0, 180);
  if (numero == PIXY)
  {
    if (posicion_camara != angulo)
    {
      posicion_camara = angulo;
      servo.setPWM(numero, 0, map(angulo, 0, 180, 172, 565));
    }
  }
  else if (numero == PINZA)
  {
    if (posicion_pinza != angulo)
    {
      posicion_pinza = angulo;
      servo.setPWM(numero, 0, map(angulo, 0, 180, 172, 565));
    }
  }
}

void coger_pieza()
{
  fue_a_pieza = false;
  robot.para();
  moverServo(PINZA, PINZA_CERRADA);
  delay (500);
  if ((digitalRead(CARRERA_D) || digitalRead(CARRERA_I)))
  {
    robot.enciende (RGB);
    robot.doble_beep();
    robot.apaga (RGB);    
    moverServo(PIXY, CAMARA_MEDIA);
    robot.mueve(-50);
    delay (700);
  }
  else
  {
    moverServo(PINZA, PINZA_ABIERTA);
    delay (400);
    robot.mueve(100);
    delay (120);
    robot.beep();
  }
  cuentaPinza = 0;
  if (signature == VERDE)
  {
    nextWay = true;
  }
  else if (signature == AZUL)
  {
    nextWay = false;
  }
  nuevo_giro = false;
}

void soltar_pieza()
{
  corrigiendo = 0;
  fue_a_contenedor = false;
  fue_a_pieza = false;

  if (tenemos_pieza)
  {
    switch (signature)
    {
      case AMARILLO:
        amarillo = false;
        break;
      case AZUL:
        azul = false;
        break;
      case VERDE:
        verde = false;
        break;
    }
  }
  moverServo(PINZA, PINZA_ABIERTA);
  robot.mueve(50);
  delay (500);
  robot.mueve(-50);
  robot.enciende (RGB);
  robot.doble_beep();
  robot.apaga (RGB);
  delay (1200);
  if (signature == VERDE)
  {
    nextWay = false;
  }
  else if (signature == AZUL)
  {
    nextWay = true;
  }
}
