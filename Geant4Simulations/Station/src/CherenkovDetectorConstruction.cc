//Bradley Manning 8/2017
//CherenkovDetectorConstruction.cc
//This .cc constructs the 'world' and its objects

#include "CherenkovDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"


CherenkovDetectorConstruction::CherenkovDetectorConstruction()
{
  //Define dimensions of tank
  //From Pierre Auger NIM2015 pg 175
  //1.2m height, 1.8m radius
  scintillatorBar_height = 1*cm;
  scintillatorBar_width = 10*cm;
  scintillatorBar_length = 160*cm;
  tank_innerRadius = 1.8*m;
  tank_height = 1.2*m;
  tank_outerRadius = 1.9*m;
  tank_startingAngle = 0.*deg;
  tank_spanningAngle = 360.*deg;
  world_innerRadius = 0.*m;
  world_outerRadius = 5.0*m;
  world_z = 1.2*m;
  world_startingAngle = 0.*deg;
  world_spanningAngle = 360.*deg;
}


CherenkovDetectorConstruction::~CherenkovDetectorConstruction(){;}

G4VPhysicalVolume* CherenkovDetectorConstruction::Construct()
{
  bool checkOverlaps = 1;

  //Materials
  G4double a, z, density;
  G4int nelements;

  G4NistManager* nist = G4NistManager::Instance();
 
  // Generate and Add Material Properties Table

  const G4int nEntries = 32;
  // Energies correspond to 300-600 nm
  G4double PhotonEnergy[nEntries] =
   { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
     2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
     2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
     2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
     2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
     3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
     3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
     3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };

  // Use inbuilt G4Materials
  G4Material* tank_mat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material* Water = nist->FindOrBuildMaterial("G4_WATER");
  G4Material* Air = nist->FindOrBuildMaterial("G4_AIR");

  G4double RefractiveIndex1[nEntries] =
            { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
              1.346,  1.3465, 1.347,  1.3475, 1.348,
              1.3485, 1.3492, 1.35,   1.3505, 1.351,
              1.3518, 1.3522, 1.3530, 1.3535, 1.354,
              1.3545, 1.355,  1.3555, 1.356,  1.3568,
              1.3572, 1.358,  1.3585, 1.359,  1.3595,
              1.36,   1.3608};

  G4double Absorption1[nEntries] =
           {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
           15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
           45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
           52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
           30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
           17.500*m, 14.500*m };


  G4MaterialPropertiesTable* water_MPT = new G4MaterialPropertiesTable();
  water_MPT->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex1,nEntries);
  water_MPT->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption1,     nEntries);
  Water->SetMaterialPropertiesTable(water_MPT);


  //Construct objects
  //---------------------------------------------------------------------------------
  // The 'World'

  G4Box* world = new G4Box("world", 2*m, 2*m, 1*m);
  // G4Tubs* world = new G4Tubs("world", world_innerRadius, world_outerRadius, world_z/2.0, world_startingAngle, world_spanningAngle);
  
  G4LogicalVolume* world_log = new G4LogicalVolume(world, Air, "world", 0,0,0);

  G4VPhysicalVolume* world_phys = new G4PVPlacement(0,G4ThreeVector(),world_log, "world", 0,false,0);

  // The water tank
  //Polyethylene container

  G4Tubs* tank = new G4Tubs("tank", tank_innerRadius, tank_outerRadius, tank_height/2.0,tank_startingAngle, tank_spanningAngle);
  G4LogicalVolume* tank_log = new G4LogicalVolume(tank, tank_mat, "tank");
  G4VisAttributes * tank_visAtt = new G4VisAttributes(G4Colour(1.,1.,1.));
  //tank_visAtt->SetForceWireframe(true);
  tank_log->SetVisAttributes(tank_visAtt);

  G4VPhysicalVolume* tank_phys = new G4PVPlacement(0,G4ThreeVector(), tank_log, "tank", world_log, false, 0);

  //Water inside tank

  tank_innerRadius = 0.*m;
  tank_outerRadius = 1.8*m;

  G4Tubs* watertank = new G4Tubs("watertank", tank_innerRadius, tank_outerRadius, tank_height/2.0, tank_startingAngle, tank_spanningAngle);

  G4LogicalVolume* watertank_log = new G4LogicalVolume(watertank,Water, "watertank");
  G4VisAttributes * watertank_visAtt = new G4VisAttributes(G4Colour(0.,1.,1.));

  watertank_log->SetVisAttributes(watertank_visAtt);

  G4VPhysicalVolume* watertank_phys = new G4PVPlacement(0,G4ThreeVector(), watertank_log, "watertank", world_log, false, 0);

  // Surfaces
  // Plane to generate particles randomly on: - Aid for visualisation
  // G4Box* plane = new G4Box("plane", 1.8*m, 0.01*m, 0.6*m);
  //G4LogicalVolume* plane_log = new G4LogicalVolume(plane, Air, "plane");
  //G4VPhysicalVolume* plane_phys = new G4PVPlacement(0,G4ThreeVector(0,1.8*m,0),plane_log, "plane", world_log, false, 0);
  //G4VPhysicalVolume* plane_phys2 = new G4PVPlacement(0,G4ThreeVector(0,-1.8*m,0),plane_log, "plane2", world_log, false, 0);

  //  G4RotationMatrix* myRotation = new G4RotationMatrix();
  //myRotation->rotateX(0.*deg);
  //myRotation->rotateY(0.*deg);
  //myRotation->rotateZ(90.*deg);
  
  //G4VPhysicalVolume* plane_phys3 = new G4PVPlacement(myRotation,G4ThreeVector(1.8*m,0,0),plane_log, "plane3", world_log, false, 0);
  //G4VPhysicalVolume* plane_phys4 = new G4PVPlacement(myRotation,G4ThreeVector(-1.8*m,0,0),plane_log, "plane4", world_log, false, 0);
  
  // water tank

  G4OpticalSurface* OpWaterSurface = new G4OpticalSurface("WaterSurface");
  OpWaterSurface->SetType(dielectric_dielectric);
  OpWaterSurface->SetFinish(ground);
  OpWaterSurface->SetModel(unified);

  G4LogicalBorderSurface* WaterSurface = new G4LogicalBorderSurface("WaterSurface", watertank_phys, world_phys, OpWaterSurface);

  G4OpticalSurface* OpTankSurface = new G4OpticalSurface("TankSurface");
  OpTankSurface->SetType(dielectric_dielectric);
  OpTankSurface->SetFinish(ground);
  OpTankSurface->SetModel(unified);

  G4LogicalBorderSurface* TankSurface = new G4LogicalBorderSurface("TankSurface", tank_phys, world_phys, OpTankSurface);

  //
  // Generate & Add Material Properties Table attached to the optical surfaces
  //
  const G4int num = 2;
  G4double Ephoton[num] = {2.038*eV, 4.144*eV};

  //OpticalWaterSurface 
  G4double RefractiveIndex[num] = {1.35, 1.40};
  G4double SpecularLobe[num]    = {0.3, 0.3};
  G4double SpecularSpike[num]   = {0.2, 0.2};
  G4double Backscatter[num]     = {0.2, 0.2};

  G4MaterialPropertiesTable* tank_MPT = tank_mat->GetMaterialPropertiesTable();

  water_MPT->AddProperty("RINDEX",                Ephoton, RefractiveIndex, num);
  water_MPT->AddProperty("SPECULARLOBECONSTANT",  Ephoton, SpecularLobe,    num);
  water_MPT->AddProperty("SPECULARSPIKECONSTANT", Ephoton, SpecularSpike,   num);
  water_MPT->AddProperty("BACKSCATTERCONSTANT",   Ephoton, Backscatter,     num);

  OpWaterSurface->SetMaterialPropertiesTable(water_MPT);
  OpTankSurface->SetMaterialPropertiesTable(tank_MPT);


  //Construct Scintillator -------------

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
  
  new G4PVPlacement(0, G4ThreeVector(0,0*cm,1.27*m+0.5*cm),logic_vessel_cover,"vessel_cover",world_log,false,0,checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,61*cm,1.27*m+1.5*cm), logic_vessel_horizontal_strip, "vessel_horizontal_strip_1", world_log, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,-61*cm,1.27*m+1.5*cm),logic_vessel_horizontal_strip, "vessel_horizontal_strip_2", world_log, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0+186*cm,0*cm,1.27*m+1.5*cm), logic_vessel_vertical_strip, "vessel_vertical_strip_1",world_log, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0-186*cm,0*cm,1.27*m+1.5*cm), logic_vessel_vertical_strip, "vessel_vertical_strip_2", world_log, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0,0*cm,1.27*m+3.0*cm), logic_vessel_bottom, "vessel_bottom", world_log, false, 0, checkOverlaps);

  new G4PVPlacement(0, G4ThreeVector(0,0*cm,1.27*m-0.5*mm), logic_aluminum_cover, "aluminum_cover", world_log, false, 0, checkOverlaps);

  new G4PVPlacement(0, G4ThreeVector(0,0*cm,1.27*m-10.5*mm), logic_aluminum_cover, "aluminum_cover2", world_log, false, 0, checkOverlaps);
  
  new G4PVPlacement(0, G4ThreeVector(0,0*cm,1.27*m+40.5*mm), logic_aluminum_bottom, "aluminum_bottom", world_log, false, 0, checkOverlaps);

  new G4PVPlacement(0, G4ThreeVector(0,0*cm,1.27*m+50.5*mm), logic_aluminum_bottom, "aluminum_bottom2", world_log, false, 0, checkOverlaps);
  
  
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

  new G4PVPlacement(0, G4ThreeVector(0+105*cm,0*cm,1.27*m+1.5*cm), logic_scintillator_left_module, "scintillator_left_module", world_log, false, 0, checkOverlaps);
  new G4PVPlacement(0, G4ThreeVector(0-105*cm, 0*cm,1.27*m+1.5*cm), logic_scintillator_right_module,  "scintillator_right_module", world_log, false, 0, checkOverlaps);

  fScoringVolume = logic_scintillator_left_module;
  fScoringVolume2 = logic_scintillator_right_module;
  
  
  return world_phys;
  
}

  
