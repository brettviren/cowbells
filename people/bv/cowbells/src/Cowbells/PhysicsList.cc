#include "Cowbells/PhysicsList.h"
#include "Cowbells/PhysicsConsGeneral.h"
#include "Cowbells/PhysicsConsOp.h"
#include "Cowbells/PhysicsConsEM.h"
#include "Cowbells/PhysicsConsMuon.h"

#include "G4HadronElasticPhysics.hh"

#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"

#include <iostream>
using namespace std;

Cowbells::PhysicsList::PhysicsList()
    : G4VModularPhysicsList()
{
    cerr << "Creating Cowbells::PhysicsList" << endl;

    defaultCutValue = 1.0*mm;

    verboseLevel = 9;

    RegisterPhysics( new Cowbells::PhysicsConsGeneral() );
    RegisterPhysics( new Cowbells::PhysicsConsEM() );
    RegisterPhysics( new Cowbells::PhysicsConsMuon() );
    RegisterPhysics( new Cowbells::PhysicsConsOp() );
    RegisterPhysics( new G4HadronElasticPhysics() );

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
