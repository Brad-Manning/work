
#ifndef SSDActionInitialization_h
#define SSDActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class SSDActionInitialization : public G4VUserActionInitialization
{
  public:
    SSDActionInitialization();
    virtual ~SSDActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
