
#include "Cowbells/Interface.h"
#include "Cowbells/DataRecorder.h"
#include "Cowbells/DetConsBase.h"
#include "Cowbells/BuildFromRoot.h"
#include "Cowbells/BuildFromGdml.h"
#include "Cowbells/PrimaryGenerator.h"
#include "Cowbells/PhysicsList.h"
#include "Cowbells/SensitiveDetector.h"
#include "Cowbells/RunAction.h"
#include "Cowbells/EventAction.h"
#include "Cowbells/Event.h"
#include "Cowbells/Hit.h"

#include "Cowbells/TestDetectorConstruction.h"
#include "Cowbells/TestPhysicsList.h"
#include "Cowbells/TestPrimaryGeneratorAction.h"
#include "Cowbells/TestRunAction.h"
#include "Cowbells/TestStackingAction.h"
#include "Cowbells/TestSteppingVerbose.h"
#include "Cowbells/TestMain.h"
#include "Cowbells/TestCB.h"

// Exposed Geant4 classes
#include <G4RunManager.hh>
#include <G4VModularPhysicsList.hh>
#include <G4VUserPhysicsList.hh>
#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4VUserDetectorConstruction.hh>
#include <G4UImanager.hh>
#include <G4THitsCollection.hh>

#include "HepMC/GenRanges.h"
#include "HepMC/GenEvent.h"

// export the CLHEP system of units to Python.  For some reason
// genreflex only "sees" this header if we force it to be #include'd
// again by undefining its protection.  If it is moved to the top of
// this file then it causes the units to go undefined when they are
// later included by G4 files.
#undef HEP_SYSTEM_OF_UNITS_H
#undef HEP_PHYSICAL_CONSTANTS_H
namespace units {
//#include "G4SystemOfUnits.hh"
// #include "CLHEP/Units/SystemOfUnits.h" 
// next one pulls this in
#include "CLHEP/Units/PhysicalConstants.h"
}

namespace CowbellsInstantiations
{
    
    struct __Instantiations 
    {
        std::vector<Cowbells::Hit*> vch;
    };    
}
