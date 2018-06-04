//Bradley Manning 8/2017
//CherenkovRunAction.cc
//This pre-initialises environment and controls destructors

#include "G4Timer.hh"
#include "CherenkovRunAction.hh"
#include "G4Run.hh"

#include "G4RunManager.hh"
#include "CherenkovAnalysis.hh"
#include "G4ParameterManager.hh"
#include <fstream>
#include <vector>


CherenkovRunAction::CherenkovRunAction()
  : G4UserRunAction(),
    fgammaCounter("gammaCounter",0.)
{
  timer = new G4Timer;
  //Define Paramters for number of photons
  G4ParameterManager* parameterManager = G4ParameterManager::Instance();
  parameterManager->RegisterParameter(fgammaCounter);

  //Manager instance to create root files
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  G4String fileName = "Cherenkov";
  man->CreateH1("0","Number of Photons",100,0,70000);
  man->CreateH1("1","S1000 (VEM)",100,0,0.2);
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
      if ( line == "0" ) {
	fUseWeights = false;
      }
    }
  }
  std::ofstream finalVEM;
  finalVEM.open ("finalVEM.txt");
  finalVEM << 0 << " " << 0 << " " << tank_pos << "\n";
  G4cout << "Run: Tank Position: " << tank_pos << " # Injected Particles: " << nParticles << G4endl;
  G4cout.rdbuf(NULL);
}

//Destructor
CherenkovRunAction::~CherenkovRunAction()
{
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->Write();
  // G4cout << totalPhotons << G4endl;
  delete G4AnalysisManager::Instance();
  delete timer;
}

void CherenkovRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  
}

void CherenkovRunAction::EndOfRunAction(const G4Run* aRun)
{
  fIdx++;
}

void CherenkovRunAction::AddCounter(G4double gammaCounter)
{
  fgammaCounter += gammaCounter;
}

G4double CherenkovRunAction::GetWeight()
{
  if (fUseWeights)
    {
      return fWeight[fIdx];
    } else {
    return 1;
  }
}
