

#ifndef SSDRunAction_h
#define SSDRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Parameter.hh"
#include "globals.hh"

class G4Run;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class SSDRunAction : public G4UserRunAction
{
  public:
    SSDRunAction();
    virtual ~SSDRunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void AddEdep (G4double edep); 
    G4double GetWeight (G4int count);
  private:
    G4Parameter<G4double> fEdep;
    G4Parameter<G4double> fEdep2;
};

#endif

