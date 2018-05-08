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

G4double r_s;

SSDPrimaryGeneratorAction::SSDPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), 
  fEnvelopeBox(0)
{
  G4int n_particle = 1;
  srand (time(NULL));
  G4cin >> r_s;
  
  
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
  G4double delta = (rand() % 500)/10000.0;
  G4double random_no = (rand() % 1000)/1000.0;
  G4double r;
  if (random_no >=0.5) {
    r = delta*r_s + r_s;
  } else {
    r = -delta*r_s + r_s;
  }

  G4double phi = (rand() % 1500) / 10000.0;
  G4double posx = r*cos(phi-0.075);
  G4double posy = r*sin(phi-0.075);
    
  fParticleGun->SetParticlePosition(G4ThreeVector(posx*m,posy*m,-6*cm));
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
