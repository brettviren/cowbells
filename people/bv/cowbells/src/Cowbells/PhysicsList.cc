#include "Cowbells/PhysicsList.h"

#include <G4DecayPhysics.hh>
#include <G4RadioactiveDecayPhysics.hh>
#include <G4EmStandardPhysics.hh>
#include <G4OpticalPhysics.hh>

Cowbells::PhysicsList::PhysicsList()
    : G4VModularPhysicsList()
{
    defaultCutValue = 1.0*mm;

    // fixme: need to extend hooks to configure the underlying
    // processes.

    // fixme: G4EmStandardPhysics does not do MuonMinusCaptureAtRest
    RegisterPhysics(new G4EmStandardPhysics());

    RegisterPhysics(new G4DecayPhysics());

    // fixme: should be okay, but needs proper configuration:
    RegisterPhysics(new G4OpticalPhysics());

    RegisterPhysics(new G4RadioactiveDecayPhysics());

    // fixme: 
    //RegisterPhysics(new hadronic physics...);
}


Cowbells::PhysicsList::~PhysicsList()
{
}

void Cowbells::PhysicsList::SetCuts()
{
    SetCutsWithDefault();
}
