//Bradley Manning 8/2017


#ifndef CherenkovEventAction_h
#define CherenkovEventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;
class CherenkovRunAction;

class CherenkovEventAction : public G4UserEventAction
{

public:
  CherenkovEventAction(CherenkovRunAction* runAction);
  ~CherenkovEventAction();

  void AddCounter(G4double gammaCounter) { fgammaCounter += gammaCounter;}
public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

private:
  CherenkovRunAction* fRunAction;
  G4double fgammaCounter;
};

#endif /*CherenkovEventAction.hh*/
