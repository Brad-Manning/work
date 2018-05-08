//Bradley Manning 8/2017


#ifndef CherenkovPrimaryGeneratorMessenger_h
#define CherenkovPrimaryGeneratorMessenger_h 1


#include "G4UImessenger.hh"
#include "globals.hh"

class CherenkovPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;

class CherenkovPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  CherenkovPrimaryGeneratorMessenger(CherenkovPrimaryGeneratorAction*);
  ~CherenkovPrimaryGeneratorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:
  CherenkovPrimaryGeneratorAction* cherenkovAction;
  G4UIdirectory* gunDir;
  G4UIcmdWithADoubleAndUnit* polarCmd;
};

#endif /*CherenkovPrimaryGeneratorMessenger.hh*/
