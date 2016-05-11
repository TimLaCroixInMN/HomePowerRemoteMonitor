// SEIS 785 Project
// Name: Tim LaCroix
//
// Project: Home Power Remote Monitor and Control
//
// Hardware: Particle Photon and Power Shield, solar panel
// and circuit 12 in the Spark SIK project book
// 
// Firmware: this code for the Photon with libraries included below
// 
// Web Page: ComboBatteryMeter.html with javascript libraries (see Github)
// 
// Integration Services: Particle and Ubidots
//
// NOTE: because this code is built and flashed from the Particle IDE,
// the general Particle library is automatically included in the build.

// include the Ubidots library
#include "Ubidots/Ubidots.h"
// my Ubidots TOKEN
#define TOKEN "jUVLoj0sUc6RPe9BJLyC6pttGd0Xrt"
Ubidots ubidots(TOKEN); 

// include the powershield library
#include "PowerShield/PowerShield.h"
// declare the batteryMonitor object
PowerShield batteryMonitor;

// This is the pin for the little blue LED on the Photon board
const int ledPin = D7;

// connect the motor control line to D0 output on the Photon board
const int motorPin = D6;

// used to avoid too much chatter with integration services
int publishCount = 0;

// measurements acquired from the power shield
double cellVoltage = 0;
double chargeLevel = 0;

// controls current to motor using a transistor gate
int motor(String command) {
    if (command.equalsIgnoreCase("on")) {
        digitalWrite(motorPin, HIGH);
        digitalWrite(motorPin, HIGH);
        return 1;
    }
    else if (command.equalsIgnoreCase("off")) {
        digitalWrite(motorPin, LOW);
        digitalWrite(motorPin, LOW);
        return 1;
    }
    return -1;
}

void setup() {
    // Identify our device to Ubidots service (along with Ubidots Token above)
    ubidots.setDatasourceName("Home Power");

    // declare the control function and variables to monitor through Particle
    Particle.function("motor", motor);
    Particle.variable("cellVoltage", &cellVoltage, DOUBLE);
    Particle.variable("chargeState", &chargeLevel, DOUBLE);
 
    // set up the pins for the LED and the motor control line
    pinMode(ledPin, OUTPUT);
    pinMode(motorPin, OUTPUT); 

    // default the pins to off
    digitalWrite(ledPin, LOW);
    digitalWrite(motorPin, LOW);

    Serial.begin(9600); 
    // start the I2C bus for comm with the power board
    batteryMonitor.begin(); 
    // set up the fuel gauge
    batteryMonitor.quickStart();
    // wait for it to settle down
    delay(1000);
}

void powerManagement(void)
{
    // WARNING: sleep disables WIFI making reflash difficult or impossible from WEB IDE
    //
    // This may require a safe reset and restore of device along with WIFI setup 
    // System.sleep(SLEEP_MODE_DEEP, 5); // 5 seconds
    
    // after sleeping, re-enable comm with the battery monitor
    // Wire.begin(); 
    // batteryMonitor.reset();
    // batteryMonitor.quickStart();
}

void flashLEDs(int count, int delayValue)
{
    // flash the led
    for (int i=0; i<count; i++) {
        digitalWrite(ledPin, HIGH);
        delay(delayValue);
        digitalWrite(ledPin, LOW);
        delay(delayValue);
    }
}

void loop() {
    // toggle the LED so we know the board is alive
    flashLEDs(1, 1000);

    // get voltage and charge percent level from power management board
    cellVoltage = batteryMonitor.getVCell();
    chargeLevel = batteryMonitor.getSoC();
  
    // talk to the internet
    publishCount++;
    if (publishCount >= 10) {
        publishCount = 0;

        // update the Particle dashboard
        Particle.publish("ps-voltage", String(cellVoltage), 60, PUBLIC);
        delay(100);
        Particle.publish("ps-charge", String(chargeLevel), 60, PUBLIC);
        delay(100);
        
        // toggle the LED fast so we know the data was sent     
        flashLEDs(2, 200);

        // update the ubidots dashboard
        ubidots.add("Battery Voltage", cellVoltage);
        ubidots.add("Charge Level", chargeLevel);
        ubidots.sendAll();
   
        // toggle the LED fast so we know the data was sent     
        flashLEDs(2, 200);
    }
  
    // Sleep to conserve power
    powerManagement();
}