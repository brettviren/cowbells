#include "Cowbells/PhysicsList.h"

#include <G4DecayPhysics.hh>
#include <G4RadioactiveDecayPhysics.hh>
#include <G4EmStandardPhysics.hh>
#include <G4OpticalPhysics.hh>

Cowbells::PhysicsList::PhysicsList()
    : G4VModularPhysicsList()
{
       defaultCutValue = 1.0*mm;

       RegisterPhysics(new G4DecayPhysics());
       RegisterPhysics(new G4RadioactiveDecayPhysics());
       RegisterPhysics(new G4EmStandardPhysics());
       RegisterPhysics(new G4OpticalPhysics());
}

Cowbells::PhysicsList::~PhysicsList()
{
}

void Cowbells::PhysicsList::SetCuts()
{
    SetCutsWithDefault();
}
