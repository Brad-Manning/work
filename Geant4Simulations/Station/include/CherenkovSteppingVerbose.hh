//Bradley Manning 8/2017


class CherenkovSteppingVerbose;

#ifndef CherenkovSteppingVerbose_h
#define CherenkovSteppingVerbose_h 1


#include "G4SteppingVerbose.hh"

class CherenkovSteppingVerbose : public G4SteppingVerbose
{
public:
  CherenkovSteppingVerbose();
  ~CherenkovSteppingVerbose();

  void StepInfo();
  void TrackingStarted();

};

#endif /*CherenkovSteppingVerbose.hh*/
