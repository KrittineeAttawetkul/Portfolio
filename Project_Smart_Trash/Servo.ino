#include <Servo.h>
int trig = D0;
int echo = D1;

float duration,distance;

Servo sv1;

void setup()
{
  Serial.begin(9600);
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  
  sv1.attach(D6);
  sv1.write(5);
}

void loop()
{
  digitalWrite(trig,LOW);
  delayMicroseconds(1);
  digitalWrite(trig,HIGH);
  delayMicroseconds(1);
  digitalWrite(trig,LOW);

  duration = pulseIn(echo, HIGH);
  distance = duration*0.034/2;

  Serial.print(distance);
  Serial.print("cm\n");
  
  if(distance <= 20)
    {
       sv1.write(180);
       delay(2000);
    }
  else
    {
      sv1.write(5);
      delay(1000);
    }
}
  
