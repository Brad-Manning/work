// Bradley Manning

// simple radioactive decay


#include "globals.hh"
#include "simpleRadioactiveDecayPhysicsList.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"

simpleRadioactiveDecayPhysicsList::simpleRadioactiveDecayPhysicsList() : G4VUserPhysicsList()
{}

simpleRadioactiveDecayPhysicsList::~simpleRadioactiveDecayPhysicsList()
{}

void simpleRadioactiveDecayPhysicsList::ConstructParticle()
{
  //Necessary to describe interactions of following particles
  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
}

void simpleRadioactiveDecayPhysicsList::ConstructBosons()
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

void simpleRadioactiveDecayPhysicsList::ConstructLeptons()
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

void simpleRadioactiveDecayPhysicsList::ConstructMesons()
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

void simpleRadioactiveDecayPhysicsList::ConstructBaryons()
{
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();
}

void simpleRadioactiveDecayPhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructGeneral();
  ConstructEM();
  ConstructOp();
}

// May not be necessary, included for accuracy
#include "G4Decay.hh"

void simpleRadioactiveDecayPhysicsList::ConstructGeneral()
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

void simpleRadioactiveDecayPhysicsList::ConstructEM()
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

void simpleRadioactiveDecayPhysicsList::SetCuts()
{
  //G4VUserPhysicsList::SetCutsWIthDefault method sets the
  //default cut value for all particle types
  SetCutsWithDefault();
  if (verboseLevel>0) DumpCutValuesTable();
}
