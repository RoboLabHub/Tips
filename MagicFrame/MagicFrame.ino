#define MAGNET_PIN1 D2
#define MAGNET_PIN2 D5

#define LIGHT_PIN1  D6
#define LIGHT_PIN2  D7

const int k_magnetHz  = 70;     // in Hz
const int k_magnetPwm = 50;     // in percentage (0-100)
const int k_lightOffset = 120;  // im microseconds
const int k_lightPwm    = 30;   // in percentage (0-100)

int g_magnetOn;
int g_magnetSubPeriod;

int g_lightOn;
int g_lightSubPeriod;

void CalculateParams()
{
  g_magnetSubPeriod = 1000000.0 / k_magnetHz / 4;  // Magnet has 4 sub-periods
  g_magnetOn        = g_magnetSubPeriod * k_magnetPwm / 100;

  g_lightSubPeriod = (2 * g_magnetSubPeriod) + k_lightOffset;  // Light has 2 sub-periods
  g_lightOn        = g_lightSubPeriod * k_lightPwm / 100;
}

void setup()
{
  pinMode(MAGNET_PIN1, OUTPUT);
  pinMode(MAGNET_PIN2, OUTPUT);
  
  pinMode(LIGHT_PIN1, OUTPUT);
  pinMode(LIGHT_PIN2, OUTPUT);

  CalculateParams();
}

void loop()
{
  static int s_magnetPeriodState = 0;
  static uint32_t s_magnetPrev  = 0;
  static uint32_t s_magnetDelay = 0;
  if (micros() - s_magnetPrev >= s_magnetDelay) 
  {
    s_magnetPrev = micros();

    digitalWrite(MAGNET_PIN1, LOW);
    digitalWrite(MAGNET_PIN2, LOW);

    if (++s_magnetPeriodState > 3)
      s_magnetPeriodState = 0;
    
    switch (s_magnetPeriodState)
    {
    case 0: // ON period
      digitalWrite(MAGNET_PIN1, HIGH);
      s_magnetDelay = g_magnetOn;
      break;
    case 2: // ON period (reverse polarity)
      digitalWrite(MAGNET_PIN2, HIGH);
      s_magnetDelay = g_magnetOn;
      break;
    case 1: // OFF period
    case 3:
      s_magnetDelay = g_magnetSubPeriod - g_magnetOn;
      break;
    }
  }

  static boolean s_lightState = true;
  static uint32_t s_lightPrev  = 0;
  static uint32_t s_lightDelay = 0;
  if (micros() - s_lightPrev >= s_lightDelay) 
  {
    s_lightPrev = micros();
    s_lightState = !s_lightState;

    digitalWrite(LIGHT_PIN1, s_lightState ? HIGH : LOW );
    digitalWrite(LIGHT_PIN2, s_lightState ? LOW  : HIGH);
    s_lightDelay = s_lightState ? g_lightOn : (g_lightSubPeriod - g_lightOn);
  }
}
