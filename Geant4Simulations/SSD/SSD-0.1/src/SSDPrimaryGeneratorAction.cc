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
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1));
}


SSDPrimaryGeneratorAction::~SSDPrimaryGeneratorAction()
{
  delete fParticleGun;
  //delete[] energy;
  //delete[] particleName;
}


void SSDPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //Place particle randomly inside buffer centred at 0.
  G4double r = ((rand() % 2501)/100.0);
  G4double theta = ((rand() % 36000)/100.0);
  G4double x0 = sqrt(r)*cos(theta*(3.1415926/180.0));
  G4double y0 = sqrt(r)*sin(theta*(3.1415926/180.0));
  G4double z0 = -0.1;
  
  fParticleGun->SetParticlePosition(G4ThreeVector(-1*m,0*m,0*m));
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
