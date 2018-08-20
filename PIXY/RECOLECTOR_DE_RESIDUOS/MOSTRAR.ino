void mostrar ()
{
  robot.borra_oled();
  switch (accion_en_curso)
  {
    case BUSCAR_PIEZA:
      robot.escribe_oled (2, 2, "PIEZA?");
      robot.escribe_oled_int(3,4, times_searching);
      if (way < 0)
        robot.escribe_oled (5, 5, "->");
      else
        robot.escribe_oled (5, 5, "<-");
      break;
    case IR_A_PIEZA:
      robot.escribe_oled_int (1,1,correccion_mi);
      robot.escribe_oled_int (8,1,correccion_md);
      robot.escribe_oled (2, 2, "VE PIEZA");
      if (errorABSX > MARGEN_X)
      {
        robot.escribe_oled (2, 3, "->");
      }
      else
      {
        robot.escribe_oled (2, 5, "->");
      }
      robot.escribe_oled (5, 3, "X");
      robot.escribe_oled_int (3, 4, errorX);
      robot.escribe_oled (6, 5, "Y");
      if ((abs(errorY_M) > MARGEN_M) && (abs(errorY_C) < MARGEN_CAMARA))
      {
        robot.escribe_oled (4, 5, "^");
        robot.escribe_oled (8, 5, "^");
      }
      robot.escribe_oled_int (1, 6, errorY_C);
      robot.escribe_oled_int (7, 6, pieza_respecto_pinza);
      robot.escribe_oled_int (3, 7, signature);
      break;
    case COGER_PIEZA:
      robot.escribe_oled (2, 2, "COGER");
      robot.escribe_oled_int (3, 7, signature);
      if (tenemos_pieza)
      {
        robot.escribe_oled (2, 2, "OK");
      }
      else
      {
        robot.escribe_oled (2, 2, "NOT");
      }
      break;
    case IR_A_CONTENEDOR:
      robot.escribe_oled (2, 2, "VE CONTENEDOR");
      robot.escribe_oled_int (3, 5, errorX);
      robot.escribe_oled_int (3, 7, signature + MAX_SIGNATURE_USED);
      break;
    case BUSCAR_CONTENEDOR:
      robot.escribe_oled (2, 3, "CONTENEDOR?");
      robot.escribe_oled_int(3,4, times_searching);
      robot.escribe_oled_int (3, 7, signature + MAX_SIGNATURE_USED);
      if (way < 0)
        robot.escribe_oled (5, 5, "->");
      else
        robot.escribe_oled (5, 5, "<-");
      break;
      break;
    case SOLTAR_PIEZA:
      robot.escribe_oled (2, 2, "SOLTAR");
      switch (signature)
      {
        case AMARILLO:
          robot.escribe_oled_int (3, 5, amarillo);
          break;
        case AZUL:
          robot.escribe_oled_int (3, 5, azul);
          break;
        case VERDE:
          robot.escribe_oled_int (3, 5, verde);
          break;
      }
      robot.escribe_oled_int (3, 7, signature);
      break;
    case OPS:
      robot.escribe_oled (2, 2, "OPS");
      robot.escribe_oled_int(4,4, cuenta_ops);
      break;
    case FIN:
      robot.escribe_oled (4, 4, "FIN");
      break;
  }
  led();
}

void led()
{
  if (tenemos_pieza)
  {
    robot.enciende(RGB);
  }
  else
  {
    robot.apaga(RGB);
  }
}

void mostrar_debug ()
{
  for (int j = 0; j < MAX_SIGNATURE; j++)
  {
    Serial.print(j);
    Serial.print(" > ");
    for (int i = 0; i < MAX_DATA; i++)
    {
      switch (i)
      {
        case X:
          Serial.print(" X: ");
          break;
        case Y:
          Serial.print(" Y: ");
          break;
        case H:
          Serial.print(" H: ");
          break;
        case W:
          Serial.print(" W: ");
          break;
        case V:
          Serial.print(" V: ");
          break;
      }
      Serial.print(datos[j][i]);
    }
    Serial.println();
  }
  Serial.print("Suelo: ");
  Serial.println(vemos_suelo);
  Serial.print("Pinza: ");
  Serial.println(tenemos_pieza);
  Serial.print("Camara: ");
  Serial.println(posicion_camara);
  Serial.print("Veces: ");
  Serial.println(veces);
  Serial.print("Blocks: ");
  Serial.println(n_blocks);
  Serial.println("----------");
}

