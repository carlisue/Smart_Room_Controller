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
 bool encoderButtonState;
 Encoder myEnc(22,23);
 OneButton encoderButton(20, true, true);  
 const int REDLED = 16;
 const int GREENLED = 17;
 int position;
 const int ENMIN = 0;
 const int ENMAX = 96;
 const int HUEMIN = 0;
 const int HUEMAX = 12;
 int HueBri;
 int b;
 int HueWork[] = {HueBlue, HueGreen, HueBlue, HueIndigo, HueGreen};
 
void setup() {
   Serial.begin(9600);
   bme.begin(0x76);
   display.setRotation(rot);
   
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
  
  orangeButton.tick(); // checks teapot buttonstate
 
  yellowButton.tick(); // checks fan buttonstate
 
  encoderButton.tick(); // encoder controls brightness and toggles hues on/off 

  defaultMode();
  
//  HueBri = map(position, ENMIN, ENMAX, HUEMIN, HUEMAX); //maps encoder to huebrightness
//  Serial.printf("%i", HueBri);
//  currentTime = millis();
//    if((currentTime-lastSecond) > 1000) {
//      for(b=1; b<=5; b++) {
//        if(encoderButtonState == false) {
//          digitalWrite(REDLED,LOW);
//          digitalWrite(GREENLED,HIGH);
//          setHue(b, true, HueWork[b-1], HueBri*20, 190);
//          lastSecond = millis();
//        }
//
//        else {
//          digitalWrite(REDLED,HIGH);
//          digitalWrite(GREENLED,LOW);
//          setHue(b, false, HueWork[b-1],HueBri*20, 0);
//        }
//      }
//    }
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
void encoderClick() { // turns everything off
  encoderButtonState = !encoderButtonState;
  Serial.printf("encoderButtonState = %i", encoderButtonState);
  if(!encoderButtonState)
    setHue(b, false, HueWork[b-1],HueBri*20, 0);   
}

void defaultMode() { //this is also work mode as the most time is spent here
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw WHITE text
  display.setCursor(0,0);             // Start at top-left corner
  
  tempF = 1.8*(bme.readTemperature())+32; // Converts celsius to fahrenheit
  display.printf("Tempurature is %0.1f%c\n" ,tempF, D);
  Serial.printf("Tempurature is %0.1f\n" ,tempF);

  humidRH = bme.readHumidity(); // displays humidity
  display.printf("Humidity is %0.1f\n" ,humidRH);
  Serial.printf("Humidity is %0.1f\n" ,humidRH);

  display.printf("\n");
  display.printf("\n");

  display.printf("You are in WORK MODE");
  display.display();

  manualFan = false;

  HueBri = map(position, ENMIN, ENMAX, HUEMIN, HUEMAX); //maps encoder to huebrightness
  Serial.printf("%i", HueBri);
  currentTime = millis();
    if((currentTime-lastSecond) > 1000) {
      for(b=1; b<=5; b++) {
        if(encoderButtonState == false) {
          digitalWrite(REDLED,LOW);
          digitalWrite(GREENLED,HIGH);
          setHue(b, true, HueWork[b-1], HueBri*20, 190);
          lastSecond = millis();
        }
      }
    }
}
