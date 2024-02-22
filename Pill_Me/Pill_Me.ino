#include <Servo.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>

// CONNECTIONS:
// DS1302 CLK/SCLK --> 11
// DS1302 DAT/IO --> 12
// DS1302 RST/CE --> 13 
ThreeWire myWire(12,11,13); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

uint8_t month, day, hour, minute, second;
uint16_t year;

Servo sv1;
Servo sv2;

int sensor = 2;                                          
int color[] = {5,4,3}; // Array rgb

void RTC_set();
void RTC_loop();
void printDateTime();
void RTC_time();

void green(); // Function สีเขียว
void red(); // Function สีแดง
void white(); // Function สีขาว
void LCD_off();

void setup() {

  white();
  
  RTC_set();

  for(int index = 0; index <= 3 ;index++) //  For Loop ประกาศค่า  Output
  {
  pinMode(color[index], OUTPUT);
  }

  sv1.attach(10); // ขา pin servo
  sv1.write(90); // เริ่มต้นที่ 90 องศา

  sv2.attach(9); // ขา pin servo
  sv2.write(100); // เริ่มต้นที่ 90 องศา

  Serial.begin(9600);
}

void loop() {
  RTC_loop();
}

/*------------------------Funtion------------------------------*/

/*--------------------------RTC--------------------------------*/
void RTC_set()
{
   Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    } 

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
}

void RTC_loop()
{
  RtcDateTime now = Rtc.GetDateTime();
  printDateTime(now);
  RTC_time(now);
}

void printDateTime(const RtcDateTime& dt)
{
  year   = (int)dt.Year();
  month  = (int)dt.Month();
  day    = (int)dt.Day();
  hour   = (int)dt.Hour();
  minute = (int)dt.Minute();
  second = (int)dt.Second();

  int val = digitalRead(sensor);

  
  //แสดงค่าวัน/เวลา
  String str = String(day) + 
               "/" + String(month) + 
               "/" + String(year) + 
               "\t" + String(hour) + 
               ":" + String(minute) + 
               ":" + String(second);
  Serial.print(str);
  Serial.print(" ");
  Serial.print ("\t");
  Serial.print ("val : ");
  Serial.println(val);
  delay(1000);
}

void RTC_time(const RtcDateTime& dt)
{
  hour   = (int)dt.Hour();
  minute = (int)dt.Minute();
  second = (int)dt.Second();


  int val = digitalRead(sensor);

/*-----------------MORNING---------------*/
    if( hour == 7 && minute <= 15 )
    {
      if(val == 0)
      {
        green();

        sv1.write(120);
        sv2.write(70);
        delay(90);
        sv1.write(90);
        sv2.write(100);
        delay(1000);
      }else red();
    }else white();

    if( hour == 8 && minute <= 15 )
    {
      if(val == 0)
      {
        green();

        sv1.write(120);
        sv2.write(70);
        delay(90);
        sv1.write(90);
        sv2.write(100);
        delay(1000);
      }else red();
    }

/*-----------------NOON---------------*/
    if( hour == 11 && minute <= 15 )
    {
      if(val == 0)
      {
        green();

        sv1.write(120);
        delay(90);
        sv1.write(90);
        delay(1000);
      }else red();
    }

    if( hour == 12 && minute <= 15 )
    {
      if(val == 0)
      {
        green();

        sv2.write(70);
        delay(90);
        sv2.write(100);
        delay(1000);
      }else red();
    }

/*--------------EVENING--------------*/
    if( hour == 16 && minute <= 15 )
    {
      if(val == 0)
      {
        green();

        sv1.write(120);
        sv2.write(70);
        delay(90);
        sv1.write(90);
        sv2.write(100);
        delay(1000);
      }else red();
    }

    if( hour == 17 && minute <= 15 )
    {
      if(val == 0)
      {
        green();

        sv2.write(70);
        delay(90);
        sv2.write(100);
        delay(1000);
      }else red();
    }

/*-----------------BF BED---------------*/
    if( hour == 20 && minute <= 15 )
    {
      if(val == 0)
      {
        green();

        sv1.write(120);
        delay(90);
        sv1.write(90);
        delay(1000);
      }else red();
    }

/*-----------------TESTER----------------*/
    if( second <= 50 )
    {
      if(val == 0)
      {
        green();

        sv1.write(120);
        sv2.write(70);
        delay(90);
        sv1.write(90);
        sv2.write(100);
        delay(1000);
      }else red();
    }

    //  if( minute == 48 )
    // {
    //   if(val == 0)
    //   {
    //     green();

    //     sv2.write(70);
    //     delay(90);
    //     sv2.write(100);
    //     delay(1000);
    //   }else red();
    // }
     
}

/*-----------------------------LCD------------------------------*/
void green()
{
  analogWrite(color[0], 255);
  analogWrite(color[1], 0);
  analogWrite(color[2], 255);
}
void red()
{
  analogWrite(color[0], 0);
  analogWrite(color[1], 255);
  analogWrite(color[2], 255);
}
void white()
{
  analogWrite(color[0], 0);
  analogWrite(color[1], 0);
  analogWrite(color[2], 0);
}
void LCD_off()
{
  analogWrite(color[0], 255);
  analogWrite(color[1], 255);
  analogWrite(color[2], 255);
}