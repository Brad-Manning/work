//Bradley Manning 8/2017

#include "CherenkovPhysicsListMessenger.hh"

#include "CherenkovPhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"


CherenkovPhysicsListMessenger::CherenkovPhysicsListMessenger(CherenkovPhysicsList* pPhys)
  :pPhysicsList(pPhys)
{
  CherenkovDir = new G4UIdirectory("/Cherenkov/");
  CherenkovDir->SetGuidance("UI commands");

  physDir = new G4UIdirectory("/Cherenkov/phys/");
  physDir->SetGuidance("PhysicsList control");

  verboseCmd = new G4UIcmdWithAnInteger("/Cherenkov/phys/verbose",this);
  verboseCmd->SetGuidance("set verbose for physics processes");
  verboseCmd->SetParameterName("verbose",true);
  verboseCmd->SetDefaultValue(0);
  verboseCmd->SetRange("verbose>=0");
  verboseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  cherenkovCmd = new G4UIcmdWithAnInteger( "/Cherenkov/phys/cherenkovMaxPhotons",this);
  cherenkovCmd->SetGuidance("set max number of photons per step");
  cherenkovCmd->SetParameterName("MaxNumber",false);
  cherenkovCmd->SetRange("MaxNumber>=0");
  cherenkovCmd->AvailableForStates(G4State_Idle);
}

CherenkovPhysicsListMessenger::~CherenkovPhysicsListMessenger()
{
  delete verboseCmd;
  delete cherenkovCmd;
  delete physDir;
  delete CherenkovDir;
}

void CherenkovPhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if ( command == verboseCmd )
    { pPhysicsList->SetVerbose(verboseCmd->GetNewIntValue(newValue));}
  if ( command == cherenkovCmd )
    { pPhysicsList->SetNbOfPhotonsCherenkov(cherenkovCmd->GetNewIntValue(newValue));}
}

							    
