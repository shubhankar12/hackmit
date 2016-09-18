/* Analog Read to LED
 * ------------------ 
 *
 * turns on and off a light emitting diode(LED) connected to digital  
 * pin 13. The amount of time the LED will be on and off depends on
 * the value obtained by analogRead(). In the easiest case we connect
 * a potentiometer to analog pin 2.
 *
 * Created 1 December 2005
 * copyleft 2005 DojoDave <http://www.0j0.org>
 * http://arduino.berlios.de
 *
 */


/*
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "hackmit-bc42a.firebaseio.com"
#define FIREBASE_AUTH "V73Dt8zTwjtPcp7BEYD8BpRx2XzuQvqKMAMlGpyI"
#define WIFI_SSID "LenhartFamily"
#define WIFI_PASSWORD "correcthorsebatterystaple"


void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

*/

//POTENTIOMETER STUFF
int potPin = A0;    // select the input pin for the potentiometer
int ledPin = 13;   // select the pin for the LED
double potRead = 0;       // variable to store the value coming from the sensor
double angle = 0;
double potCenterRead = 591;


//HALL EFFECT STUFF
int halfRevs;
float rpm;
unsigned long timeold;
int hallEffectPin = 0;


//This function is called whenever a magnet/interrupt is detected by the arduino
 void magnet_detect()
 {
  //increment the # of half revolutions
   halfRevs++;
 }


void setup() {
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
  Serial.begin(9600);      // open the serial port at 115200 bps:    

  //Initialize the intterrupt pin (Arduino digital pin 2)
   attachInterrupt(hallEffectPin, magnet_detect, RISING);
   
   halfRevs = 0;
   rpm = 0;
   timeold = 0;
}

void loop() {

  //GET VALUE FROM THE POTENTIOMETER
  potRead = analogRead(potPin);  // read the value from the sensor
  if (potRead >= potCenterRead)  // normalizing the potentiometer value to get the angle 
     {  potRead = potRead - potCenterRead ; 
        angle = potRead/511; 
        angle = angle * 810; //extra factor of 9 to deal with weird non-linearity of potentiometer
     }
  else 
    {
       potRead = potCenterRead - potRead; 
       angle = potRead/511;
       angle = -angle * 110;     
    }

  //CALCULATE RPMs
  if (halfRevs >= 2) { 
     rpm = float(halfRevs) * 30000 / (millis()-timeold); //30000 because divide revs by 2
     timeold = millis();
     halfRevs = 0;
     }
     

  //DEBUGGING AREA
  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print(" | ");
  Serial.print("RPM: ");
  Serial.print(rpm);
  Serial.print('\n');

  //blink the status LED
  digitalWrite(ledPin, HIGH);  // turn the ledPin on
  delay(10);              // stop the program for some time
  digitalWrite(ledPin, LOW);   // turn the ledPin off
}

