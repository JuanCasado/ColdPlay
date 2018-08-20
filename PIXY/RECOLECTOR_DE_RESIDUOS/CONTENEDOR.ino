void buscar_contenedor ()
{
  if (fue_a_contenedor)
  {
    cambio_contenedor ++;
    if (cambio_contenedor > MAX_CAMBIO_CONTENEDOR)
    {
      fue_a_contenedor = false;
    }
  }
  else
  {
    way = (nextWay ? VEL_GIRO_CONTENEDOR : -VEL_GIRO_CONTENEDOR);
    robot.gira(way);
    moverServo(PIXY, CAMARA_MEDIA);
    vel_contenedor = 100;
  }
}

void ir_a_contenedor ()
{
  fue_a_contenedor = true;
  cambio_contenedor = 0;
  moverServo(PIXY, CAMARA_MEDIA);
  //MOVER ROBOT EN X
  if (errorABSX > MARGEN_X_CONTENEDOR)
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
  }
  else
  {
    integralX = 0;
    correccion = 0;
    correccion_md = 0;
    correccion_mi = 0;
  }
  //MOVER ROBOT EN Y
  robot.mueve_motor(PIN_MOTOR_D, constrain(vel_contenedor+correccion_md,-VEL_MAX,VEL_MAX));
  robot.mueve_motor(PIN_MOTOR_I, constrain(vel_contenedor+correccion_mi,-VEL_MAX,VEL_MAX));
  if (vel_contenedor > MIN_VEL_CONTENEDOR)
    vel_contenedor --;
}
