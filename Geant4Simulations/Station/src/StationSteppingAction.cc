//Bradley Manning

// Geant4 Auger station simulator

#include "StationSteppingAction.hh"
#include "CherenkovEventAction.hh"
#include "CherenkovDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

StationSteppingAction::StationSteppingAction(CherenkovEventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0),
  fScoringVolume2(0)
{}

StationSteppingAction::~StationSteppingAction()
{}

void StationSteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume) {
    const CherenkovDetectorConstruction* detectorConstruction
      = static_cast<const CherenkovDetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume(1);
  }
  if (!fScoringVolume2) {
    const CherenkovDetectorConstruction* detectorConstruction
      = static_cast<const CherenkovDetectorConstruction*>
      (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume2 = detectorConstruction->GetScoringVolume(2);
  }

  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();

  if (volume != fScoringVolume && volume != fScoringVolume2) return;

  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);
}

