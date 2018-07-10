//Bradley Manning 8/2017

#ifndef CherenkovPhysicsListMessenger_h
#define CherenkovPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class CherenkovPhysicsList;
class G4UIdirectory;
class G4UIcmdWithAnInteger;

class CherenkovPhysicsListMessenger: public G4UImessenger
{
public:
  CherenkovPhysicsListMessenger(CherenkovPhysicsList* );
  ~CherenkovPhysicsListMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:
  CherenkovPhysicsList* pPhysicsList;

  G4UIdirectory* CherenkovDir;
  G4UIdirectory* physDir;
  G4UIcmdWithAnInteger* verboseCmd;
  G4UIcmdWithAnInteger* cherenkovCmd;
};

#endif /*CherenkovPhysicsListMessenger.hh*/
