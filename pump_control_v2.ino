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

#define POWERSWITCH 18

SoftwareSerial bluetoothSerial =  SoftwareSerial(10,11);   
VirtuinoBluetooth virtuino(bluetoothSerial,9600); 
pumpcontrols pump1;

float runTime1, runTime2, holdTime,runTime;       
unsigned int   pumpSpeedCurrent,pumpSpeedRun,pumpSpeedSuck,pumpSpeedHold;
int temperature;
int humidity;
float DHTperiod;
unsigned int cycles;
bool StartStop, runType,dir,dir1;
state currentState;               

void setup() {
  cli(); //this shuts down interrupts and allows setup to initialize and configure the timers needed in loop.

//set timer1 interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  Serial.begin(115200);    
  virtuino.DEBUG=true;      
  Serial.println("Setup completed.");
  pump1.initialize(4,7,5);
  currentState = pump1.current();
  attachInterrupt(digitalPinToInterrupt(POWERSWITCH), switchIsr, RISING);
  sei();//allow interrupts again after setting everything up
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

};

void goNextState()
{
  switch (currentState)
  {
    case Stop:
    case Idle:
      currentState = RunFirst;
      runTime = runTime1;
    case RunFirst:
      currentState = RunSecond;
      runTime = runTime2;      
    case RunSecond:
      currentState = Hold;
      runTime = holdTime;      
    default:
      currentState = Stop;
  }
}

void TimerIsr() //this is the function that occurs when the hardware interval timer hits zero
{
  //main function is to reload the timer conpare register, change the pump state and then restart the timer.
  // the timer trimesout every 1 second. Then it decrements the runTime counter. 
  // When runTime counter == 0, it sets the run state to next value.
  runTime--; 
  if (runTime <= 0 ) goNextState();
  
};

void switchIsr() // when the staurt/Stop switch is operated. 
{
  //main function is to initiate or stop pump actions by setting the pump state and initiating the internal timer
  if (StartStop)            //running
  {
    currentState = Stop;
    StartStop = false;     //stop pump
  }

 else                       //not running 
 {
    currentState = RunFirst;
    StartStop = true;       //start pump
 }
}
