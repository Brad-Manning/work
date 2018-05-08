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

std::vector < G4double > weight;
G4double w;
CherenkovStackingAction::CherenkovStackingAction()
  : G4UserStackingAction(),
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
	  // interactions from Cherenkov Light.
	   return fKill;
	 
	}
    }
  return fUrgent;
}

void CherenkovStackingAction::NewStage()
{
  //open manager instance for root
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  //Read in from weight file
  // G4cout << "HELP" << G4endl;
  //std::ifstream weightAll ("weightAll.txt");
 
  w = 1;
  //while (weightAll >> w)
  // {
  //   weight.push_back (w);
    
  // }
  //G4cout << "Weight vector created, last value is " << w << "\n";
 
  //G4cout << weight[count] << G4endl;
  //Uncomment weights when using weight files
  //if ( gammaCounter != 0) {
    man->FillH1(0,gammaCounter);//,weight[count]);
    man->FillH1(1,gammaCounter/49500.0);
    // }//,weight[count]); //VEM graph
  count++;
  // weight.clear();
  totalPhotons += (gammaCounter)/49500.0*(w);//[count]);///49450.0;

  // G4cout << "VEM: " << totalPhotons << " Hits: "  << hits << " Hits w/ weight: " << hitsWeight <<  G4endl;
  std::ofstream finalVEM ("finalVEM.txt");
  finalVEM << totalPhotons << "\n";
}

void CherenkovStackingAction::PrepareNewEvent()
{
  //Determine if the particle hit or not
  if (gammaCounter > 0)
    {
      hits += 1;
      hitsWeight += w;//weight[count];
    }
  gammaCounter = 0;
}

