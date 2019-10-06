#include "Eye.h"

Eye g_Eye;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Eye setup started");
  
  g_Eye.Init(5,6, 7,8);
}

void AdjustLids()
{
  int x = map(analogRead(0),  0, 900,  -10, 100);
  int y = map(analogRead(1),  0, 900,  -10, 100);

  g_Eye.OpenUpLid(x);
  g_Eye.OpenDownLid(y);

  g_Eye.MoveEye(0, 0);

  Serial.println();
  delay(1);
}

void AdjustEye()
{
  int x = map(analogRead(0),  0, 900, -100, 100);
  int y = map(analogRead(1),  0, 900, -100, 100);

  g_Eye.MoveEye(x ,y);
  Serial.println();
  delay(1);
}

void AdjustComplex()
{
  int x = map(analogRead(0),  0, 900, -100, 100);
  int y = map(analogRead(1),  0, 900, -100, 100);

  g_Eye.ComplexMove(0, x, y);
  //Serial.println();
  delay(10);
}

void loop() 
{
  //g_Eye.Calibrate();
  //AdjustLids();
  //AdjustEye();
  //AdjustComplex();
/*
  g_Eye.ComplexMove(-100, -100, -100, -100, 0);

  g_Eye.ComplexMove(0, 0, 50, 0, 1000);
  g_Eye.ComplexMove(-100, -100, -100, -100, 100);
*/

  static uint32_t blinkTime = millis();
  if (millis() > blinkTime)
  {
    Serial.println("Blink");
    g_Eye.Blink();

    if (random(4) == 3)
    {
      Serial.println("Double blink");
      g_Eye.Blink(g_Eye.m_prevY);
    }
    
    blinkTime = millis() + 500 + random(3*2000);
  }

  static uint32_t moveTime = millis();
  if (millis() > moveTime)
  {
    Serial.println("Move");
    int x    = random(-50, 50);
    int y    = random(-20, 20);
    int lids = g_Eye.m_prevLids; 
    int t    = 10 + random(200);

    if (random(1, 5) == 1)
      lids = random(50, 80);
    
    g_Eye.ComplexMove(x, y, lids, 0, t);
    
    moveTime = millis() + 10 + random(500);
  }

  delay(10);
}
