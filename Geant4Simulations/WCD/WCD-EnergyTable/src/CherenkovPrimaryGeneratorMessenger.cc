//Bradley Manning 8/2017

#include "CherenkovPrimaryGeneratorMessenger.hh"

#include "CherenkovPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

CherenkovPrimaryGeneratorMessenger::CherenkovPrimaryGeneratorMessenger(
						    CherenkovPrimaryGeneratorAction* cherenkovGun)

:cherenkovAction(cherenkovGun)
{
  gunDir = new G4UIdirectory("/Cherenkov/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");

  polarCmd = new G4UIcmdWithADoubleAndUnit("/Cherenkov/gun/optPhotonPolar",this);
  polarCmd->SetGuidance("Set Linear Polarisation");
  polarCmd->SetGuidance("  angle w.r.t. (k,n) plane");
  polarCmd->SetParameterName("angle",true);
  polarCmd->SetUnitCategory("Angle");
  polarCmd->SetDefaultValue(-360.0);
  polarCmd->SetDefaultUnit("deg");
  polarCmd->AvailableForStates(G4State_Idle);
}

CherenkovPrimaryGeneratorMessenger::~CherenkovPrimaryGeneratorMessenger()
{
  delete polarCmd;
  delete gunDir;
}

void CherenkovPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == polarCmd ) {
    G4double angle = polarCmd->GetNewDoubleValue(newValue);
    cherenkovAction->SetOptPhotonPolar(angle);
  
  }
}

