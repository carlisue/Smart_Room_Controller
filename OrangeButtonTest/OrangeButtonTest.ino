#include <OneButton.h>
#include <SPI.h>
#include <Ethernet.h>
#include <mac.h>
#include <wemo.h>

EthernetClient client;
bool status;

OneButton orangeButton (14, false);
bool orangeButtonState;

void setup() {
  // sets orangebutton to output
  Serial.begin(9600);
  orangeButton.attachClick(orangeClick);
  orangeButton.setClickTicks(500);
  orangeButton.setPressTicks(2000);

  // Ethernet setup
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
  // put your main code here, to run repeatedly:
  orangeButton.tick();
  if(orangeButtonState) {
    switchON(3);
  }
  else {
   switchOFF(3); 
  }
 
}
void orangeClick() {
  orangeButtonState = !orangeButtonState;
  Serial.printf("orangeButtonState = %i",orangeButtonState);
}
