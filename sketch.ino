
/*
CDH Intro-Task
Name: Mateo Garzon Velasco
Program: BEng Software Engineering
Practical Section
*/
//libraries for servo, RTC and microSD card
#include <Servo.h>;
#include <RTClib.h>;
#include <SdFat.h>;
#include <Wire.h>; //for I2C 

//the servo, RTC and SDfat objects
Servo servant; 
RTC_DS1307 rtc1;
SdFat sd;


// creating a date-time object



//pins for potentiometer and servo
int potpin = 0;
int serpin = 9;

//potentiometer and servo signals
int potvalue;
int servalue;

//setup function
void setup() {

  servant.attach(serpin); //attach servo to pin

  Serial.begin(9600); // The baud is how fast the info travels

  //SD card control flow (in case of error)
  if (!sd.begin(10)) {
    Serial.println("SD card initialization failed.");
    return;
  }
  


  //RTC control flow (in case of error)
  if (!rtc1.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
    
}




void loop() {

  //servo 
  potvalue = analogRead(potpin);
  servalue = map(potvalue, 0, 1023, 180, 0); //switched 0 and 180
  servant.write(servalue);



  //starting the clock
  rtc1.begin();

  //printing date and time
  DateTime now = rtc1.now();
  Serial.print("[");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print("]  ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);

  //angle of servo-arm added
  Serial.print("  Angle: ");
  Serial.print(-servalue + 90);//math to make 90 start up
  Serial.println(" degrees");

  // Check if a data.txt file exists; if not, create one
  if (!sd.exists("data.txt")) {
    File dataFile = sd.open("data.txt", FILE_WRITE);
    dataFile.println("Date, Time"); // Header for the CSV file
    dataFile.close();
  }

  // Open the data.txt file in append mode
  File dataFile = sd.open("data.txt", FILE_WRITE);

  if (dataFile) {
  // Format the date and time as a string
    String dateTimeStr = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()) + " " +
                         String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + "  Angle: " +
                         String(-servalue + 90)  + " degrees.";
  // Write the date and time to the file
    dataFile.println(dateTimeStr);
    dataFile.close();
  }

  delay(3000);
  
}