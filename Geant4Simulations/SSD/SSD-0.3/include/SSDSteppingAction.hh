
#ifndef SSDSteppingAction_h
#define SSDSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class SSDEventAction;

class G4LogicalVolume;

/// Stepping action class
/// 

class SSDSteppingAction : public G4UserSteppingAction
{
  public:
    SSDSteppingAction(SSDEventAction* eventAction);
    virtual ~SSDSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    SSDEventAction*  fEventAction;
    G4LogicalVolume* fScoringVolume;
    G4LogicalVolume* fScoringVolume2;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
