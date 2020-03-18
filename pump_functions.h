
#include "pump_state_machine.h"

class pumpcontrols : public pumpState
{
  private:
     pumpState pState;
     int cycles;
     float seconds;
  public:
     pumpcontrols(){pState.newState(PowerOff);}
     ~pumpcontrols(){pState.newState(PowerOff);}
     initialize();
     powerdown();
     runCycles(int cycles);
     runTimed(float seconds);
};

pumpcontrols::initialize()
{
  
};

pumpcontrols::powerdown()
{
  
};

pumpcontrols::runCycles(int cycles)
{
  
};

pumpcontrols::runTimed(float seconds)
{
  
};
