#include "Cowbells/PhysicsConsMuon.h"

Cowbells::PhysicsConsMuon::PhysicsConsMuon()
    :  G4VPhysicsConstructor("muon")
{
}

Cowbells::PhysicsConsMuon::~PhysicsConsMuon()
{
}

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"

void Cowbells::PhysicsConsMuon::ConstructParticle()
{
    // Mu
    G4MuonPlus::MuonPlusDefinition();
    G4MuonMinus::MuonMinusDefinition();
    G4NeutrinoMu::NeutrinoMuDefinition();
    G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}


#include "G4ProcessManager.hh"

void Cowbells::PhysicsConsMuon::ConstructProcess()
{
    fMuPlusIonisation = new G4MuIonisation();
    fMuPlusMultipleScattering = new G4MuMultipleScattering();
    fMuPlusBremsstrahlung=new G4MuBremsstrahlung();
    fMuPlusPairProduction= new G4MuPairProduction();

    fMuMinusIonisation = new G4MuIonisation();
    fMuMinusMultipleScattering = new G4MuMultipleScattering;
    fMuMinusBremsstrahlung = new G4MuBremsstrahlung();
    fMuMinusPairProduction = new G4MuPairProduction();

    fMuMinusCaptureAtRest = new G4MuonMinusCaptureAtRest();

    G4ProcessManager * pManager = 0;

    // Muon Plus Physics
    pManager = G4MuonPlus::MuonPlus()->GetProcessManager();
   
    pManager->AddProcess(fMuPlusMultipleScattering,-1,  1, 1);
    pManager->AddProcess(fMuPlusIonisation,        -1,  2, 2);
    pManager->AddProcess(fMuPlusBremsstrahlung,    -1,  3, 3);
    pManager->AddProcess(fMuPlusPairProduction,    -1,  4, 4);

    // Muon Minus Physics
    pManager = G4MuonMinus::MuonMinus()->GetProcessManager();
   
    pManager->AddProcess(fMuMinusMultipleScattering,-1,  1, 1);
    pManager->AddProcess(fMuMinusIonisation,        -1,  2, 2);
    pManager->AddProcess(fMuMinusBremsstrahlung,    -1,  3, 3);
    pManager->AddProcess(fMuMinusPairProduction,    -1,  4, 4);

    pManager->AddRestProcess(fMuMinusCaptureAtRest);

}
