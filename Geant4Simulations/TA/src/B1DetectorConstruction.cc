// Bradley Manning 5/2017
// TADetectorConstruction.cc
// Constructs objects and defines world

#include "TADetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

TADetectorConstruction::TADetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0),
  fScoringVolume2(0)
{ }

TADetectorConstruction::~TADetectorConstruction()
{ }

G4VPhysicalVolume* TADetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  G4double env_sizeXY = 180*cm, env_sizeZ = 90*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");
   
  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;
  
  // World
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.8*world_sizeXY, 0.8*world_sizeXY, 0.8*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                          
  // Envelope 
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  //Get Materials from G4 Database
  G4Material* SS_mat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4Material* scin_mat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  //Create objects
  
  // Rectangular shape SSTOP
  G4double SStop_dx = 200*cm;// shape2_dxb = 12*cm;
  G4double SStop_dy = 150*cm;// shape2_dyb = 16*cm;
  G4double SStop_dz  = 1.5*mm;
  G4ThreeVector pos_SStop = G4ThreeVector(0, 0*cm, 0.075*cm);
  G4Box* solid_SStop =    
    new G4Box("SStop",                      //its name
              0.5*SStop_dx, 0.5*SStop_dy, 0.5*SStop_dz); //its size
                
  G4LogicalVolume* logic_SStop =                         
    new G4LogicalVolume(solid_SStop,         //its solid
                        SS_mat,          //its material
                        "SStop");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    pos_SStop,                    //at position
                    logic_SStop,             //its logical volume
                    "SStop",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


  G4double SSmid_dz = 1*mm;
  G4ThreeVector pos_SSmid = G4ThreeVector(0, 0*cm, 1.40*cm);
  G4Box* solid_SSmid =    
    new G4Box("SSmid",                      //its name
              0.5*SStop_dx, 0.5*SStop_dy, 0.5*SSmid_dz); //its size
  
  G4LogicalVolume* logic_SSmid =                         
    new G4LogicalVolume(solid_SSmid,         //its solid
                        SS_mat,          //its material
                        "SSmid");           //its name
  
  new G4PVPlacement(0,                       //no rotation
                    pos_SSmid,                    //at position
                    logic_SSmid,             //its logical volume
                    "SSmid",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  G4double SSbot_dz = 1.2*mm;
  G4ThreeVector pos_SSbot = G4ThreeVector(0, 0*cm, 2.71*cm);
  G4Box* solid_SSbot =    
    new G4Box("SSbot",                      //its name
              0.5*SStop_dx, 0.5*SStop_dy, 0.5*SSbot_dz); //its size
  
  G4LogicalVolume* logic_SSbot =                         
    new G4LogicalVolume(solid_SSbot,         //its solid
                        SS_mat,          //its material
                        "SSbot");           //its name
  
  new G4PVPlacement(0,                       //no rotation
                    pos_SSbot,                    //at position
                    logic_SSbot,             //its logical volume
                    "SSbot",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  G4double Scin1_dz = 1.2*cm;
  G4ThreeVector pos_Scin1 = G4ThreeVector(0, 0*cm, 0.75*cm);
  G4Box* solid_Scin1 =    
    new G4Box("Scin1",                      //its name
              0.5*SStop_dx, 0.5*SStop_dy, 0.5*Scin1_dz); //its size
  
  G4LogicalVolume* logic_Scin1 =                         
    new G4LogicalVolume(solid_Scin1,         //its solid
                        scin_mat,          //its material
                        "Scin1");           //its name
  
  new G4PVPlacement(0,                       //no rotation
                    pos_Scin1,                    //at position
                    logic_Scin1,             //its logical volume
                    "Scin1",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  G4double Scin2_dz = 1.2*cm;
  G4ThreeVector pos_Scin2 = G4ThreeVector(0, 0*cm, 2.05*cm);
  G4Box* solid_Scin2 =    
    new G4Box("Scin2",                      //its name
              0.5*SStop_dx, 0.5*SStop_dy, 0.5*Scin2_dz); //its size
  
  G4LogicalVolume* logic_Scin2 =                         
    new G4LogicalVolume(solid_Scin2,         //its solid
                        scin_mat,          //its material
                        "Scin2");           //its name
  
  new G4PVPlacement(0,                       //no rotation
                    pos_Scin2,                    //at position
                    logic_Scin2,             //its logical volume
                    "Scin2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
  // Set Shape2 as scoring volume
  fScoringVolume = logic_Scin1;
  fScoringVolume2 = logic_Scin2;
  
  //always return the physical World
  return physWorld;
}
