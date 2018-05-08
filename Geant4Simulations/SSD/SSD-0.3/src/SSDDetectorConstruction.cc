// Bradley Manning 3/2018
// SSDDetectorConstruction.cc

#include "SSDDetectorConstruction.hh"
#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"





SSDDetectorConstruction::SSDDetectorConstruction()
{
  //Detector variables
  //All variables taken from: https://arxiv.org/abs/1604.03637; section 4.2
  //The scintillator consists of two modules. 160cm long scintillator bars
  //10 cm long strips, 1 cm high
  scintillatorBar_height = 1*cm;
  scintillatorBar_width = 10*cm;
  scintillatorBar_length = 160*cm;
  delta = 0.1;
  G4cout << "CHOOSE TANK POS (m)" << G4endl;
  G4cin >> r_s;
  r_s = r_s*m;
  world_innerRadius = (r_s-delta*r_s);
  world_outerRadius = (r_s+delta*r_s);
  world_z = 50*cm;
  world_startingAngle = -0.5*8.594367*deg;
  world_spanningAngle = 1.*8.594367*deg;
  
  // Each module consists of 12 bars.
  
}

SSDDetectorConstruction::~SSDDetectorConstruction()
{
  ;
}

G4VPhysicalVolume* SSDDetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();

  //Envelope
  G4double env_x = 10*scintillatorBar_length;
  G4double env_y = 10*scintillatorBar_width;
  G4double env_z = 10*scintillatorBar_height;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

  //Option to switch overlap checking on/off
  G4bool checkOverlaps = true;
  
  // World ----------------------------------------------------------------------------------
  // G4double world_x = 1.2*env_x;
  // G4double world_y = 1.2*env_y;
  // G4double world_z = 1.2*env_z;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Tubs* world = new G4Tubs("world", world_innerRadius, world_outerRadius, world_z/2.0, world_startingAngle, world_spanningAngle);

  G4LogicalVolume* logicWorld = new G4LogicalVolume(world, world_mat, "world",0,0,0);
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "world", 0, false, checkOverlaps);

   //Get Materials from NIST Database
  G4Material* polystyrene = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
  G4Material* plastic = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  G4Material* aluminum = nist->FindOrBuildMaterial("G4_Al");


  //Polystyrene foam vessel
  //124 cm by 187 cm (one module)
  //  vessel_width = 124*cm;
  //  vessel_length = 187*cm;
  //  vessel_height = 4*cm;

  //Construction of VESSEL-------------------See fig 4.11------------------------------------------
  //Need to be half lengths.
  G4Box* vessel_cover = new G4Box("vessel_cover",187*2*0.5*cm,124*0.5*cm,1*0.5*cm);
  G4Box* vessel_horizontal_strip = new G4Box("vessel_horizontal_strip",187*2*0.5*cm,2*0.5*cm,1*0.5*cm);
  G4Box* vessel_vertical_strip = new G4Box("vessel_vertical_strip",2*0.5*cm,122*0.5*cm,1*0.5*cm);
  G4Box* vessel_bottom = new G4Box("vessel_bottom", 187*2*0.5*cm,124*0.5*cm,2*0.5*cm);
  G4Box* aluminum_cover = new G4Box("aluminum_cover", 187*2*0.5*cm, 124*0.5*cm,0.1*0.5*cm);
  G4Box* aluminum_bottom = new G4Box("aluminum_bottom", 187*2*0.5*cm, 124*0.5*cm,0.1*0.5*cm);
  
  
  G4LogicalVolume* logic_vessel_cover = new G4LogicalVolume(vessel_cover, polystyrene,"vessel_cover");
  G4LogicalVolume* logic_vessel_horizontal_strip =new G4LogicalVolume(vessel_horizontal_strip, polystyrene, "vessel_horizontal_strip");
  G4LogicalVolume* logic_vessel_vertical_strip = new G4LogicalVolume(vessel_vertical_strip, polystyrene, "vessel_vertical_strip");
  G4LogicalVolume* logic_vessel_bottom = new G4LogicalVolume(vessel_bottom, polystyrene, "vessel_bottom");
  G4LogicalVolume* logic_aluminum_cover = new G4LogicalVolume(aluminum_cover, aluminum, "aluminum_cover");
  G4LogicalVolume* logic_aluminum_bottom = new G4LogicalVolume(aluminum_bottom, aluminum, "aluminum_bottom");
  
  new G4PVPlacement(0, G4ThreeVector(r_s,0*cm,0.5*cm),logic_vessel_cover,"vessel_cover",logicWorld,false,0,checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(r_s,61*cm,1.5*cm), logic_vessel_horizontal_strip, "vessel_horizontal_strip_1", logicWorld, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(r_s,-61*cm,1.5*cm),logic_vessel_horizontal_strip, "vessel_horizontal_strip_2", logicWorld, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(r_s+186*cm,0*cm,1.5*cm), logic_vessel_vertical_strip, "vessel_vertical_strip_1",logicWorld, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(r_s-186*cm,0*cm,1.5*cm), logic_vessel_vertical_strip, "vessel_vertical_strip_2", logicWorld, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(r_s,0*cm,3.0*cm), logic_vessel_bottom, "vessel_bottom", logicWorld, false, 0, checkOverlaps);

  new G4PVPlacement(0, G4ThreeVector(r_s,0*cm,-0.5*mm), logic_aluminum_cover, "aluminum_cover", logicWorld, false, 0, checkOverlaps);

  new G4PVPlacement(0, G4ThreeVector(r_s,0*cm,-10.5*mm), logic_aluminum_cover, "aluminum_cover2", logicWorld, false, 0, checkOverlaps);
  
  new G4PVPlacement(0, G4ThreeVector(r_s,0*cm,40.5*mm), logic_aluminum_bottom, "aluminum_bottom", logicWorld, false, 0, checkOverlaps);

  new G4PVPlacement(0, G4ThreeVector(r_s,0*cm, 50.5*mm), logic_aluminum_bottom, "aluminum_bottom2", logicWorld, false, 0, checkOverlaps);
  
  
  //-----------------------------------------------------------------------------------------------------------

  //Construction of SCINTILLATOR-------------------------------------------------------------------------
  // scintillatorBar_height = 1*cm;
  // scintillatorBar_width = 10*cm;
  // scintillatorBar_length = 160*cm;
  // Each module consists of 12 bars.
  G4Box* scintillator_left_module = new G4Box("scintillator_left_module", 160*0.5*cm, 12*10*0.5*cm,1*0.5*cm);
  G4Box* scintillator_right_module = new G4Box("scintillator_right_module",160*0.5*cm,12*10*0.5*cm,1*0.5*cm);

  G4LogicalVolume* logic_scintillator_left_module = new G4LogicalVolume(scintillator_left_module, plastic, "scintillator_left_module");
  G4LogicalVolume* logic_scintillator_right_module = new G4LogicalVolume(scintillator_right_module, plastic, "scintillator_right_module");

  new G4PVPlacement(0, G4ThreeVector(r_s+105*cm,0*cm,1.5*cm), logic_scintillator_left_module, "scintillator_left_module", logicWorld, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(r_s-105*cm, 0*cm,1.5*cm), logic_scintillator_right_module,  "scintillator_right_module", logicWorld, false, 0, checkOverlaps);

  fScoringVolume = logic_scintillator_left_module;
  fScoringVolume2 = logic_scintillator_right_module;
  
  return physWorld;
}

  
