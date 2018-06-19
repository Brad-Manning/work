//Bradley Manning 8/2017

#ifndef CherenkovPhysicsList_h
#define CherenkovPhysicsList_h 1

#include "globals.hh"
#include "G4VUserPhysicsList.hh"

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpBoundaryProcess;
class CherenkovPhysicsListMessenger;

class CherenkovPhysicsList : public G4VUserPhysicsList
{
public:
  CherenkovPhysicsList();
  ~CherenkovPhysicsList();
  
public:
  void ConstructParticle();
  void ConstructProcess();

  void SetCuts();

  //Construct Particles & Register them
  void ConstructBosons();
  void ConstructLeptons();
  void ConstructMesons();
  void ConstructBaryons();

  //Construct Physics Processes
  void ConstructGeneral();
  void ConstructEM();
  void ConstructOp();

  //for messenger
  void SetVerbose(G4int);
  void SetNbOfPhotonsCherenkov(G4int);

private:
  G4Cerenkov* cherenkovProcess;
  G4Scintillation* scintillationProcess;
  G4OpAbsorption* absorptionProcess;
  G4OpRayleigh* rayleighScatteringProcess;
  G4OpBoundaryProcess* boundaryProcess;

  CherenkovPhysicsListMessenger* pMessenger;
};

#endif /*CherenkovPhysicsList.hh*/
  
