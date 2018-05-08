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
  timer->Start();
}

void CherenkovRunAction::EndOfRunAction(const G4Run* aRun)
{
  timer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent() << " " << *timer << G4endl;
}

void CherenkovRunAction::AddCounter(G4double gammaCounter)
{
  fgammaCounter += gammaCounter;
}
