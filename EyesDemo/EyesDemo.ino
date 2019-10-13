#include "ServoController.h"
#include "Eye.h"

Eye g_eye1(true );
Eye g_eye2(false);

void setup() 
{
  Serial.begin(115200);
  Serial.println("Eye started");

  InitServoController();
  
  g_eye1.Init(0, 1, 2, 3);
  g_eye2.Init(4, 5, 6, 7);
}

int g_x, g_y, g_lids, g_offset;

void Move(int x, int y = 0, int lids = 50, int offset = 0)
{
  g_eye1.MoveEye(x, y, lids, offset);
  g_eye2.MoveEye(x, y, lids, offset);

  g_x = x;
  g_y = 0;
  g_lids = lids;
  g_offset = offset;
}

void MoveRandom() 
{
  static uint32_t blinkTime = millis();
  if (millis() > blinkTime)
  {
    int lids = g_lids;
    Move(g_x, g_y, 0, g_offset);
    delay(100);
    Move(g_x, g_y, lids, g_offset);
   
    blinkTime = millis() + random(3*2000);
  }

  static uint32_t moveTime = millis();
  if (millis() > moveTime)
  {
    Serial.println("Move");
    int x = random(-30, 30);
    int y = random(-20, 20);
    int lids = g_lids;

    if (random(1, 5) == 1)
      lids = random(40, 50);

    Move(x, y, lids, y/2);
    
    moveTime = millis() + random(1000);
  }

  delay(10);
}

void Move1(int x, int y, int lids, int offset)
{
  Move(x, y, lids, offset);
  delay(2000);
}

void test()
{
  Move1(0, 0, 0, 0);

//  Move1(-100, 0, 0, 0);
//  Move1(100, 0, 0, 0);

  Move1(0, -100, 0, 0);
  Move1(0, 100, 0, 0);

//  Move(0, 0, 100, 0);

  Move1(0, -70, 30, -35);
  Move1(0, 70, 30, 35);
}

void Demo1()
{
  static uint32_t s_stopTime = 0;
  
  static bool s_Start = true;
  if (s_Start)
  {
    s_Start = false;

    Move(0, 0, 0, 0);
    delay(3*1000);

    Move(0, 0, 70, 0);
    delay(1000);

    for (int i = 0; i < 2; i++)
    {
      Move(0, 0, 0, 0);
      delay(100);
      Move(0, 0, 60, 0);
      delay(500);
    }
    
    Move(-100, 0, 50, 0);
    delay(500);
    Move(100, 0, 50, 0);
    delay(500);

    Move(0, 0, 30, 0);
    delay(1000);

    s_stopTime = millis() + (uint32_t)40*1000;
  }

  if (millis() > s_stopTime)
  {
    Move(0, 0, 0, 0);
    delay(20*1000);

    Move(0, 0, 60, 0);
    delay(30*1000);
    
    s_stopTime = millis() + (uint32_t)10*1000;
  }

  MoveRandom();
}

void RadialMove(float angle, int distance = 70, int lids = 40, float phase_shift = 0)
{
  int x = sin(angle) * distance;
  int y = cos(angle) * distance;

  g_eye1.MoveEye(x, y, lids, y/2);

  x = sin(angle + phase_shift) * distance;
  y = cos(angle + phase_shift) * distance;

  g_eye2.MoveEye(x, y, lids, y/2);
}

void Demo2()
{
  Move(0, 0, 0, 0);
  delay(3*1000);

  Move(0, 0, 70, 0);
  delay(1000);

  for (int i = 0; i < 100; i+= 1)
  {
    g_eye1.MoveEye( i, 0, 50);
    g_eye2.MoveEye(-i, 0, 50);
    
    delay(1);
  }

  delay(1000);
  g_eye1.MoveEye( 100, 0, 0);
  g_eye2.MoveEye(-100, 0, 0);
  delay(200);
  g_eye1.MoveEye( 100, 0, 50);
  g_eye2.MoveEye(-100, 0, 50);
  delay(1000);

  for (int i = 100; i > -100; i -= 1)
  {
    g_eye1.MoveEye( i, 0, 50);
    g_eye2.MoveEye(-i, 0, 50);
    
    delay(1);
  }

  delay(1000);
  g_eye1.MoveEye(-100, 0, 0);
  g_eye2.MoveEye( 100, 0, 0);
  delay(200);
  g_eye1.MoveEye(-100, 0, 50);
  g_eye2.MoveEye( 100, 0, 50);
  delay(1000);

  Move(0);
  delay(500);

  for (int i = 0; i < 2; i++)
  {
    Move(-100);
    delay(500);
    
    Move(100);
    delay(500);
  }

  Move(0, 0, 70, 0);
  delay(1000);

  for (float a = 0; a < 4*PI; a += PI/100)
  {
    RadialMove(a);
    delay(1);
  }

  delay(100);

  for (float a = 4*PI; a > 0; a -= PI/100)
  {
    RadialMove(a);
    delay(1);
  }

  delay(1000);

  for (float a = 4*PI; a > 0; a -= PI/100)
  {
    RadialMove(a, 70, 60, PI);
    delay(1);
  }
}

void loop() 
{
  //test();
  //MoveRandom();
  
  //Demo1();
  Demo2();
}
