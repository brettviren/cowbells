#include "Cowbells/PhysicsList.h"

#include <G4DecayPhysics.hh>
#include <G4RadioactiveDecayPhysics.hh>
#include <G4EmStandardPhysics.hh>
#include <G4OpticalPhysics.hh>

#include "G4Electron.hh"
#include "G4Positron.hh"

#include <iostream>
using namespace std;

Cowbells::PhysicsList::PhysicsList()
    : G4VModularPhysicsList()
{
    cerr << "Creating Cowbells::PhysicsList" << endl;

    defaultCutValue = 1.0*mm;

    // fixme: need to extend hooks to configure the underlying
    // processes.

    // Fixme: G4EmStandardPhysics::ConstructParticle has the following:
    ////G4Electron::Electron();
    ////G4Positron::Positron();
    // but I get the error:
    // physicsList->CheckParticleList() start.
    // G4PhysicsListHelper::CheckParticleList: e-  do not exist 
    //  These particle are necessary for basic EM processes
    //
    // If I add these two here then I get:
    ///G4PhysicsListHelper::CheckParticleList: e- e+  do not exist 
    // wtf ?!?

    // G4Electron * ele = G4Electron::Electron();
    // G4Positron * pos = G4Positron::Positron();
    // cerr << "Cowbells::PhysicsList::PhysicsList"
    //      << " G4Electron at " << (void*)ele 
    //      << " G4Positron at " << (void*)pos << endl;
    SetVerboseLevel(9);

    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());

    // fixme: G4EmStandardPhysics does not do MuonMinusCaptureAtRest
    RegisterPhysics(new G4EmStandardPhysics());

    // fixme: should be okay, but needs proper configuration:
    RegisterPhysics(new G4OpticalPhysics());


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
