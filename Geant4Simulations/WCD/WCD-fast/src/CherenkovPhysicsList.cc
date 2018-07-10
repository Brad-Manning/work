//Bradley Manning 8/2017
//CherenkovPhysicsLists.cc
//Defines all physics
#include "G4ios.hh"
#include <iomanip>

#include "globals.hh"
#include "CherenkovPhysicsList.hh"
#include "CherenkovPhysicsListMessenger.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4StepLimiter.hh"

CherenkovPhysicsList::CherenkovPhysicsList() : G4VUserPhysicsList()
{
  cherenkovProcess = 0;
  scintillationProcess = 0;
  absorptionProcess = 0;
  rayleighScatteringProcess = 0;
  boundaryProcess = 0;

  pMessenger = new CherenkovPhysicsListMessenger(this);
  SetVerboseLevel(0);
}

CherenkovPhysicsList::~CherenkovPhysicsList() {delete pMessenger;}


void CherenkovPhysicsList::ConstructParticle()
{
  //Necessary to describe interactions of following particles
  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
}

void CherenkovPhysicsList::ConstructBosons()
{
  // G4 pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();
  G4GenericIon::GenericIon();
  // gamma
  G4Gamma::GammaDefinition();

  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();
}

void CherenkovPhysicsList::ConstructLeptons()
{
  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
  
}

void CherenkovPhysicsList::ConstructMesons()
{
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4KaonZero::KaonZeroDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
  G4KaonZeroLong::KaonZeroLongDefinition();
  G4KaonZeroShort::KaonZeroShortDefinition();
}

void CherenkovPhysicsList::ConstructBaryons()
{
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();
}

void CherenkovPhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructGeneral();
  ConstructEM();
  ConstructOp();
}

// May not be necessary, included for accuracy
#include "G4Decay.hh"

void CherenkovPhysicsList::ConstructGeneral()
{
  G4Decay* const decayProcess = new G4Decay();
  
  G4ProcessManager& mumManager =
    *G4MuonMinus::MuonMinusDefinition()->GetProcessManager();  
  mumManager.AddDiscreteProcess(decayProcess);
  mumManager.SetProcessOrdering(decayProcess, idxPostStep);
  mumManager.SetProcessOrdering(decayProcess, idxAtRest);
  
  G4ProcessManager& mupManager =
    *G4MuonPlus::MuonPlusDefinition()->GetProcessManager();  
  mupManager.AddDiscreteProcess(decayProcess);
  mupManager.SetProcessOrdering(decayProcess, idxPostStep);
  mupManager.SetProcessOrdering(decayProcess, idxAtRest);
}

//Other necessary physics effects

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"

//Construct Electromagnetic Processes

void CherenkovPhysicsList::ConstructEM()
{
  GetParticleIterator()->reset();
  while( (*GetParticleIterator())() ){
    G4ParticleDefinition* particle = GetParticleIterator()->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
      //Construct processes for gamma
         pmanager->AddDiscreteProcess(new G4GammaConversion());//, -1, 1, 1);
         pmanager->AddDiscreteProcess(new G4ComptonScattering());//, -1, 2, 2);
         pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());//, -1, 3, 3);
    } else if (particleName == "e-") {
     //Construct processes for electron
      //Numbers imply the order of the processes, negative number means it is inactive at given stage
        pmanager->AddProcess(new G4eMultipleScattering(), -1, 1, 1);
        pmanager->AddProcess(new G4eIonisation(), -1, 2, 2);
	pmanager->AddProcess(new G4eBremsstrahlung(), -1, 3, 3);
    } else if (particleName == "e+") {
      //Construct processes for positron
      pmanager->AddProcess(new G4eMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(), -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(), -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation(), 0, -1, 4);
       
    } else if (particleName == "mu+" || particleName == "mu-" ) {
      //Construct processes for muon+,muon-
      pmanager->AddProcess(new G4MuMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation(), -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung(), -1, 3, 3);
      pmanager->AddProcess(new G4MuPairProduction(), -1, 4, 4);

    } else {
      if ((particle->GetPDGCharge() != 0.0) && (particle->GetParticleName() != "chargedgeantino")) {
	// all other particles except geantino
	pmanager->AddProcess(new G4hMultipleScattering(), -1, 1, 1);
	pmanager->AddProcess(new G4hIonisation(), -1, 2, 2);
      }
    }
  }
}

//Construct Optical Processes
void CherenkovPhysicsList::ConstructOp()
{
  cherenkovProcess = new G4Cerenkov("Cerenkov");
  scintillationProcess = new G4Scintillation("Scintillation");
  absorptionProcess = new G4OpAbsorption();
  rayleighScatteringProcess = new G4OpRayleigh();
  boundaryProcess = new G4OpBoundaryProcess();

  //optional dumps here. eg.
  //cherenkovProcess->DumpPhysicstable();

  SetVerbose(0);
  // Define maximum number of photons generated per step, in water is it ~300 per step; note that this is the maximum mean
  cherenkovProcess->SetMaxNumPhotonsPerStep(3);
  cherenkovProcess->SetTrackSecondariesFirst(true);

  scintillationProcess->SetScintillationYieldFactor(1.);
  scintillationProcess->SetTrackSecondariesFirst(true);

  //Necessary for boundary process ##For older versions of GEANT4##
  //G4OpticalSurfaceModel model = unified;
  //boundaryProcess->SetModel(model);

  //Add discrete processes to optical photons and particles if applicable
  GetParticleIterator()->reset();
  while( (*GetParticleIterator())() ){
    G4ParticleDefinition* particle = GetParticleIterator()->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    G4cout << particleName << G4endl;
    if (cherenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(cherenkovProcess);
      pmanager->SetProcessOrdering(cherenkovProcess, idxPostStep);
    }
    if (scintillationProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(scintillationProcess);
      pmanager->SetProcessOrderingToLast(scintillationProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast(scintillationProcess, idxPostStep);
    }
    if (particleName == "opticalphoton") {
      G4cout << "AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(absorptionProcess);
      pmanager->AddDiscreteProcess(rayleighScatteringProcess);
      pmanager->AddDiscreteProcess(boundaryProcess);
     
    }
  }
}

void CherenkovPhysicsList::SetVerbose(G4int verbose)
{
  cherenkovProcess->SetVerboseLevel(verbose);
  scintillationProcess->SetVerboseLevel(verbose);
  absorptionProcess->SetVerboseLevel(verbose);
  rayleighScatteringProcess->SetVerboseLevel(verbose);
  boundaryProcess->SetVerboseLevel(verbose);
}

void CherenkovPhysicsList::SetNbOfPhotonsCherenkov(G4int MaxNumber)
{
  cherenkovProcess->SetMaxNumPhotonsPerStep(MaxNumber);
}

void CherenkovPhysicsList::SetCuts()
{
  //G4VUserPhysicsList::SetCutsWIthDefault method sets the
  //default cut value for all particle types
  SetCutsWithDefault();
  if (verboseLevel>0) DumpCutValuesTable();
}

  
  

					   
