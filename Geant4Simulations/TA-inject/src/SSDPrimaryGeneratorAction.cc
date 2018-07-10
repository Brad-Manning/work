//Bradley Manning 3/2018
//SSDPrimaryGeneratorAction.cc
//Creates gun object
#include "SSDPrimaryGeneratorAction.hh"
#include "SSDAnalysis.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>


SSDPrimaryGeneratorAction::SSDPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), 
  fEnvelopeBox(0)
{
  G4int n_particle = 1;
  srand (time(NULL));
 
  
  
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="mu+");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(10.*MeV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
}


SSDPrimaryGeneratorAction::~SSDPrimaryGeneratorAction()
{
  delete fParticleGun;
  //delete[] energy;
  //delete[] particleName;
}


void SSDPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //--------------------------------------------------------------------------
  //Randomise Position
   // G4double width_box = 130; //cm
  //G4double length_box = 380; //cm

  G4float posy = (rand() % 15000)/100.; //random number between 0 and 130;
  G4float posx = (rand() % 20000)/100.; //random number between 0 and 380;
  //G4ThreeVector direction = particleGun->GetParticleMomentumDirection();
  //particleGun->SetParticleMomentumDirection(G4ThreeVector(direction.x(), direction.y(), direction.z()));
  //  G4double zenith = atan ( direction.y() / direction.z() );
  //G4double azimuthh = atan ( direction.x() / direction.y() );
					    



  
  fParticleGun->SetParticlePosition(G4ThreeVector(-100*cm+posx*cm,-75*cm+posy*cm,2*cm));
  fParticleGun->GeneratePrimaryVertex(anEvent);
  //If particles energy is 0, write VEM to separate file
  //I placed 0 energy particles with 0 weight to do this
  G4double energy;
  energy = fParticleGun->GetParticleEnergy();
  if (energy == 0 )
    {
      std::ifstream iVEM ("finalVEM.txt");
      std::ofstream oVEM;
      std::string line;
      oVEM.open("VEM.txt", std::ofstream::out | std::ofstream::app);
      if (iVEM.is_open())
	{
	while ( getline (iVEM, line) )
	  {
	    oVEM << line << "\n";
	  }
	}
      oVEM.close();
      iVEM.close();
    }
}  
