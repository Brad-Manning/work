//Bradley Manning 8/2017



#ifndef CherenkovStackingAction_h
#define CherenkovstackingAction_h 1

#include "globals.hh"
#include "G4UserStackingAction.hh"
class G4StackingAction;
class CherenkovRunAction;

class CherenkovStackingAction : public G4UserStackingAction
{
public:
  CherenkovStackingAction();
  ~CherenkovStackingAction();

  G4double GetWeight(G4int count);
public:
  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  void NewStage();
  void PrepareNewEvent();

private:
  CherenkovRunAction* fRunAction;
  G4int gammaCounter;
  G4int count;
  G4double totalPhotons;
  G4double hits;
  G4double hitsWeight;
};

#endif /*CherenkovStackingAction.hh*/
