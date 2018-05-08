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
#include "CherenkovDetectorConstruction.hh"

#include <cmath>
#include <cstdlib>
#include <time.h>
#include <math.h>

G4double r_s;

 

CherenkovPrimaryGeneratorAction::CherenkovPrimaryGeneratorAction()
{
  //number of particles fired per /run/beamOn, ie. with n_particle = 5,
  ///run/beamOn 1 will fire 5 particles.
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  srand (time(NULL));
  G4cin >> r_s;
  //Create messenger
  gunMessenger = new CherenkovPrimaryGeneratorMessenger(this);

  // default values (can be changed in visualisation UI)
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("mu+");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleTime(0.0*ns);
  // particleGun->SetParticlePosition(G4ThreeVector(posx,posy,1.0*m));
  particleGun->SetParticlePosition(G4ThreeVector(r_s*m,0.*m,0.6*m));
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
  particleGun->SetParticleMomentumDirection(G4ThreeVector(direction.x(),direction.y(),-1.0*direction.z()));

  //G4double modP = sqrt(pow(direction.x(),2) + pow(direction.y(),2) + pow(direction.z(),2));
  //  G4ThreeVector unitP = G4ThreeVector(direction.x()/modP,direction.y()/modP,direction.z()/modP);
  //------------------------------------------------------------------------
  //Randomise position
  G4double delta = (rand() % 500)/10000.0;
  G4double random_no = (rand() % 1000)/1000.0;
  G4double r;
  if (random_no >= 0.5) {
    r = delta*r_s + r_s;
  } else {
    r = -delta*r_s + r_s;
  }
  //G4cout << delta << G4endl;
  //G4cout << r << G4endl;
  G4double phi = (rand() % 1500) / 10000.0;
  // G4cout << phi << G4endl;
  G4double posx = r*cos(phi-0.075);
  G4double posy = r*sin(phi-0.075);
  //G4cout << posx << " " << posy << G4endl;
  particleGun->SetParticlePosition(G4ThreeVector(posx*m,posy*m,0.6*m));





  //----------------------------------------------------------------------
  
  //Determine whether TOP of tank, or SIDE of tank
  //HARDCODED FOR VERTICAL SHOWER
  //100% CHANCE OF HITTING TOP OF TANK, 0 FOR SIDE OF TANK.
  //Variable is the CHANCE OF HITTING SIDE.
  // G4double variable = 0.0;
  // G4double chance = (rand() % 100)/100.0;
  // //  G4cout << "DASDAW "<< chance << G4endl;
  // if (chance >= variable) {
  // //Randomise position in top of tank
  // G4ThreeVector position;
  // G4double r = (rand() % 324)/100.0; 
  // G4double theta = (rand() % 36000)/100.0;
  // G4double posx = sqrt(r)*cos(theta*(3.1415926/180.0));
  // G4double posy = sqrt(r)*sin(theta*(3.1415926/180.0));
  // particleGun->SetParticlePosition(G4ThreeVector(posx*m,posy*m,0.6*m));
  // } else {
  //   G4ThreeVector position;
  //   G4double posz = (rand() % 1200)/1000.0;
  //   G4double posx = (rand() % 1800)/1000.0;
  //   //G4cout << "Z COORD" << posz << " X COORD " << posx << G4endl;
  
  //   double ang_theta = acos ( (posx) / 1.8 ) ;
    
  //   double posy = 1.8*sin (ang_theta);
    
  //   //G4cout << "THETA " << ang_theta << "Y COORD " << posy << G4endl;
  //   //Choose Quadrant, randomly::
  //   if (direction.x() >= 0 && direction.y() >= 0 ) {
  //     posy = -1*posy;
  //     posx = -1*posx;
  //   } else if (direction.x() >= 0 && direction.y() <= 0) {
  //     posx = -1*posx;
  //   } else if (direction.x() <= 0 && direction.y() >= 0) {
  //     posy = -1*posy;
  //   }
  //   particleGun->SetParticlePosition(G4ThreeVector(posx*m,posy*m,0.6*m-posz*m));
  // }
  
  // G4cout << posx << " " << posy << " " << r << " " << theta << G4endl;
  //TO MAKE GRAPH STUFF
  
  //Reassign for understanding
  //G4double mu = unitP.x();
  //G4double nu = unitP.y();
  //G4double omega = unitP.z();
  
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

    
