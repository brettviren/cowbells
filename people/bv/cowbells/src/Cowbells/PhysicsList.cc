#include "Cowbells/PhysicsList.h"
#include "Cowbells/PhysicsConsGeneral.h"
#include "Cowbells/PhysicsConsOp.h"
#include "Cowbells/PhysicsConsEM.h"
#include "Cowbells/PhysicsConsMuon.h"
#include "Cowbells/strutil.h"

#include "G4HadronElasticPhysics.hh"

#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"

#include <iostream>
using namespace std;

using Cowbells::get_startswith;

Cowbells::PhysicsList::PhysicsList(const char* physics)
    : G4VModularPhysicsList()
{
    string all("all");
    if (!physics || all == physics) {
        physics = "em,op,had";
    }
    cerr << "Creating Cowbells::PhysicsList with: \"" << physics << "\"" << endl;

    defaultCutValue = 1.0*mm;

    verboseLevel = 9;

    // always
    RegisterPhysics( new Cowbells::PhysicsConsGeneral() );

    if (get_startswith(physics,"em",",","notfound") != "notfound") {
        cout << "\tusing EM Physics" << endl;
        RegisterPhysics( new Cowbells::PhysicsConsEM() );
        RegisterPhysics( new Cowbells::PhysicsConsMuon() );
    }
    if (get_startswith(physics,"op",",","notfound") != "notfound") {
        cout << "\tusing Optical Physics" << endl;
        RegisterPhysics( new Cowbells::PhysicsConsOp() );
    }
    if (get_startswith(physics,"had",",","notfound") != "notfound") {
        cout << "\tusing Hadronic Physics" << endl;
        RegisterPhysics( new G4HadronElasticPhysics() );
    }

    SetVerboseLevel(verboseLevel);
}


Cowbells::PhysicsList::~PhysicsList()
{
    cerr << "Destructing Cowbells::PhysicsList" << endl;
}

void Cowbells::PhysicsList::SetCuts()
{
    SetCutsWithDefault();
}

void Cowbells::PhysicsList::ConstructParticle()
{
    this->G4VModularPhysicsList::ConstructParticle();
}
void Cowbells::PhysicsList::ConstructProcess()
{
    this->G4VModularPhysicsList::ConstructProcess();

    assert(G4Electron::Definition()->GetProcessManager());
    assert(G4Proton::Definition()->GetProcessManager());    
    assert(G4Neutron::Definition()->GetProcessManager());    
}
