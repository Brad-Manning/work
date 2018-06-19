//Bradley Manning

// simple radioactive decay

#ifndef simpleRadioactiveDecayDetectorConstruction_h
#define simpleRadioactiveDecayDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class simpleRadioactiveDecayDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  simpleRadioactiveDecayDetectorConstruction();
  ~simpleRadioactiveDecayDetectorConstruction();

public:
  G4VPhysicalVolume* Construct();

};

#endif
