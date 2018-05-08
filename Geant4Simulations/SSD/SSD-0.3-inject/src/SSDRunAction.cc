// Bradley Manning 3/2018
//SSDRunAction.cc
//Initialises all objects


#include "SSDRunAction.hh"
#include "SSDPrimaryGeneratorAction.hh"
#include "SSDDetectorConstruction.hh"
#include "SSDAnalysis.hh"
// #include "SSDRun.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4ParameterManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <cstdio>
#include <fstream>
#include <vector>

SSDRunAction::SSDRunAction()
: G4UserRunAction(),
  fEdep("Edep", 0.),
  fEdep2("Edep2", 0.)
{ 

  // Register parameter to the parameter manager
  G4ParameterManager* parameterManager = G4ParameterManager::Instance();
  parameterManager->RegisterParameter(fEdep);
  parameterManager->RegisterParameter(fEdep2);

  G4AnalysisManager* man = G4AnalysisManager::Instance();

  G4String fileName = "SSD";

  man->CreateH1("0","Energy Deposit",100, 0.000000, 15*MeV);
  man->CreateH1("1","Energy Deposit Variance", 100, 0.000000, 16.0*MeV);
  man->CreateH1("2","Energy Deposit (MIP)", 100, 0.000000, 3);
  man->OpenFile(fileName);

  std::string nParticles;
  std::string tank_pos;
  std::string line;
  bool var = false;
  std::ifstream myFile("runInfo.txt");
  std::ifstream weightAll ("weightAll.txt");
  G4double w;
  G4int line_no = 0;
  while(myFile >> line) {
    line_no++;
    if (line_no == 3 ) { nParticles = line; }
    if (line_no == 1 ) {    
      if ( var == 0 ) tank_pos = line;
      var = true;
    }
    if (line_no == 2) {
      if ( line == "1" )
	{
	  fUseWeights = true;
	  while (weightAll >> w)
	    {
	      fWeight.push_back (w);
	    }
	}
      if ( line == "0" ) { fUseWeights = false; }
    }
  }
  G4cout << "Run: Detector Position: " << tank_pos << " # Injected Particles: " << nParticles << G4endl;
  
  G4cout.rdbuf(NULL); 
  
}

SSDRunAction::~SSDRunAction()
{
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->Write();
  delete G4AnalysisManager::Instance();
}

void SSDRunAction::BeginOfRunAction(const G4Run* run)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset parameters to their initial values
  G4ParameterManager* parameterManager = G4ParameterManager::Instance();
  parameterManager->Reset();

}

void SSDRunAction::EndOfRunAction(const G4Run* run)
{
  
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge parameters 
  G4ParameterManager* parameterManager = G4ParameterManager::Instance();
  parameterManager->Merge();

   const SSDDetectorConstruction* detectorConstruction
   = static_cast<const SSDDetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const SSDPrimaryGeneratorAction* generatorAction
   = static_cast<const SSDPrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }
  //Increment weight index ready for new run   
  fIdx++;
}

void SSDRunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}

G4double SSDRunAction::GetWeight()
{
  if (fUseWeights)
    {
      return fWeight[fIdx];
    } else {
    return 1;
  }
}

