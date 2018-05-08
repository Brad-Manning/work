

#include "SSDSteppingAction.hh"
#include "SSDEventAction.hh"
#include "SSDDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SSDSteppingAction::SSDSteppingAction(SSDEventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0),
  fScoringVolume2(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SSDSteppingAction::~SSDSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void SSDSteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume) { 
    const SSDDetectorConstruction* detectorConstruction
      = static_cast<const SSDDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume(1);   
  }

  if (!fScoringVolume2) { 
    const SSDDetectorConstruction* detectorConstruction
      = static_cast<const SSDDetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume2 = detectorConstruction->GetScoringVolume(2);   
  }
  // get volume of the current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
  //G4LogicalVolume* volume2
    //= step->GetPreStepPoiunt()->GetTouchableHandle()
    // ->GetVolume()->GetLogicalVolume();
      
  // check if we are in scoring volume
  if (volume != fScoringVolume && volume != fScoringVolume2) return;// && volume != fScoringVolume2) return;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);

 
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

