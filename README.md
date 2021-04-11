# Smart_Room_Controller
#### Midterm IoT Project
#### Carli Stringfellow
#### 04-12-2021

### **Overview**
This smart room controller offers a unique use of Phillips Hue Lighting control 
to set the mood in a workspace. There are two modes *defaultMode* and *breakMode*.
The first being a working mode mood and the latter is an automatic mood triggered
by an ultrasonic sensor timer, which goes off every hour if one has not moved from
their desk. This mood sets the lighting to warmer colors to encourge a break and 
movement of the body as this is very beneficial for efficent work.

### **Specifics**
1. **Controls Hue Lights**
  * *The defaultMode sets a work tone with blue, green, and indigo colors*
  * *The breakMode sets a relaxing tone with orange, red, and yellow colors*
  * *The encoder allows one to control the brightness of the defaultMode lighting* 
  *and toggles the hue lights on and off*

2. **Reads BME Sensor Data**
  * *Temperature and humidity are writen to OLED screen*
  * *Temperature over 75 degrees will trigger fan to ON*

3. **Ultrasonic Sensor**
  * *Toggles defaultMode and breakMode by detecting distance in inches*
  * *Detects whether or not someone has been sitting at their desk for
  *too long using a timer function.*

4. **Buttons**
  * *Orange button toggles teapot wemo outlet*
  * *Yellow button toggles fan wemo outlet*

5. **Neopixels**
  * *Cycle through different colors to mimic an alarm for breakMode*

### **Components** 
* Philips Hue Lights
* Adafruit Colorful Round Tactile Button Switch (2)
* Ultrasonic SR-04
* Teensy Board 3.1
* Standard USB Cable
* OLED Display Module 12864 128x64 Pixel
* Adafruit BME Sensor
* Ethernet

### **Summary** 
* Check out my project on hackster.io: [hackster.io](https : //www.hackster.io/carlisue1)
* Check out my coding on GitHub: [GitHub](https : //www.github.com/carlisue)

