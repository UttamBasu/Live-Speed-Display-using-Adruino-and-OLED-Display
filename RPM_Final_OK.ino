#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);

int rpmsensor = 12;
unsigned long start_time = 0;
unsigned long end_time = 0;
int steps = 0;
float steps_old = 0;
float temp = 0;
float rps = 0;


int spd1 = 0;
float kmph1 = 0.0;


void u8g_prepare(void)
{
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void intro(uint8_t a)
{
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(1, 10, "RPM");  //1 from left side and 10 from above position.
  u8g.setPrintPos(1, 25); //1 from left side and 25 from above position.
  u8g.print(spd1, 1);

  u8g.drawStr(1, 40, "Speed of Car");
  u8g.setPrintPos(1, 55); //1 from the left side and 55 from above position.
  u8g.print(kmph1, 1);
  u8g.drawStr(40, 55, "KP/h");  //20 from the left side and 55 from above position.
}

void RPM(uint8_t a)
{
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(1, 10, "RPM");  //1 from left side and 10 from above position.
  u8g.setPrintPos(1, 25); //1 from left side and 25 from above position.
  u8g.print(spd1, 1);

  u8g.drawStr(1, 40, "Speed of Car");
  u8g.setPrintPos(1, 55); //1 from the left side and 55 from above position.
  u8g.print(kmph1, 1);
  u8g.drawStr(40, 55, "KP/h");  //20 from the left side and 55 from above position.
}

uint8_t draw_state = 2;

void draw(void)
{
  u8g_prepare();
  switch (draw_state >> 1)
  {
    case 0: intro(draw_state & 7); break;
    case 1: RPM(draw_state & 7); break;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(rpmsensor, INPUT_PULLUP);
  Serial.println("LABEL,Date,Time,RPM,KM/H");
}
void loop()
{
  start_time = millis();
  end_time = start_time + 1000;
  while (millis() < end_time)
  {
    if (digitalRead(rpmsensor))
    {
      steps = steps + 1;
      while (digitalRead(rpmsensor));
    }
  }
  temp = steps - steps_old;
  steps_old = steps;
  rps = (temp);
  float spd = 3.14 * 0.135 * rps; // speed = 3.14159*(D/100)*(rpm/60) // speed in m/s
  spd1 = ((3.14 * 0.000135 * rps * 60 * 1000 * 2.5) * 0.9285); //RPM use 2 number of reflector connected
  kmph1 = (spd1 * 0.1885 * 0.45);
  Serial.println( (String) "DATA,DATE,TIME" + "," + spd1 + "," + kmph1);


  // picture loop (It is compulsory)
  u8g.firstPage();
  do
  {
    draw();
  }
  
  while (u8g.nextPage());

  //increase the state
  draw_state++;
  if ( draw_state >= 3 * 2)
    draw_state = 0;
}
