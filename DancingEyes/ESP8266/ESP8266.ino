#include "Eye.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

Eye g_leftEye (true);
Eye g_rightEye(false);

WiFiServer g_Server(80);    

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

void setup() 
{
  Serial.begin(115200);

  InitServoController();
  
  g_leftEye .Init(0, 1, 2, 3);
  g_rightEye.Init(4, 5, 6, 7);
  
  delay(1000);

  // Connect to WiFi network
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  g_Server.begin();

  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

String getParam(String s, int index)
{
  int end_pos = -1;
  
  for (int i = 0; i < index; i++)
  {
    s = s.substring(end_pos + 1);
    end_pos = s.indexOf(',');
    if (end_pos == -1)
      return s;
  }

  return s.substring(0, end_pos);
}

void parseData(WiFiClient& client, String s)
{
  switch (s[0])
  {
  case 'L':
    {
      s = s.substring(2);

      int x      = getParam(s, 1).toInt();
      int y      = getParam(s, 2).toInt();
      int lids   = getParam(s, 3).toInt();
      int offset = getParam(s, 4).toInt();

      g_leftEye.MoveEye(x, y, lids, offset);
    }
    break;
  case 'R':
    {
      s = s.substring(2);

      int x      = getParam(s, 1).toInt();
      int y      = getParam(s, 2).toInt();
      int lids   = getParam(s, 3).toInt();
      int offset = getParam(s, 4).toInt();

      g_rightEye.MoveEye(x, y, lids, offset);
    }
    break;
    
  default:
    return;  
  }

  client.println("");
}

void loop() 
{
  WiFiClient client = g_Server.available();
  if (!client)
    return;

  Serial.println("Client connected");
  client.setTimeout(5000);
   
  while (client.connected()) 
  {
    while (client.available())
    {
      String input = client.readStringUntil('\r');
      parseData(client, input);
    }
  }

  Serial.println("Disconnected");
}
