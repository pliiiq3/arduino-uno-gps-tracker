#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//#include <GSMSim.h>

#define RX 7
#define TX 8
#define RESET 2
#define BAUD 19200


#define Float lat4 ;
#define Float lng4 ;

//SIM800L 
//GSMSim gsm;

//Sim800L device
SoftwareSerial MySim(7,8); //SIM800L Tx & Rx is connected to Arduino #3 & #2
static const uint32_t SimBaud = 19200;

String message;
String GPSAdresse;

/*
   This sample code demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires th-e use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

//GPS device 
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 19200;



// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(BAUD);
  ss.begin(GPSBaud);
  gpsInitializing();
  MySimInitializing();

}

void loop()
{
 
  while (MySim.available() )
  {
message=MySim.readString();
Serial.println(message);
//Serial.println(message);
if (message.indexOf("Code121")>=0)
{
  gpsInitializing();
 Serial.println("gps ist ");
 MySimSmsSenden();
// MySimsmsSenden();
}


    delay(250);
  }


 //   displayInfo();
  
 // GpsDataSenden();
  // displayInfo();
  if (gps.location.isValid())
  {
   // displayInfo();
      gpsInitializing();
  //  smsSenden();
 
  }

  smartDelay(1000);

 // if (millis() > 5000 && gps.charsProcessed() < 10)
   // Serial.println(F("No GPS data received: check wiring"));
}
// end loop




void gpsInitializing()
{
  smartDelay(1000);
  if (gps.location.isValid())
  {



  String TempMessage = "https://www.google.com/maps/search/?api=1&query=";
    TempMessage += String(gps.location.lat(), DEC);  // url += String(gps.location.lat(), DEC);
    TempMessage += ",";
    TempMessage += String(gps.location.lng(), DEC);  //    url += String(gps.location.lng(), DEC);
     TempMessage += "  at ";
 

 char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", gps.date.month(), gps.date.day(), gps.date.year());
     String temp2( sz) ;
      TempMessage += temp2;
       TempMessage += "  " ;
  // Serial.println(sz);


   char sz2[32];
    sprintf(sz2, "%02d:%02d:%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());
  //  Serial.println(sz2);
 String temp3( sz2) ;
      TempMessage += temp3;
       TempMessage += "  " ;



    
    GPSAdresse=TempMessage;

  }
}


void MySimInitializing()
{
//Serial.begin(9600);
  
  MySim.begin(SimBaud);
  Serial.println("Initializing..."); 
  delay(1000);

  MySim.println("AT+CMGF=1"); // Configuring TEXT mode
  
  MySim.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
 // pinMode(4, OUTPUT);
 // pinMode(5, OUTPUT);  

  Serial.println("end Initializing  ."); 
}


void MySimSmsSenden()
{
  //  mySerial.begin(9600);
  delay(1000);
  MySim.println("AT");
  delay(500);
  MySim.println("AT+CMGF=1");
  delay(500);
  // telefonnummer  
  MySim.println("AT+CMGS=\"+4915000000000\"\r");
  delay(500);

 Serial.println("The Sms : ");
  Serial.println(GPSAdresse);
  MySim.print(GPSAdresse);
  delay(500);
  MySim.write(26);
 

}








//print Gps  
void displayInfo()
{
 printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  printInt(gps.location.age(), gps.location.isValid(), 5);
  printDateTime(gps.date, gps.time);
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
  Serial.println("end ");



//Temp
  String TempMessage = "https://www.google.com/maps/search/?api=1&query=";

    TempMessage += String(gps.location.lat(), DEC);  // url += String(gps.location.lat(), DEC);
    TempMessage += ",";
    TempMessage += String(gps.location.lng(), DEC);  //    url += String(gps.location.lng(), DEC);
     TempMessage += "  at ";


 char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", gps.date.month(), gps.date.day(), gps.date.year());
     String temp2( sz) ;
      TempMessage += temp2;
       TempMessage += "  " ;
  // Serial.println(sz);


   char sz2[32];
    sprintf(sz2, "%02d:%02d:%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());
  //  Serial.println(sz2);
 String temp3( sz2) ;
      TempMessage += temp3;
       TempMessage += "  " ;
Serial.println(" is ");
Serial.println(TempMessage);

}




//------------------------------------------------------------------
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
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}
