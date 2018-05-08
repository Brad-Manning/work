// Bradley Manning 3/2018
// SSDDetectorConstruction.hh

#ifndef SSDDetectorConstruction_h
#define SSDDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class SSDDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  SSDDetectorConstruction();
  virtual ~SSDDetectorConstruction();
  
  virtual G4VPhysicalVolume* Construct();

  G4LogicalVolume* GetScoringVolume(int a) const {
    if (a == 1)
      return fScoringVolume;
    else
      return fScoringVolume2;
  }
protected:
  G4LogicalVolume* fScoringVolume;
  G4LogicalVolume* fScoringVolume2;
private:
  // Variables used in .cc
  G4double scintillatorBar_height;
  G4double scintillatorBar_width;
  G4double scintillatorBar_length;

};

#endif /*SSDDetectorConstruction.hh*/
