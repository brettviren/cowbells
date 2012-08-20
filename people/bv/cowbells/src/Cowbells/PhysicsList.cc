#include "Cowbells/PhysicsList.h"
#include "Cowbells/PhysicsConsGeneral.h"
#include "Cowbells/PhysicsConsEM.h"
#include "Cowbells/PhysicsConsMuon.h"
#include "Cowbells/PhysicsConsOp.h"

#include <G4DecayPhysics.hh>
#include <G4RadioactiveDecayPhysics.hh>

#include "G4Electron.hh"
#include "G4Positron.hh"

#include <iostream>
using namespace std;

Cowbells::PhysicsList::PhysicsList()
    : G4VModularPhysicsList()
{
    cerr << "Creating Cowbells::PhysicsList" << endl;

    defaultCutValue = 1.0*mm;

    // These three classes are copied from extended/optical/LXe examples

    //RegisterPhysics( new Cowbells::PhysicsConsGeneral("general") );
    RegisterPhysics( new Cowbells::PhysicsConsEM()   );
    RegisterPhysics( new Cowbells::PhysicsConsMuon() );
    RegisterPhysics( new Cowbells::PhysicsConsOp()   );

    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());


    // fixme: 
    //RegisterPhysics(new hadronic physics...);
}


Cowbells::PhysicsList::~PhysicsList()
{
    cerr << "Destructing Cowbells::PhysicsList" << endl;
}

void Cowbells::PhysicsList::SetCuts()
{
    SetCutsWithDefault();
}
