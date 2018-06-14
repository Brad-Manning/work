

#ifndef SSDRunAction_h
#define SSDRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"
#include <vector>

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
    G4double GetWeight ();
  private:
    G4Accumulable<G4double> fEdep;
    G4Accumulable<G4double> fEdep2;
    std::vector<G4double> fWeight;
    bool fUseWeights;
    G4int fIdx = 0;
};

#endif

