//Bradley Manning 8/2017
//CherenkovStackingAction.cc
//This .cc is ran everytime a new track is detected


#include "CherenkovStackingAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"

#include "CherenkovAnalysis.hh"
#include "G4RunManager.hh"
#include <fstream>
#include "CherenkovRunAction.hh"
#include <vector>

G4double nParticles = 0;
CherenkovStackingAction::CherenkovStackingAction(CherenkovRunAction* runAction)
  : G4UserStackingAction(),
    fRunAction(runAction),
    gammaCounter(0),
    count(0.),
    totalPhotons(0.)
    
{}

CherenkovStackingAction::~CherenkovStackingAction()
{}

G4ClassificationOfNewTrack
CherenkovStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{

  
  if( aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {
      if( aTrack->GetParentID()>0)
	{ //ie. particle is a secondary
	  gammaCounter++;

	  // fKill option increases performance, but removes any possible
	  // interactions from Cherenkov Light. Fine for no PMTs 
	  return fKill;
	 
	}
    }
  return fUrgent;
}

void CherenkovStackingAction::NewStage()
{
  w = fRunAction->GetWeight();
  totalPhotons += (gammaCounter)/49450.0*w;///49450.0;

  nParticles += w;
  
  //G4cout << "VEM: " << totalPhotons << " Hits: "  << hits << " Hits w/ weight: " << hitsWeight <<  G4endl;
  std::ofstream finalVEM ("finalVEM.txt");
  std::ifstream runInfo ("runInfo.txt");
  G4int line_no = 0;
  G4double line;
  G4int tank_pos;
  while(runInfo >> line) {
    line_no++;
    //if (line_no == 3 ) { nParticles = line; }
    if (line_no == 1) tank_pos = line;
  }
  finalVEM << totalPhotons << " " << (int) nParticles << " " << tank_pos << "\n";
}

void CherenkovStackingAction::PrepareNewEvent()
{
  //Determine if the particle hit or not
  if (gammaCounter > 0)
    {
      hits += 1;
      hitsWeight += 1*w;
    }
  gammaCounter = 0;
}

