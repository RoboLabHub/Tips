#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver g_pwm = Adafruit_PWMServoDriver();

const int kMinPos = 0;
const int kMaxPos = 180;

const int kPwmFrequency = 50;

const int kServoMinPWM = 700;   // 0.7 ms pulse (0   - degree)
const int kServoMaxPWM = 2500;  // 2.5 ms pulse (180 - degree)

void InitServoController()
{
  g_pwm.begin();
  g_pwm.setPWMFreq(kPwmFrequency);
}

class ServoController
{
public:
  void Init(int id, int adjust = 0)
  {
    m_id     = id;
    m_adjust = adjust;
  }

  void Detach()
  {
    g_pwm.setPWM(m_id, 0, 0);
  }

  int GetPulseWidth(int pos)
  {
    pos = constrain(pos, kMinPos, kMaxPos);
    
    float pwm = map(pos, kMinPos, kMaxPos, kServoMinPWM, kServoMaxPWM);
    return (int)(pwm / 1000000.0 * kPwmFrequency * 4096);
  }

  void SetPos(int pos)
  {
    g_pwm.setPWM(m_id, 0, GetPulseWidth(pos + m_adjust));
  }
  
private:
  int   m_id;
  int   m_adjust;
};
