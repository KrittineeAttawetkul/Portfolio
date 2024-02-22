#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <TridentTD_LineNotify.h>

#define SSID        "50Bath/hr"   //ใส่ชื่อ Wifi
#define PASSWORD    "krittin33"   //ใส่รหัส Wifi
#define LINE_TOKEN  "zLqK2jXHEiORAJJtBKPGFyYXlmxXBRhOh9boGDPDXL8"   //ใส่ TOKEN

int trigPin = D1;
int echoPin = D2;
int red = D6;

int dur, dis;

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(red, OUTPUT);
  
  Serial.begin(9600);
  Serial.println(LINE.getVersion());
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting ",  SSID);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
  LINE.setToken(LINE_TOKEN);
  
}

void loop()
{ 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(trigPin, LOW);

  dur = pulseIn(echoPin, HIGH);
  dis = dur*0.034/2;
  
  Serial.print(dis);
  Serial.print(" cm\n");
  
  if (dis == 10) 
   {
    //LINE.notify("ระยะ = " + String(dis) + " cm");
    LINE.notifySticker("THE TRASH IS FULL",6370, 11088027);
    
    digitalWrite(red,HIGH);
    delay(600000);
    digitalWrite(red,LOW);
  } 
  
}
