void setup() 
{
  Serial.begin(115200);
  //Serial.println("Started");
}

bool map_if_in_range(int& val, int from_min, int from_max, int to_min, int to_max)
{
  if (val >= from_min && val <= from_max)
  {
    val = map(val, from_min, from_max, to_min, to_max);
    return true;
  }

  return false;
}

void loop()
{
  int x1 = analogRead(A0);
  int y1 = analogRead(A1);

  int x2 = analogRead(A2);
  int y2 = analogRead(A3);

  //Serial.print(x1); Serial.print(", ");
  //Serial.print(y1); Serial.print(", ");
  //Serial.print(x2); Serial.print(", ");
  //Serial.print(y2); Serial.print(" ... ");

  int x1_0 = 170;
  int y1_0 = 165;
  int x2_0 = 162;
  int y2_0 = 167;

  int dt = 3;

  if (!map_if_in_range(x1,        0, x1_0-dt,   100,    0) &&
      !map_if_in_range(x1,  x1_0+dt, 340,  0,  -100))
  {
    x1 = 0; // In dead zone
  } 

  if (!map_if_in_range(y1,        0, y1_0-dt,  -100,   0) &&
      !map_if_in_range(y1,  y1_0+dt,     340,     0, 100))
  {
    y1 = 0; // In dead zone
  }

  if (!map_if_in_range(x2 ,       0, x2_0-dt,  -100,   0) &&
      !map_if_in_range(x2,  x2_0+dt,     340,     0, 100))
  {
    x2 = 0; // In dead zone
  } 

  if (!map_if_in_range(y2,        0, y2_0-dt,  100,    0) &&
      !map_if_in_range(y2,  y2_0+dt,     340,    0, -100))
  {
    y2 = 0; // In dead zone
  }

  Serial.print(x1); Serial.print(", ");
  Serial.print(y1); Serial.print(", ");
  Serial.print(x2); Serial.print(", ");
  Serial.print(y2); Serial.println("");

  // Publish data with freq: 20 Hz
  const int publishFreq = 20;
  delay(1000 / publishFreq);
}
