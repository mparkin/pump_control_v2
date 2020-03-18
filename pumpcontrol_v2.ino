

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

float runForward, runReverse, hold;
int   speedForward,speedReverse;
int temperature;
int humidity;
float DHTperiod;
unsigned int cycles;
bool StartStop;               

void setup() {
 Serial.begin(115200);    
  virtuino.DEBUG=true;      
  Serial.println("Setup completed.");

pinMode(5, OUTPUT); //motor speed PWM
pinMode(7, OUTPUT); //                       
pinMode(4, OUTPUT); //
}

void loop() {
  virtuino.run();               //  neccesary command to communicate with Virtuino android app
  int value4 = virtuino.vMemoryRead(4);
  if (value4 == HIGH) {
      digitalWrite(4,HIGH);
      digitalWrite(7,LOW);
      analogWrite(5, 5 );
      delay(1000); 
      digitalWrite(4,LOW);
      delay(4000);

    }
    else digitalWrite(4,LOW);

}
