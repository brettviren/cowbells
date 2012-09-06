#include "Cowbells/PhysicsConsGeneral.h"

#include "G4ParticleTypes.hh"

Cowbells::PhysicsConsGeneral::PhysicsConsGeneral()
    : G4VPhysicsConstructor("general")
{
}

Cowbells::PhysicsConsGeneral::~PhysicsConsGeneral()
{
}

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
// Bosons
#include "G4ChargedGeantino.hh"
#include "G4Geantino.hh"

void Cowbells::PhysicsConsGeneral::ConstructParticle()
{
    G4Geantino::GeantinoDefinition();
    G4ChargedGeantino::ChargedGeantinoDefinition();  

    // neutrinos
    G4NeutrinoE::NeutrinoEDefinition();
    G4AntiNeutrinoE::AntiNeutrinoEDefinition();
    G4NeutrinoMu::NeutrinoMuDefinition();
    G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
    G4NeutrinoTau::NeutrinoTauDefinition();
    G4AntiNeutrinoTau::AntiNeutrinoTauDefinition();
}

void Cowbells::PhysicsConsGeneral::ConstructProcess()
{
    fDecayProcess = new G4Decay();

    // Add Decay Process
    theParticleIterator->reset();
    while( (*theParticleIterator)() ){
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        if (fDecayProcess->IsApplicable(*particle)) { 
            pmanager ->AddProcess(fDecayProcess);
            // set ordering for PostStepDoIt and AtRestDoIt
            pmanager ->SetProcessOrdering(fDecayProcess, idxPostStep);
            pmanager ->SetProcessOrdering(fDecayProcess, idxAtRest);
        }
    }
}






