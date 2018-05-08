#include "SSDActionInitialization.hh"
#include "SSDPrimaryGeneratorAction.hh"
#include "SSDRunAction.hh"
#include "SSDEventAction.hh"
#include "SSDSteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SSDActionInitialization::SSDActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SSDActionInitialization::~SSDActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SSDActionInitialization::BuildForMaster() const
{
  SSDRunAction* runAction = new SSDRunAction;
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SSDActionInitialization::Build() const
{
  SetUserAction(new SSDPrimaryGeneratorAction);

  SSDRunAction* runAction = new SSDRunAction;
  SetUserAction(runAction);
  
  SSDEventAction* eventAction = new SSDEventAction(runAction);
  SetUserAction(eventAction);
  
  SetUserAction(new SSDSteppingAction(eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
