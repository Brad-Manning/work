//Bradley Manning 8/2017


#ifndef CherenkovRunAction_h
#define CherenkovRunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include <vector>
class G4Timer;
class G4Run;

class CherenkovRunAction : public G4UserRunAction
{
public:
  CherenkovRunAction();
  ~CherenkovRunAction();

public:
  void BeginOfRunAction(const G4Run* aRun);
  void EndOfRunAction(const G4Run* aRun);
  void AddCounter(G4double gammaCounter);

  void AddEdep (G4double edep);
  G4double GetWeight();
private:
  G4Timer* timer;
  G4Accumulable<G4double> fgammaCounter;
  G4Accumulable<G4double> fEdep;
  G4Accumulable<G4double> fEdep2;
  G4double totalPhotons;
  std::vector<G4double> fWeight;
  bool fUseWeights;
  G4int fIdx = 0;
};

#endif /*CherenkovRunAction.hh*/
