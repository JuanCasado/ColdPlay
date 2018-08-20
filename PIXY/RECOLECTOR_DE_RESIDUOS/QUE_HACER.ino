int que_hacer ()
{
  if (!(amarillo || verde || azul))
  {
    return FIN;
  }
  
  tiempo_actual = millis();
  if ((tiempo_actual - tiempo_anterior_pixy) > TIEMPO_PIXY) //PODEMOS LEER PIXY
  {
    blocks = pixy.getBlocks();
    if (blocks)
    {
      tiempo_anterior_pixy = tiempo_actual;
      actualizar_bloques ();
      suelo_izq |= robot.lee_suelo_id();
      suelo_der |= robot.lee_suelo_dd();
      tenemos_pieza |= ((digitalRead(CARRERA_D) || digitalRead(CARRERA_I)));
      vemos_suelo |= (suelo_izq || suelo_der);
      pixy.setLED(255, 255, 255);
    }

    if ((vemos_pieza && (accion_anterior == BUSCAR_PIEZA)) || (vemos_contenedor && (accion_anterior == BUSCAR_CONTENEDOR)))
    {
      robot.beep();
      tiempo_anterior_accion = 0;
    }
  }
  else
  {
    blocks = pixy.getBlocks();
  }

  tiempo_actual = millis();
  if ((tiempo_actual - tiempo_anterior_accion) > TIEMPO_ACCION) //PODEMOS MOVERNOS
  {
    tiempo_anterior_accion = tiempo_actual;

    suelo_izq |= robot.lee_suelo_id();
    suelo_der |= robot.lee_suelo_dd();
    tenemos_pieza |= ((digitalRead(CARRERA_D) || digitalRead(CARRERA_I)));
    vemos_suelo |= (suelo_izq || suelo_der);
    
    normalizar_datos ();

    //DECISION TREE
    if (vemos_suelo)
    {
      t_busqueda = 0;
      return SOLTAR_PIEZA;
    }
    else if (hay_que_cerrar())
    {
      t_busqueda = 0;
      return COGER_PIEZA;
    }
    else if (corrigiendo > MAX_CORRIGIENDO)
    {
      corrigiendo = 0;
      return ATRAS;
    }
    else if (tenemos_pieza)
    {
      moverServo(PINZA, PINZA_CERRADA);
      if (vemos_contenedor)
      {
        errorX = MITAD - datos[signature + MAX_SIGNATURE_USED][X];
        errorABSX = abs(errorX);
        t_busqueda = 0;
        nuevo_giro = true;
        return IR_A_CONTENEDOR;
      }
      else
      {
        if (t_busqueda > MAX_SEARCHING)
        {
          nextWay = ! nextWay;
          t_busqueda = 0;
          return ATRAS;
        }
        t_busqueda ++;
        if (nuevo_giro)
        {
          nextWay = !nextWay;
          nuevo_giro = false;
        }
        return BUSCAR_CONTENEDOR;
      }
    }
    else
    {
      moverServo(PINZA, PINZA_ABIERTA);
      if (vemos_pieza)
      {
        errorY_C = TARGET_CAMARA - datos[signature][Y];
        errorY_C_ABS = abs(errorY_C);
        errorX = MITAD - datos[signature][X];
        errorABSX = abs(errorX);
        t_busqueda = 0;
        nuevo_giro = true;
        return IR_A_PIEZA;
      }
      else
      {
        if (t_busqueda > MAX_SEARCHING)
        {
          nextWay = ! nextWay;
          t_busqueda = 0;
          return ATRAS;
        }
        t_busqueda ++;
        if (nuevo_giro)
        {
          nextWay = !nextWay;
          nuevo_giro = false;
        }
        return BUSCAR_PIEZA;
      }
    }
  }
  else
  {
    return ESPERAR;
  }
}

void actualizar_bloques ()
{
  unsigned int sign;
  boolean do_it = false;
  n_blocks += blocks;
  for (int j = 0; j < blocks; j++)
  {
    sign = pixy.blocks[j].signature-1;
    datos[sign][X] += pixy.blocks[j].x;
    datos[sign][Y] += pixy.blocks[j].y;
    datos[sign][H] += pixy.blocks[j].height;
    datos[sign][W] += pixy.blocks[j].width;
    datos[sign][V] ++;

    if (sign < MAX_SIGNATURE_USED)
    {
      if (sign == AMARILLO)
      {
        do_it = amarillo;
      }
      else if (sign == AZUL)
      {
        do_it = azul;
      }
      else if (sign == VERDE)
      {
        do_it = verde;
      }
      
      if (do_it)
      {
        vemos_pieza |= sign < MAX_SIGNATURE_USED;
      }
    }
    else
    {
      vemos_contenedor |= (sign == (signature + MAX_SIGNATURE_USED));
    }
  }
  veces ++;
}

void normalizar_datos ()
{
  boolean do_it = false;
  searchHeight = 0;
  for (int j = 0; j < MAX_SIGNATURE; j++)
  {
    if (datos[j][V] > 0)
    {
      for (int i = 0; i < V; i++)
      {
          datos[j][i] /= datos[j][V];
      }
      if (!tenemos_pieza && vemos_pieza && (j < MAX_SIGNATURE_USED))
      {
        switch (j)
        {
          case AMARILLO:
            do_it = amarillo;
            break;
          case AZUL:
            do_it = azul;
            break;
          case VERDE:
            do_it = verde;
            break;
        }
        if (do_it)
        {
          if (datos[j][H] > searchHeight)
          {
            signature = j;
            searchHeight = datos[j][H];
          }
        }
      }
    }
    vemos_contenedor |= (datos[signature + MAX_SIGNATURE_USED][V] > 0);
  }
}

void reset_datos()
{
  for (int j = 0; j < MAX_SIGNATURE; j++)
  {
    for (int i = 0; i < MAX_DATA; i++)
    {
      datos[j][i] = 0;
    }
  }
  suelo_izq = false;
  suelo_der = false;
  vemos_suelo = false;
  tenemos_pieza = false;
  vemos_pieza = false;
  vemos_contenedor = false;
  veces = 0;
  n_blocks = 0;
}

boolean hay_que_cerrar()
{
  Serial.println("LOOP");
  if (posicion_pinza == PINZA_ABIERTA)
  {
    pieza_respecto_pinza = (datos[6][Y] - datos[6][H]) - datos[signature][Y];
    if ((datos[6][V] > 0) && (datos[signature][V] > 0) && (((datos[6][Y] - datos[6][H]) - 5) <= datos[signature][Y]))
    {
      cuentaPinza += TARGET_CUENTA_PINZA;
    }
    else if ((datos[6][V] > 0) && (datos[signature][V] > 0) && (((datos[6][Y] - datos[6][H]) - 15) <= datos[signature][Y]))
    {
      cuentaPinza ++;
    }
    
    if (tenemos_pieza)
    {
      cuentaPinza = TARGET_CUENTA_PINZA+1;
    }
    return (cuentaPinza > TARGET_CUENTA_PINZA);
  }
  else
  {
    return false;
  }
}

