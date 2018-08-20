void ir_a_pieza()
{
  fue_a_pieza = true;
  cambio_pieza = 0;
  //MOVER PIXY EN Y
  if (abs(errorY_C) > MARGEN_CAMARA)
  {
    if (errorY_C_ABS > MARGEN_CAMARA)
    {
      if (errorY_C > 0)
      {
        integralY_C ++;
      }
      else
      {
        integralY_C --;
      }
    }
    integralY_C = constrain (integralY_C, -INTEGRAL_MAX_Y_C, INTEGRAL_MAX_Y_C);
    moverServo(PIXY, map(integralY_C, -INTEGRAL_MAX_Y_C, INTEGRAL_MAX_Y_C, CAMARA_BAJA, CAMARA_ALTA)); 
  }
  else
  {
    integralY_C = 0;
  }

  //MOVER ROBOT EN X
  if (errorABSX > MARGEN_X)
  {
    if (errorX > 0)
    {
      if (integralX < 0)
        integralX = 0;
      integralX += 3;
    }
    else
    {
      if (integralX > 0)
        integralX = 0;
      integralX -= 3;
    }
    integralX = constrain (integralX, -INTEGRAL_MAX_X, INTEGRAL_MAX_X);
    correccion = constrain(errorX * PX + integralX * IX, -VEL_MAX, VEL_MAX)*KG;

    correccion_md = correccion;
    correccion_mi = -correccion;
    corrigiendo ++;
  }
  else
  {
    corrigiendo = 0;
    integralX = 0;
    correccion = 0;
    correccion_md = 0;
    correccion_mi = 0;
  }
  //MOVER ROBOT EN Y
  errorY_M = TARGET_M - posicion_camara;
  if (abs(errorY_M) > MARGEN_M)
  {
    if (errorY_M > 0)
    {
      if (integralY_M < 0)
        integralY_M = 0;
      integralY_M ++;
    }
    else
    {
      if (integralY_M > 0)
        integralY_M = 0;
      integralY_M --;
    }

    integralY_M = constrain (integralY_M, -INTEGRAL_MAX_Y_M, INTEGRAL_MAX_Y_M);
    robot.mueve_motor(PIN_MOTOR_D, constrain(errorY_M * PY_M + integralY_M * IY_M + correccion_md,-VEL_MAX,VEL_MAX));
    robot.mueve_motor(PIN_MOTOR_I, constrain(errorY_M * PY_M + integralY_M * IY_M + correccion_mi,-VEL_MAX,VEL_MAX));
  }
  else
  {
    integralY_M = 0;
    robot.mueve(0);
    cuentaPinza ++;
  }
}

void buscar_pieza ()
{
  if (fue_a_pieza)
  {
    cambio_pieza ++;
    if (cambio_pieza > MAX_CAMBIO_PIEZA)
    {
      fue_a_pieza = false;
    }
  }
  else
  {
    integralY_C = 0;
    way = (nextWay ? VEL_GIRO_PIEZA : -VEL_GIRO_PIEZA);
    robot.gira(way);
    if (abs(CAMARA_MEDIA - posicion_camara) > 2)
    {
      if ((CAMARA_MEDIA - posicion_camara) > 0)
        moverServo(PIXY, posicion_camara + 1);
      else
        moverServo(PIXY, posicion_camara - 1);
    }
    else
    {
      moverServo(PIXY, CAMARA_MEDIA);
    }
  }
}
