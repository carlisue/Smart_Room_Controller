/*
 * Project:     Smart_Room_Controller: Cohort 4 Midterm
 * Description: Controls two modes: Default mode (working) and Break mode
 *              to encourge a worker to get up from desk and move their body.
 * Author:      Carli Stringfellow
 * Date:        04-09-2021
 */
 
#include <OneButton.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Encoder.h>
#include <mac.h>
#include <wemo.h>
#include <hue.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>

int currentTime;
int lastSecond;

//ultrasonic declarations
const int echoPin = 7; 
const int trigPin = 6;
unsigned long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
int lastDistance;
int dist_cm;
float inches;

//bme declarations
const int chipSelect = 4;
float tempF;
float humidRH;

Adafruit_BME280 bme;
char D = 248;

//oled declarations
const int SCREEN_WIDTH = 128; // OLED display width, in pixels
const int SCREEN_HEIGHT = 64; // OLED display height, in pixels
const int SCREEN_ADDRESS = 0x3C; 
char N = 164;
const int OLED_RESET = 4; 
int rot = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
EthernetClient client;
bool status;

// orange and yellow button declarations
OneButton orangeButton (14, false);
OneButton yellowButton (15, false);
bool orangeButtonState;
bool yellowButtonState;
const int TEAPOT = 3;
const int FAN = 2;
float f = tempF;
bool manualFan;

// encoder declarations 
 OneButton encoderButton(20, true, true); 
 Encoder myEnc(22,23);
 bool encoderButtonState = true;
 bool firstRelax = true;
 const int REDLED = 16;
 const int GREENLED = 17;
 int position;
 const int ENMIN = 0;
 const int ENMAX = 96;
 const int HUEMIN = 0;
 const int HUEMAX = 12;
 int HueBri;
 int b = 0;
 int HueWork[] = {HueBlue, HueGreen, HueBlue, HueIndigo, HueGreen};
 int HueRelax[] = {HueRed, HueYellow, HueOrange, HueRed, HueYellow};

 // Timer Declarations
 unsigned long startTimer;
 bool timerON = true;
 const int sittingDistance = 50;
 const int sittingTime = 10000;
 
void setup() {
  Serial.begin(9600); //Turns on serial monitor
  bme.begin(0x76); // Turns on bme sensor
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  display.setRotation(rot); // Sets rotation to rot = 0
  lastDistance = 0; 

  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay(); // Clears display buffer
  
  // orangebutton set up
  orangeButton.attachClick(orangeClick);
  orangeButton.setClickTicks(500);
  orangeButton.setPressTicks(2000);
 
  //yellowbutton set up
  yellowButton.attachClick(yellowClick);
  yellowButton.setClickTicks(500);
  yellowButton.setPressTicks(2000);

  //encoderbutton set up
  pinMode(REDLED,OUTPUT);
  pinMode(GREENLED,OUTPUT);
  encoderButton.attachClick(encoderClick);
  encoderButton.attachLongPressStart(resetToDefault);
  encoderButton.setClickTicks(500);
  encoderButton.setPressTicks(2500);

  // Ensures all SPI devices are off
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  
  //Start ethernet connection
  status = Ethernet.begin(mac);
    if (!status) {
      Serial.printf("failed to configure Ethernet using DHCP \n");
      //no point in continueing 
      while(1);
    }
}

void loop() {
  ultraSonic();
  
  orangeButton.tick(); // checks teapot buttonstate
 
  yellowButton.tick(); // checks fan buttonstate
 
  encoderButton.tick(); // encoder controls brightness and toggles hues on/off 
//    if(!encoderButtonState) {
//    setHue(b, false, HueWork[b-1], 0, 0); 
//    }
//    
      defaultMode(); // calls work mode 
//    breakMode();

    if(timerON) {
    if((millis() - startTimer) > sittingTime) {
      if(inches <= sittingDistance) {
        breakMode();
        Serial.printf("timer: \n", timerON);
      }
      timerON = false;
    } 
   
  }

}
void orangeClick() {
  orangeButtonState = !orangeButtonState;
    if(orangeButtonState) {
      switchON(TEAPOT);
    }
    else {
     switchOFF(TEAPOT); 
    }
//  Serial.printf("orangeButtonState = %i", orangeButtonState);
}

