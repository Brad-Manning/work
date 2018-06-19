// Bradley Manning

// Simple Radioactive Decay


// Includes --
// New includes
#include "simpleRadioactiveDecayDetectorConstruction.hh"

// G4 includes

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UnitsTable.hh"

// initialise
simpleRadioactiveDecayDetectorConstruction::simpleRadioactiveDecayDetectorConstruction()
  :G4VUserDetectorConstruction()
{}

// destructor
simpleRadioactiveDecayDetectorConstruction::~simpleRadioactiveDecayDetectorConstruction()
{}

G4VPhysicalVolume* simpleRadioactiveDecayDetectorConstruction::Construct()
{
}
