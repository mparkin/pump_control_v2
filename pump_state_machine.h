
enum state 
{
  PowerOff,
  PowerOn,
  Idle,
  RunForward,
  RunReverse,
  Hold,
  Stop
};




class pumpState
{
  public:
    pumpState(){Active = PowerOff;}
    current();
    newState(state newState);
    ~pumpState() { Active = PowerOff;}
  private:
    state Active;
};


pumpState::current()
{
  return Active;
};

pumpState::newState(state reqState)
{
  switch (reqState)
  {
    case PowerOn:
      if (Active == PowerOff)Active = reqState;
      break;
    case PowerOff:
      Active = reqState;
      break;
    case Idle:
      if (Active == Idle)Active = reqState;
      break;
    case RunForward:
      if (Active == Idle)Active = reqState;
      break;
    case RunReverse:
      if (Active == RunForward)Active = reqState;
      break;
    case Hold:
      if (Active == RunReverse)Active = reqState;
      break;
    case Stop:
      Active = Stop;
    break;
    default:
      Active = Active;
  };
}