void yellowClick() { // toggles fan
  yellowButtonState = !yellowButtonState;
  manualFan = true;
       if(yellowButtonState) {
        switchON(FAN);
      }
      else {
        switchOFF(FAN);
      }
  
//  Serial.printf("yellowButtonState = %i", yellowButtonState);
}
void encoderClick() { // turns hues off
  encoderButtonState = !encoderButtonState;
  Serial.printf("encoderButtonState = %i\n", encoderButtonState);

}

void defaultMode() { //this is work mode as the most time is spent here
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw WHITE text
  display.setCursor(0,0);             // Start at top-left corner
  
  tempF = 1.8*(bme.readTemperature())+32; // Converts celsius to fahrenheit
  display.printf("Tempurature is %0.1f%c\n" ,tempF, D);
//  Serial.printf("Tempurature is %0.1f\n" ,tempF);

  humidRH = bme.readHumidity(); // displays humidity
  display.printf("Humidity is %0.1f\n" ,humidRH);
//  Serial.printf("Humidity is %0.1f\n" ,humidRH);

  display.printf("\n");
  display.printf("\n");

  display.printf("You are in WORK MODE");
  display.display();

    if(manualFan == false) {
      if(tempF >= 75){
      yellowClick();
    }
  }

  position = myEnc.read(); //sets parameter for encoder
  
    if (position > 96) {
      position = 96;
      myEnc.write(96);
    }
  
    if (position < 0) {
          position = 0;
          myEnc.write(0);  
    }
  // default (work mode) lighting  
  HueBri = map(position, ENMIN, ENMAX, HUEMIN, HUEMAX); //maps encoder to huebrightness
//  Serial.printf("%i", HueBri);
  currentTime = millis();
    if((currentTime-lastSecond) > 1000) {
      for(b=1; b<=5; b++) {
        if(encoderButtonState == true) {
          digitalWrite(REDLED,LOW);
          digitalWrite(GREENLED,HIGH);
          setHue(b, true, HueWork[b-1], HueBri*20, 190);
          }
        else {
          digitalWrite(REDLED,HIGH);
          digitalWrite(GREENLED,LOW);
          setHue(b, false, HueWork[b-1], 0, 0);
        }
      }
        lastSecond = millis();
    }
          
//    if(timerON) {
//    if((millis() - startTimer) > sittingTime) {
//      if(inches <= sittingDistance) {
//        breakMode();
//        timerON = false;
//        Serial.printf("timer: \n", timerON);
//      }
//    }
//  }
}


void breakMode() { // automatic mode triggered by ultrasonic wave in inches
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw WHITE text
  display.setCursor(0,0);             // Start at top-left corner
  
  display.printf("\n");
  display.printf("Time to Relax\n");
  display.printf("\n");
  display.printf("Take a Break\n");
  display.printf("\n");
  display.printf("STRETCH YOUR BODY\n");
  display.printf("\n");
  display.display();

  for(b=1; b<=5; b++) {
    setHue(b, true, HueRelax[b-1], 255, 255);
  }

//  if(firstRelax) { // Flashes hue lights for first relax
//   for(int i = 0; i >= 3; i++) {
//    setHue(b, false, HueRelax[b-1], 0, 0);
//    delay(1000);
//    setHue(b, true, HueRelax[b-1], 255, 255);
//    delay(1000);
//    setHue(b, false, HueRelax[b-1], 0, 0);
//    delay(1000);
//    setHue(b, true, HueRelax[b-1], 195, 255);
//      firstRelax = false;
//   }
//  }
 }

void resetToDefault() { // Uses longpress on the encoder to reset to default mode
  defaultMode();
  manualFan = false;
  timerON = true;
  firstRelax = true;
  startTimer = millis();
}
void ultraSonic() {
  //Sends ultrasonic pulse
  digitalWrite(trigPin, LOW); // Clears the trigPin condition
  delayMicroseconds(2);
  
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculates the distance
  distance = duration * 0.34 / 2; // time passed * speed of sound divided by 2 (go and back)
  dist_cm = distance / 10;
  inches = dist_cm * 0.3937;
  
  // Displays the distance on the Serial Monitor
 
//    Serial.printf("Distance:\n\n%i mm\n%i cm\n%0.02f inches\n\n", distance, dist_cm, inches);
    lastDistance = distance;
}
