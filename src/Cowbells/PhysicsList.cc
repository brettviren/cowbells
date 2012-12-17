#include "Cowbells/PhysicsList.h"
#include "Cowbells/PhysicsConsGeneral.h"
#include "Cowbells/PhysicsConsOp.h"
#include "Cowbells/PhysicsConsEM.h"
#include "Cowbells/PhysicsConsMuon.h"

#include "G4HadronElasticPhysics.hh"

#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"

#include <G4EmStandardPhysics.hh>

#include <iostream>
using namespace std;

//const char* physics, float default_cut_value_mm)

Cowbells::PhysicsList::PhysicsList(ConfigPhysicsList phys_list, double default_cut)
    : G4VModularPhysicsList()
{
    defaultCutValue = default_cut;
    verboseLevel = 0;

    // always
    RegisterPhysics( new Cowbells::PhysicsConsGeneral() );

    int nphys = phys_list.size();
    if (!nphys) {
        cerr << "No physics given.  This universe is too boring to exist." << endl;
        assert (nphys);
    }

    for (int iphys=0; iphys<nphys; ++iphys) {
        string physname = phys_list[iphys];
        cout << "Registering physics: \"" << physname << "\"" << endl;
        if (physname == "em") {
            RegisterPhysics( new G4EmStandardPhysics(verboseLevel) );
            continue;
        }
        if (physname == "op") {
            RegisterPhysics( new Cowbells::PhysicsConsOp() );
            continue;
        }
        if (physname == "had") {
            RegisterPhysics( new G4HadronElasticPhysics(verboseLevel) );
            continue;
        }
        cerr << "Unknown physics: \"" << physname << "\"" << endl;
        assert(0);              // fixme: change to a throw
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

    // assert(G4Electron::Definition()->GetProcessManager());
    // assert(G4Proton::Definition()->GetProcessManager());    
    // assert(G4Neutron::Definition()->GetProcessManager());    
}
