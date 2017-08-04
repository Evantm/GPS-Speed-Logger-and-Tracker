#include <TinyGPS.h>
#include <SoftwareSerial.h>

#include <SPI.h>
#include <SD.h> 

#include "SSD1306Ascii.h" //I need to buy whoever made this libary a beer
#include "SSD1306AsciiAvrI2c.h"

#define I2C_ADDRESS 0x3C
SSD1306AsciiAvrI2c oled;


float flat, flon;
unsigned long age;
  

TinyGPS gps;
File myFile;
String fileName = "trip-log.csv";

// The serial connection to the GPS device
SoftwareSerial ss(3, 2);

void setup()
{

  oled.begin(&Adafruit128x32, I2C_ADDRESS);
  Serial.begin(115200);
  ss.begin(9600);
  delay(2000);
  while(!SD.begin(9)) {
    delay(500);
    Serial.println("initialization failed!");
  }
 

}

void loop()
{
  
  printFloat(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
  
  gps.f_get_position(&flat, &flon, &age);
  
  smartDelay(1000);
  
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  String mySpeed = String(val,prec);  
 
  myFile = SD.open("log.csv", FILE_WRITE);
  oled.setFont(Adafruit5x7);  
  oled.set2X();
  oled.setCursor(10,0);
  
  if (!valid)
  {
    while (len-- > 1){
      oled.print('*');
    }
    oled.print(' ');
  }
  else
  {
    oled.println(val, prec);
    oled.print(" km/h");

    if(mySpeed == "-1.00")
      return; 
      
    Serial.print(val, prec);
    Serial.print(", ");
    
    Serial.print(print_date(gps));
    Serial.print(",");
    Serial.print(val, prec);
    Serial.print(",");
    Serial.print(flat,6);
    Serial.print(",");
    Serial.println(flon,6);
    
    if(myFile) {
      myFile.print(print_date(gps));
      myFile.print(",");
      myFile.print(val, prec);  
      myFile.print(",");
      myFile.print(flat,6);
      myFile.print(",");
      myFile.println(flon,6);
      myFile.close();
      
      Serial.println("SD WRITE");
      oled.print("~");
    }
    
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i){
      oled.print(' ');
    }
  }
  smartDelay(0);
}

static String print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    Serial.print("********** ******** ");
  else
  {
    char sz[32];
    sprintf(sz, "%02d_%02d_%02d",
        hour, minute, second);
    return(sz);
  }
  smartDelay(0);
}

