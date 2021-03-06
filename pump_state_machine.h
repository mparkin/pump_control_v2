
enum state 
{
  PowerOff,
  PowerOn,
  Idle,
  RunFirst,
  RunSecond,
  Hold,
  Stop,
  Change
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
      if (Active == PowerOn)Active = reqState;
      break;
    case RunFirst:
      if (Active == Idle)Active = reqState;
      break;
    case RunSecond:
      if (Active == RunFirst)Active = reqState;
      break;
    case Hold:
      if (Active == RunSecond)Active = reqState;
      break;
    case Stop:
      Active = Stop;
    break;
    default:
      Active = Active;
  };
}
