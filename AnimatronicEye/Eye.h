#include <Servo.h>

class Eye
{
public:
  Eye()
  {
    m_prevX = 0;
    m_prevY = 0;
    m_prevLids         = 0;
    m_prevCenterOffset = 0;
  }

  void Init(int pinX, int pinY, int pinLidUp, int pinLidDown)
  {
    m_eyeX.attach(pinX);
    m_eyeY.attach(pinY);
  
    m_lidUp  .attach(pinLidUp);
    m_lidDown.attach(pinLidDown);
  }

  void OpenUpLid(int percent = 100)   // [-100 .. 100]
  {
    int v = map(percent,  -10, 100,  135, 90);
    v = constrain(v, 90, 135);
    m_lidUp.write(v);

    Serial.print("Up: "); Serial.print(v); Serial.print(" ("); Serial.print(percent); Serial.print("%)\t ");
    //Serial.print(v); Serial.print(",");
  }

  void OpenDownLid(int percent = 100)  // [-100 .. 100]
  {
    int v = map(percent,  -10, 100,  15, 60);
    v = constrain(v, 15, 60);
    m_lidDown.write(v);

    Serial.print("Down: "); Serial.print(v); Serial.print(" ("); Serial.print(percent); Serial.print("%)\t ");
    //Serial.print(v); Serial.print(",");
  }

  void OpenEye(int percent = 100, int centerOffset = 0)
  {
    OpenUpLid  (percent + centerOffset);
    OpenDownLid(percent - centerOffset);
  }

  void MoveEye(int x, int y)  // [-100 .. 100]
  {
    int eyeX = map(x,  -100, 100,  90, 160);
    int eyeY = map(y,  -100, 100,  110, 175);

    m_eyeX.write(eyeX);
    m_eyeY.write(eyeY);

    Serial.print("X: "); Serial.print(eyeX); Serial.print(" ("); Serial.print(x); Serial.print("%)\tY: "); 
    Serial.print(eyeY); Serial.print(" ("); Serial.print(y); Serial.print("%)\t ");
    //Serial.print(eyeX); Serial.print(","); Serial.print(eyeY); Serial.print(",");
  }

  void ComplexMove(int x, int y, int lids, int centerOffset = 0, int moveTime = 0)
  {
    int steps = moveTime / 10;
    if (steps == 0)
      steps = 1;

    float dtX            = (x            - m_prevX           ) / steps;
    float dtY            = (y            - m_prevY           ) / steps;
    float dtLids         = (lids         - m_prevLids        ) / steps;
    float dtCenterOffset = (centerOffset - m_prevCenterOffset) / steps;

    for (; steps > 0; steps--)
    {
      delay(10);

      m_prevX            += dtX;
      m_prevY            += dtY;
      m_prevLids         += dtLids;
      m_prevCenterOffset += dtCenterOffset;
      
      MoveEye(m_prevX, m_prevY);
      OpenEye(m_prevLids, m_prevY + m_prevCenterOffset);

      Serial.println();
    }

    m_prevX            = x;
    m_prevY            = y;
    m_prevLids         = lids;
    m_prevCenterOffset = centerOffset;
  }

  void Blink(int centerOffset = 0, int moveTime = 300)
  {
    float prevLids         = m_prevLids;
    float prevCenterOffset = m_prevCenterOffset;

    moveTime = moveTime * (prevLids / 100.0);
   
    ComplexMove(m_prevX, m_prevY, 0,        centerOffset,     moveTime / 2);
    ComplexMove(m_prevX, m_prevY, prevLids, prevCenterOffset, moveTime / 2);
  }

  void Calibrate()
  {
    OpenEye(0);       delay(1000);
    OpenEye(0, -100); delay(1000);
    OpenEye(0, -100); delay(1000);
    
    OpenEye(100);       delay(1000);
    OpenEye(100, -100); delay(1000);
    OpenEye(100,  100); delay(1000);
  }

public:
  Servo m_eyeX;
  Servo m_eyeY;
  Servo m_lidUp;
  Servo m_lidDown;

  float m_prevX;
  float m_prevY;
  float m_prevLids;
  float m_prevCenterOffset;
};
