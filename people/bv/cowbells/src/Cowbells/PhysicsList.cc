#include "Cowbells/PhysicsList.h"
#include "Cowbells/PhysicsConsGeneral.h"
#include "Cowbells/PhysicsConsEM.h"
#include "Cowbells/PhysicsConsMuon.h"

#include <G4DecayPhysics.hh>
#include <G4RadioactiveDecayPhysics.hh>
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

    // These three classes are copied from extended/optical/LXe example

    //RegisterPhysics( new Cowbells::PhysicsConsGeneral("general") );
    RegisterPhysics( new Cowbells::PhysicsConsEM("standard EM"));
    RegisterPhysics( new Cowbells::PhysicsConsMuon("muon"));

    // Standard G4 physics constructors:

    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    RegisterPhysics( opticalPhysics );

    opticalPhysics->SetWLSTimeProfile("delta");
    opticalPhysics->SetScintillationYieldFactor(1.0);
    opticalPhysics->SetScintillationExcitationRatio(0.0);
    opticalPhysics->SetMaxNumPhotonsPerStep(100);
    opticalPhysics->SetMaxBetaChangePerStep(10.0);
    opticalPhysics->SetTrackSecondariesFirst(kCerenkov,true);
    opticalPhysics->SetTrackSecondariesFirst(kScintillation,true);


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
