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

G4double totalMIP;
G4double nParticlesSSD = 0;
CherenkovEventAction::CherenkovEventAction(CherenkovRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.),
  fEdep2(0.),
  fgammaCounter(0.)
{}

CherenkovEventAction::~CherenkovEventAction()
{}

void CherenkovEventAction::BeginOfEventAction(const G4Event*)
{
  fgammaCounter = 0.;
  fEdep = 0.;
  fEdep2 = 0.;
}


void CherenkovEventAction::EndOfEventAction(const G4Event*)
{  
  G4double weight;
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  fRunAction->AddEdep(fEdep2);

  weight = fRunAction->GetWeight();
  nParticlesSSD += weight;

  totalMIP += fEdep*weight;

  std::ofstream finalMIP;
  std::ifstream runInfo ("runInfo.txt");
  G4int tank_pos;
  G4int line_no = 0;
  G4double line;
  while(runInfo >> line)
    {
      line_no++;
      if (line_no == 1)
	{
	  tank_pos = line;
	}
    }
   
  finalMIP.open ("finalMIP.txt");
  finalMIP << totalMIP/2. << " " << (int) nParticlesSSD << " " << tank_pos <<  "\n";
  finalMIP.close();
  
}
