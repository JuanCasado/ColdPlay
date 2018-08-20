void ops ()
{
  if (what_ops)
  {
    robot.mueve (50);
  }
  else
  {
    robot.mueve (-50);
  }
  cuenta_ops ++;
  if (cuenta_ops > MAX_OPS)
  {
    cuenta_ops = 0;
    times_searching = 0;
    vemos_algo = false;
  }
}

void atras ()
{
  robot.enciende (RGB);
  robot.doble_beep();
  robot.apaga (RGB);
  
  robot.mueve(-60);
  delay (1200);

  robot.enciende (RGB);
  robot.doble_beep();
  robot.apaga (RGB);
}

