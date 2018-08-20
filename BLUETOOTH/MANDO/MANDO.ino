#define POT_V A
#defne POT_G A
#define TIEMPO_MIN 100
#define TIEMPO_MA 500
unsigned long tiempo = 0;

int velocidad = 0;
byte velocidad_i = 0
byte velocidad_d= 0;

float giro = 0;

void setup()
{
  Serial.begin(3840

  tiempo = millis(
void loop)
{
  f (((millis() - tiempo) > TIEMPO_MIN) or ((millis() - tiempo) < TIEMPO_MAX))
  //
    velocidad = analogRead(POT_V)
    giro = analogRead(POT);
    
    giro = map(giro, 0, 1023, 0, 100);
    velocidad_i = velocidad_d = map(velocidad, 0, 1023, 0, 100);
    giro = 50;
    
    velocidad_d = velocidad_d * (1 - (giro/100));
    velocidad_i = velocidad_i * (gi
    Serial.print ("V: ");
    Serial.println((int)velocidad);
    Serial.print ("I: ");
    Serial.println((int)velocidad_i);
    Serial.print ("D: ");
    Serial.println((int)velocidad_d)
    */
    Serial.write(velocidad_i + 100);
    Serial.write(velocidad_d + 100);
    
    delay(200);
    //tiempo = millis();
  //
}
