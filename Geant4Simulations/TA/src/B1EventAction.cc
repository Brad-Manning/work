// Bradley Manning 5/2017
// TAEventAction.cc

#include "TAEventAction.hh"
#include "TARunAction.hh"
#include "TAAnalysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"


#include <fstream>

G4double totalVEM;

TAEventAction::TAEventAction(TARunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.),
  fEdep2(0.),
  count(0.)
{} 

TAEventAction::~TAEventAction()
{}

void TAEventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
  fEdep2 = 0.;
}


void TAEventAction::EndOfEventAction(const G4Event*)
{
  G4double weight;
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  fRunAction->AddEdep(fEdep2);
  //Get root manager
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  weight = fRunAction->GetWeight(count);
  count++;
  G4cout << count << G4endl;
  // MAGIC NUMBER FOR VEM FROM 100K RUNS OF VERTICAL MUON.
  man->FillH1(2,fEdep/4.822, weight);
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
  finalVEM.open ("finalVEM.txt");
  finalVEM << totalVEM/4.822 << "\n";
  finalVEM.close();
}
