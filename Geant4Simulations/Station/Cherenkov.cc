
//
// Description: Test of Continuous Process G4Cerenkov
//              and RestDiscrete Process G4Scintillation
//              -- Generation Cerenkov Photons --
//              -- Generation Scintillation Photons --
//              -- Transport of optical Photons --


#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "G4ios.hh"

#include "CherenkovDetectorConstruction.hh"
#include "CherenkovPhysicsList.hh"
#include "CherenkovPrimaryGeneratorAction.hh"
#include "CherenkovRunAction.hh"
#include "CherenkovEventAction.hh"
#include "CherenkovStackingAction.hh"
#include "CherenkovSteppingVerbose.hh"
#include "G4SystemOfUnits.hh"
#include "StationSteppingAction.hh"
#include "Randomize.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }


  // Seed the random number generator manually
  //
  G4long myseed = 345354;
  CLHEP::HepRandom::setTheSeed(myseed);
  
  // User Verbose output class
  //
  G4VSteppingVerbose* verbosity = new CherenkovSteppingVerbose;
  G4VSteppingVerbose::SetInstance(verbosity);
  
  // Run manager
  //
  G4RunManager* runManager = new G4RunManager;

  // UserInitialization classes - mandatory
  //
  G4VUserDetectorConstruction* detector = new CherenkovDetectorConstruction;
  runManager-> SetUserInitialization(detector);
  //
  G4VUserPhysicsList* physics = new CherenkovPhysicsList;
  runManager-> SetUserInitialization(physics);
  
 // #ifdef G4VIS_USE
 //  // visualization manager
 //  //
 //  G4VisManager* visManager = new G4VisExecutive;
 //  visManager->Initialize();
 //  #endif

  // UserAction classes
  //
  CherenkovRunAction* run_action = new CherenkovRunAction;
  runManager->SetUserAction(run_action);
  //
  G4VUserPrimaryGeneratorAction* gen_action = new CherenkovPrimaryGeneratorAction;
  runManager->SetUserAction(gen_action);
  //
  //G4UserEventAction* event_action = new CherenkovEventAction(run_action);
  //runManager->SetUserAction(event_action);

  CherenkovEventAction* event_action = new CherenkovEventAction(run_action);
  runManager->SetUserAction(event_action);
  //  G4UserSteppingAction* stepping_action = new StationSteppingAction(run_action);
  runManager->SetUserAction(new StationSteppingAction(event_action));
  
  G4UserStackingAction* stacking_action = new CherenkovStackingAction(run_action);
  runManager->SetUserAction(stacking_action);
  
//   // Initialize G4 kernel
//   //
//   runManager->Initialize();
    
//   // Get the pointer to the User Interface manager
//   //
//   G4UImanager* UI = G4UImanager::GetUIpointer(); 
   
//   if (argc==1)   // Define UI session for interactive mode
//     {
//       G4UIsession* session = 0;
// #ifdef G4UI_USE_TCSH
//       session = new G4UIterminal(new G4UItcsh);      
// #else
//       session = new G4UIterminal();
// #endif    
//       UI->ApplyCommand("/control/execute vis.mac"); 
//       session->SessionStart();
//       delete session;
//    }
   
//   else         // Batch mode
//    {
//      G4String command = "/control/execute ";
//      G4String fileName = argv[1];
//      UI->ApplyCommand(command+fileName);
//    }
   
//   // Job termination
//   // Free the store: user actions, physics_list and detector_description are
//   //                 owned and deleted by the run manager, so they should not
//   //                 be deleted in the main() program !

//   #ifdef G4VIS_USE
//   delete visManager;
//   #endif
//   delete runManager;
//   delete verbosity;

//   return 0;
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......





  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else { 
    // interactive mode
    UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete visManager;
  delete runManager;
}
