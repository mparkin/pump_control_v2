

/*   Requirements
//   Virtual Switch  for Start/Stop
//   Start/Stop  sw  pin8 affects same boolean as Virt Start/Stop
//   Virtual Sw EMO  hw PIN9 boolean
//   EMO pin9 
//   Analog input pin 6 Temp/Humidity  DHT11
//                         4,7
                    //     L,L stop
                    //     H,L cw
                    //     L,H ccw  L298 driver
// Control Registers 
     Pump On Time & speed(0-255)
     Pump Hold Time & speed 
     Pump SuckbackTime & speed
     Pump Wait
     cycle counter
     temperature
     Humidity
     DHT11 cycle time
       
*/

#include "pump_functions.h"
#include "VirtuinoBluetooth.h"                   
#include <SoftwareSerial.h>            
 
SoftwareSerial bluetoothSerial =  SoftwareSerial(10,11);   
VirtuinoBluetooth virtuino(bluetoothSerial,9600); 
pumpcontrols pump1;

float runForwardTime, runReverseTime, holdTime;       
unsigned int   pumpSpeedCurrent,pumpSpeedRun,pumpSpeedSuck,pumpSpeedHold;
int temperature;
int humidity;
float DHTperiod;
unsigned int cycles;
bool StartStop, runType,dir,dir1;
state currentState;               

void setup() {
  cli();
  Serial.begin(115200);    
  virtuino.DEBUG=true;      
  Serial.println("Setup completed.");
  pump1.initialize(4,7,5);
  currentState = pump1.current();
}

void loop() {
  virtuino.run();                        //  neccesary command to communicate with Virtuino android app
  int rotation_type = virtuino.vMemoryRead(4);
  //need to put other virtuino reads for these registers.
  //dummy values here to use until then
  // begin dummy variables
  dir = HIGH;
  dir1 = LOW;
  pumpSpeedRun = 120;
  pumpSpeedSuck = 120;
  pumpSpeedHold = 120;
  cycles = 5;
  // end of dummy variables
  
  if ( currentState != pump1.current()) //the start stop interrupt will change pump1.,current to runFirst to start this
  {    switch(pump1.current())           // the pump will run until the duration timer runs to zero then an ISR will change
       {
           case RunFirst:
              pumpSpeedCurrent = pumpSpeedRun;
              break;
           case RunSecond:
              pumpSpeedCurrent = pumpSpeedSuck;
              break;
           default:
               pumpSpeedCurrent = 0;
             
       }
       pump1.run(dir,dir1,pumpSpeedCurrent);   // pump1 state to runSecond
       currentState = pump1.current();
  }

}
