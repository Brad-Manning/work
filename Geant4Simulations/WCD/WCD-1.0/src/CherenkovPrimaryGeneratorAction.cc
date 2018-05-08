//Bradley Manning 8/2017
//CherenkovPrimaryGenerator.cc
//This file controls the gun
#include "CherenkovPrimaryGeneratorAction.hh"
#include "CherenkovPrimaryGeneratorMessenger.hh"

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>
#include <cstdlib>
#include <time.h>
CherenkovPrimaryGeneratorAction::CherenkovPrimaryGeneratorAction()
{
  //number of particles fired per /run/beamOn, ie. with n_particle = 5,
  ///run/beamOn 1 will fire 5 particles.
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  srand (time(NULL));
  //Create messenger
  gunMessenger = new CherenkovPrimaryGeneratorMessenger(this);

  // default values (can be changed in visualisation UI)
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("mu+");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleTime(0.0*ns);
  
  // particleGun->SetParticlePosition(G4ThreeVector(posx,posy,1.0*m));
  particleGun->SetParticlePosition(G4ThreeVector(0.*m,0.*m,1.0*m));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  particleGun->SetParticleEnergy(10*MeV);
}

CherenkovPrimaryGeneratorAction::~CherenkovPrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
}

void CherenkovPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //Randomise position in buffer circle
  G4ThreeVector position;
  G4double r = (rand() % 2500)/100.0; 
  G4double theta = (rand() % 36000)/100.0;
  G4double posx = sqrt(r)*cos(theta*(3.1415926/180.0));
  G4double posy = sqrt(r)*sin(theta*(3.1415926/180.0));
  // G4cout << posx << " " << posy << " " << r << " " << theta << G4endl;
  //TO MAKE GRAPH STUFF
  G4ThreeVector direction = particleGun->GetParticleMomentumDirection();
  //G4cout << direction << G4endl;
  G4double modP = sqrt(pow(direction.x(),2) + pow(direction.y(),2) + pow(direction.z(),2));
  G4ThreeVector unitP = G4ThreeVector(direction.x()/modP,direction.y()/modP,direction.z()/modP);

  //Reassign for understanding
  G4double mu = unitP.x();
  G4double nu = unitP.y();
  G4double omega = unitP.z();

  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  //0.6m is top of tank in GEANT4 world
  //comment this if not randomising position
  particleGun->SetParticlePosition(G4ThreeVector(posx*m,posy*m,0.6*m));
  particleGun->GeneratePrimaryVertex(anEvent);
}

void CherenkovPrimaryGeneratorAction::SetOptPhotonPolar()
{
  G4double angle = G4UniformRand() * 360.0*deg;
  SetOptPhotonPolar(angle);
}

void CherenkovPrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
{
  //Only necessary if firing optical photons from gun
  if (particleGun->GetParticleDefinition()->GetParticleName() != "opticalphoton" )
    {
      G4cout << "the particle is not an opticalphoton" << G4endl;
      return;
    }
  G4ThreeVector normal (1., 0., 0.);
  G4ThreeVector kphoton = particleGun->GetParticleMomentumDirection();
  G4ThreeVector product = normal.cross(kphoton);
  G4double modul2 = product*product;

  G4ThreeVector e_perpend (0., 0., 1.);
  if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
  G4ThreeVector e_parallel = e_perpend.cross(kphoton);
  G4ThreeVector polar = std::cos(angle)*e_parallel + std::sin(angle)*e_perpend;
  particleGun->SetParticlePolarization(polar);
}

    
