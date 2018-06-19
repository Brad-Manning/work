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
#include <math.h>
CherenkovPrimaryGeneratorAction::CherenkovPrimaryGeneratorAction()
{
  //number of particles fired per /run/beamOn, ie. with n_particle = 5,
  ///run/beamOn 1 will fire 5 particles.
  G4int n_particle = 1;
  //G4cout << "HELP" << G4endl;
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
  //As of V2.0 buffer circle is being removed, and a two step solution
  //involving the top of the tank, and side of the tank is being implemented.

  G4ThreeVector direction = particleGun->GetParticleMomentumDirection();
  particleGun->SetParticleMomentumDirection(G4ThreeVector(direction.x(),direction.y(),direction.z()));
  //G4cout << direction << G4endl;
  //G4double modP = sqrt(pow(direction.x(),2) + pow(direction.y(),2) + pow(direction.z(),2));
  //  G4ThreeVector unitP = G4ThreeVector(direction.x()/modP,direction.y()/modP,direction.z()/modP);
  G4double zenith = atan ( direction.y() / direction.z() ) ;
  G4double azimuth = atan ( direction.x() / direction.y() ) ;
  // G4cout << "Azimuth" << azimuth << G4endl;
  G4double area_top_tank = M_PI * pow(1.8,2) * cos( zenith ) ;
  G4double area_side_tank = 1.8 * 2 * 1.2 * sin ( zenith );
  //Fraction = percentage of particles hitting the side of the tank
  G4double fraction = fabs(area_side_tank / area_top_tank);

  G4double chance = (rand() % 100)/100.0;

  if (chance >= fraction) {

    //Randomise position in top of tank
  G4ThreeVector position;
  G4double r = (rand() % 324)/100.0; 
  G4double theta = (rand() % 36000)/100.0;
  G4double posx = sqrt(r)*cos(theta*(3.1415926/180.0));
  G4double posy = sqrt(r)*sin(theta*(3.1415926/180.0));
  particleGun->SetParticlePosition(G4ThreeVector(posx*m,posy*m,1.0*m));
  } else {
    
    G4ThreeVector position;
    //Since the tank is symmetrical we can rotate the particles generating on the side of the tank to face inwards
    if ( direction.x() < 0 && direction.y() < 0 ) {
      if ( azimuth > 0.017 ) {
	G4double newDirectionx = direction.x()*cos((azimuth)) - direction.y()*sin((azimuth));
	G4double newDirectiony = direction.x()*sin((azimuth)) + direction.y()*cos((azimuth));
	particleGun->SetParticleMomentumDirection(G4ThreeVector(newDirectionx,newDirectiony,direction.z()));
	//	G4cout << tan ( newDirectiony / newDirectionx ) << G4endl;
      }
    }
    if ( direction.x() < 0 && direction.y() > 0 ) {
      if ( azimuth > 0.017 || azimuth < 0.017){
	G4double newDirectionx = direction.x()*cos(M_PI+azimuth) - direction.y()*sin(M_PI+azimuth);
	G4double newDirectiony = direction.x()*sin(M_PI+azimuth) + direction.y()*cos(M_PI+azimuth);
	particleGun->SetParticleMomentumDirection(G4ThreeVector(newDirectionx,newDirectiony,direction.z()));
	//	G4cout << tan ( newDirectiony / newDirectionx ) << G4endl;
      }
    }
    if ( direction.x() > 0 && direction.y() > 0 ) {
      if ( azimuth > 0.017 ) {
	G4double newDirectionx = direction.x()*cos(M_PI+azimuth) - direction.y()*sin(M_PI+azimuth);
	G4double newDirectiony = direction.x()*sin(M_PI+azimuth) + direction.y()*cos(M_PI+azimuth);
	particleGun->SetParticleMomentumDirection(G4ThreeVector(newDirectionx,newDirectiony,direction.z()));
	//	G4cout << tan ( newDirectiony / newDirectionx ) << G4endl;
      }
    }
    if ( direction.x() > 0 && direction.y() < 0) {
      if ( azimuth > 0.017 || azimuth < 0.017) {
	G4double newDirectionx = direction.x()*cos(2*M_PI+azimuth) - direction.y()*sin(2*M_PI+azimuth);
	G4double newDirectiony = direction.x()*sin(2*M_PI+azimuth) + direction.y()*cos(2*M_PI+azimuth);
	particleGun->SetParticleMomentumDirection(G4ThreeVector(newDirectionx,newDirectiony,direction.z()));
	//	G4cout << tan ( newDirectiony / newDirectionx ) << G4endl;
      }
    }
    G4double posz = (rand() % 1200)/1000.0;
    G4double posx = (rand() % 1800)/1000.0;
    
    double ang_theta = acos ( (posx) / 1.8 ) ;
    double posy = 1.8*sin (ang_theta);
    G4double neg = (rand() % 100)/100.0;
    if (neg >= 0.5) {
       posx = -1*posx;
      }
    particleGun->SetParticlePosition(G4ThreeVector(posx*m,posy*m,1.0*m-posz*m));
  }
  particleGun->GeneratePrimaryVertex(anEvent);
  particleGun->SetParticleMomentumDirection(direction);


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

    
