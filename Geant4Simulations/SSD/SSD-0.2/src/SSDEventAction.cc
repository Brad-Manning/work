// Bradley Manning 5/2017
// SSDEventAction.cc

#include "SSDEventAction.hh"
#include "SSDRunAction.hh"
#include "SSDAnalysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"


#include <fstream>

G4double totalVEM;

SSDEventAction::SSDEventAction(SSDRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.),
  fEdep2(0.),
  count(0.)
{} 

SSDEventAction::~SSDEventAction()
{}

void SSDEventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
  fEdep2 = 0.;
}


void SSDEventAction::EndOfEventAction(const G4Event*)
{
  G4double weight;
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  fRunAction->AddEdep(fEdep2);
  //Get root manager
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  weight = fRunAction->GetWeight(count);
  //weight = 1;
  count++;
  // G4cout << count << G4endl;
  // MAGIC NUMBER FOR MIP FROM 100K RUNS OF VERTICAL MUON.
  man->FillH1(2,fEdep/2.41, weight);
  man->FillH1(0,fEdep);
  man->FillH1(1,fEdep2, weight);
  //Record energy values
  std::ofstream eValues;
  eValues.open ("eValues.txt", std::ios_base::app | std::ios_base::out);
  eValues << fEdep << "\n";
  totalVEM += fEdep*weight;
  G4cout << fEdep << " with weight " << weight << G4endl;
  //Record VEM values
  std::ofstream finalVEM;
  finalVEM.open ("finalMIP.txt");// std::ios_base::app);
  finalVEM << totalVEM << "\n";
  finalVEM.close();
}
