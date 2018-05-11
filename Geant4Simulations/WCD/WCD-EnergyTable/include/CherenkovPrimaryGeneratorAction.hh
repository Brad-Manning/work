//Bradley Manning 8/2017

#ifndef CherenkovPrimaryGeneratorAction_h
#define CherenkovPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class CherenkovPrimaryGeneratorMessenger;

class CherenkovPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  CherenkovPrimaryGeneratorAction();
  ~CherenkovPrimaryGeneratorAction();

public:
  void GeneratePrimaries(G4Event*);
  
  void SetOptPhotonPolar();
  void SetOptPhotonPolar(G4double);

private:
  G4ParticleGun* particleGun;
  CherenkovPrimaryGeneratorMessenger* gunMessenger;
};

#endif /*CherenkovPrimaryGeneratorAction.hh*/
