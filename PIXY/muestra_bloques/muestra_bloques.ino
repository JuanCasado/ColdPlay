#incññlude <SPI.h>
#include <Pixy.h>
#include <Compluino_03.h>
#define MITADX 160
#define MARNX 8
#define MITADY 100
#define MARGENY 8
Pixy pixy;
Compluino_03 Robot;

float Px = 0.4;
float Py = 0.9;
float Ix = 0.8;
float Iy = 0.8;

int x = 0;
int y = 0;
int errorx = 0;
int erriogx = 0;
int errorABSy = 0;
int integraly = 0;
uint16_tlocks;

void setup()
{
  Robot.inia();
  pixy.init();
  Serial.begin(38400);

  Robot.enciende(RGB_AMARILLO);
  Robot.escribe_oled (3, 4, ">Coldplay<");
}
void loop()
{
  blocks = pixy.getBlocks();

  if (blocks)
  {
    Robot.borra_oled();
    x = pixy= MITADX - x;
    errorABSx = abs(errorx);

    if (errorABSx > MARGENX)
    {100,100));
      Robot.escribe_oled_int (3, 4, x);
      Robot.escribe_oled_int (9, 4, y);
      if (errorx > 0)    else
        integralx --;
    }
    else
    {
      integralx = 0;
      errory = MITADY - y;   if (errorABSy > MARGENY)
      {y*Py)+(integraly*Iy)),-100,100));
        Robot.escribe_oled_int (3, 4, x);
        Robot.escribe_oled_int (9, 4, y);
        if (errorx >  integraly ++;
        else
          integraly --;
      }
      else
      {
        Robot.para();
        RobotIT<");
        integraly = 0;
      }
    }
  }
  elseegralx = 0;
    integralpara();escribe_oled (3, 5, ">STOPED<");
  }
  delay(50);
}
