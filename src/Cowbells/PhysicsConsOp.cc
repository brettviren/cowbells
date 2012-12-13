
#include "Cowbells/PhysicsConsOp.h"

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

#include "G4OpticalPhoton.hh"

#include <sstream>

Cowbells::PhysicsConsOp::PhysicsConsOp()
    : G4VPhysicsConstructor("Optical")
    , theWLSProcess(0)
    , theScintProcess(0)
    , theCerenkovProcess(0)
    , theBoundaryProcess(0)
    , theAbsorptionProcess(0)
    , theRayleighScattering(0)
    , theMieHGScatteringProcess(0)
{
}

Cowbells::PhysicsConsOp::~PhysicsConsOp()
{
}



void Cowbells::PhysicsConsOp::ConstructParticle()
{
    G4OpticalPhoton::OpticalPhotonDefinition();
}

#include "G4ProcessManager.hh"

void Cowbells::PhysicsConsOp::ConstructProcess()
{
    theWLSProcess = new G4OpWLS();

    theScintProcess = new G4Scintillation();
    theScintProcess->SetScintillationYieldFactor(1.);
    theScintProcess->SetTrackSecondariesFirst(true);

    theCerenkovProcess = new G4Cerenkov();
    theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
    theCerenkovProcess->SetTrackSecondariesFirst(true);

    theAbsorptionProcess      = new G4OpAbsorption();
    theRayleighScattering     = new G4OpRayleigh();
    theMieHGScatteringProcess = new G4OpMieHG();
    theBoundaryProcess        = new G4OpBoundaryProcess();

    G4ProcessManager* pManager =
        G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

    if (!pManager) {
        std::ostringstream o;
        o << "Optical Photon without a Process Manager";
        G4Exception("WLSOpticalPhysics::ConstructProcess()","",
                    FatalException,o.str().c_str());
    }



    pManager->AddDiscreteProcess(theAbsorptionProcess);
    //pManager->AddDiscreteProcess(theRayleighScattering);
    //pManager->AddDiscreteProcess(theMieHGScatteringProcess);

    // no more model in g4.9.6
    // https://geant4.web.cern.ch/geant4/support/ReleaseNotes4.9.6.html
    //theBoundaryProcess->SetModel(glisur);
    //theBoundaryProcess->SetModel(unified);
    pManager->AddDiscreteProcess(theBoundaryProcess);

    theWLSProcess->UseTimeProfile("delta");
    //theWLSProcess->UseTimeProfile("exponential");
    pManager->AddDiscreteProcess(theWLSProcess);

    theScintProcess->SetScintillationYieldFactor(1.);
    theScintProcess->SetScintillationExcitationRatio(0.0);
    theScintProcess->SetTrackSecondariesFirst(true);


    // Use Birks Correction in the Scintillation process
    G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
    theScintProcess->AddSaturation(emSaturation);

    theParticleIterator->reset();
    while ( (*theParticleIterator)() ) {

        G4ParticleDefinition* particle = theParticleIterator->value();
        G4String particleName = particle->GetParticleName();

        pManager = particle->GetProcessManager();
        if (!pManager) {
            std::ostringstream o;
            o << "Particle " << particleName << "without a Process Manager";
            G4Exception("WLSOpticalPhysics::ConstructProcess()","",
                        FatalException,o.str().c_str());
        }

        if(theCerenkovProcess->IsApplicable(*particle)){
            //G4cout << "Setting cerenkov process for particle " << particleName << G4endl;

            pManager->AddProcess(theCerenkovProcess);
            pManager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
        }
        if(theScintProcess->IsApplicable(*particle)){
            //G4cout << "Setting scintilation process for particle " << particleName << G4endl;

            pManager->AddProcess(theScintProcess);
            pManager->SetProcessOrderingToLast(theScintProcess,idxAtRest);
            pManager->SetProcessOrderingToLast(theScintProcess,idxPostStep);
        }

    }


}
