
#ifndef SSDEventAction_h
#define SSDEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class SSDRunAction;

/// Event action class
///

class SSDEventAction : public G4UserEventAction
{
  public:
    SSDEventAction(SSDRunAction* runAction);
    virtual ~SSDEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

  void AddEdep(G4double edep) { fEdep += edep; fEdep2 += edep*edep;}

  private:
    SSDRunAction* fRunAction;
    G4double     fEdep;
    G4double     fEdep2;
    G4int        count;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
