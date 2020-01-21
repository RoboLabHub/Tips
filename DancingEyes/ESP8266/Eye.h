#include "ServoController.h"

class Eye
{
public:
  Eye(bool leftEye)
  {
    m_bLeftEye = leftEye;
  }

  void Init(int pinX, int pinY, int pinLidUp, int pinLidDown)
  {
    m_eyeX.Init(pinX);
    m_eyeY.Init(pinY);
  
    m_lidUp  .Init(pinLidUp  );
    m_lidDown.Init(pinLidDown);
  }

  void OpenUpLid(int percent = 100)   // [-10 .. 100]
  {
    percent = constrain(percent, -15, 100);
    int v = m_bLeftEye ? map(percent, -15, 100,  110, 60) : map(percent, -15, 100,  65, 120);
    m_lidUp.SetPos(v);
  }

  void OpenDownLid(int percent = 100)  // [-10 .. 100]
  {
    percent = constrain(percent, -15, 100);
    int v = m_bLeftEye ? map(percent, -15, 100,  60, 145) : map(percent, -15, 100,  140, 60);
    m_lidDown.SetPos(v);
  }

  void SetLids(int percent = 100, int centerOffset = 0)
  {
    OpenUpLid  (percent + centerOffset);
    OpenDownLid(percent - centerOffset);
  }

  void SetEye(int x, int y)  // [-100 .. 100]
  {
    x = constrain(x, -100, 100);
    y = constrain(y, -100, 100);
    
    int eyeX = m_bLeftEye ? map(x,  -100, 100, 45, 115) : map(x,  -100, 100, 45, 115);
    int eyeY = m_bLeftEye ? map(y,  -100, 100, 25, 135) : map(y,  -100, 100, 140, 40);

    m_eyeX.SetPos(eyeX);
    m_eyeY.SetPos(eyeY);
  }

  void MoveEye(int x, int y, int lids, int offset)
  {
    SetEye(x, y);
    SetLids(lids, offset);
  }

private:
  bool m_bLeftEye;

  ServoController m_eyeX;
  ServoController m_eyeY;
  ServoController m_lidUp;
  ServoController m_lidDown;
};
