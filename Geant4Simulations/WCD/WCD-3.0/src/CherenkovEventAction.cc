//Bradley Manning 8/2017


#include "CherenkovEventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

#include "CherenkovRunAction.hh"
#include "CherenkovAnalysis.hh"
#include "G4RunManager.hh"
#include <fstream>


CherenkovEventAction::CherenkovEventAction(CherenkovRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fgammaCounter(0.)
{}

CherenkovEventAction::~CherenkovEventAction()
{}

void CherenkovEventAction::BeginOfEventAction(const G4Event*)
{
  fgammaCounter = 0.;
}


void CherenkovEventAction::EndOfEventAction(const G4Event*)
{
  //G4AnalysisManager* man = G4AnalysisManager::Instance();
  //fRunAction->AddCounter(fgammaCounter);
  //man->FillH1(0,fgammaCounter);
  //G4cout << "hello";
  //std::ofstream nPhotons;
  //nPhotons.open ("nPhotons.txt", std::ios_base::app | std::ios_base::out);
  //nPhotons << fgammaCounter << "\n";
  
}
