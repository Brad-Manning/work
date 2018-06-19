// Bradley Manning

// Simple Radioactive Decay

#ifndef simpleRadioactiveDecayRunAction_h
#define simpleRadioactiveDecayRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class simpleRadioactiveDecayRun;
class simpleRadioactiveDecayDetectorConstruction;
class simpleRadioactiveDecayPrimaryGeneratorAction;

class simpleRadioactiveDecayRunAction : public G4UserRunAction
{
public:
  simpleRadioactiveDecayRunAction(simpleRadioactiveDecayDetectorConstruction*, simpleRadioactiveDecayPrimaryGeneratorAction*)
    ~simpleRadioactiveDecayRunAction();

public:
  virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

private:
  simpleRadioactiveDecayDetectorConstruction* fDetector;
  simpleRadioactiveDecayPrimaryGeneratorAction* fPrimary;
  simpleRadioactiveDecayRun* fRun;

};

#endif
