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
  void AddEdep(G4double edep) { fEdep += edep; fEdep2 += edep*edep;}
public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

private:
  CherenkovRunAction* fRunAction;
  G4double     fEdep;
  G4double     fEdep2;
  G4double fgammaCounter;
};

#endif /*CherenkovEventAction.hh*/
