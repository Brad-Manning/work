//Bradley Manning

#ifndef StationSteppingAction_h
#define StationSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class CherenkovEventAction;

class G4LogicalVolume;

/// Stepping action class
/// 

class StationSteppingAction : public G4UserSteppingAction
{
  public:
    StationSteppingAction(CherenkovEventAction* eventAction);
    virtual ~StationSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    CherenkovEventAction*  fEventAction;
    G4LogicalVolume* fScoringVolume;
    G4LogicalVolume* fScoringVolume2;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
