#include "Cowbells/PhysicsConsEM.h"

Cowbells::PhysicsConsEM::PhysicsConsEM()
    :  G4VPhysicsConstructor("standard EM")
{
}

Cowbells::PhysicsConsEM::~PhysicsConsEM()
{
}

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

#include "G4Gamma.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

void Cowbells::PhysicsConsEM::ConstructParticle()
{
    // gamma
    G4Gamma::GammaDefinition();
 
    // electron
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();
    G4NeutrinoE::NeutrinoEDefinition();
    G4AntiNeutrinoE::AntiNeutrinoEDefinition();
}


#include "G4ProcessManager.hh"
void Cowbells::PhysicsConsEM::ConstructProcess()
{
    thePhotoEffect = new G4PhotoElectricEffect();
    theComptonEffect = new G4ComptonScattering();
    thePairProduction = new G4GammaConversion();
  
    // Electron physics
    theElectronMultipleScattering = new G4eMultipleScattering();
    theElectronIonisation = new G4eIonisation();
    theElectronBremsStrahlung = new G4eBremsstrahlung();
  
    //Positron physics
    thePositronMultipleScattering = new G4eMultipleScattering();
    thePositronIonisation = new G4eIonisation(); 
    thePositronBremsStrahlung = new G4eBremsstrahlung();  
    theAnnihilation = new G4eplusAnnihilation();


    G4ProcessManager * pManager = 0;
  
    // Gamma Physics
    pManager = G4Gamma::Gamma()->GetProcessManager();
    pManager->AddDiscreteProcess(thePhotoEffect);
    pManager->AddDiscreteProcess(theComptonEffect);
    pManager->AddDiscreteProcess(thePairProduction);

    // Electron Physics
    pManager = G4Electron::Electron()->GetProcessManager();

    pManager->AddProcess(theElectronMultipleScattering, -1, 1, 1);
    pManager->AddProcess(theElectronIonisation,         -1, 2, 2);
    pManager->AddProcess(theElectronBremsStrahlung,     -1, 3, 3);  


    //Positron Physics
    pManager = G4Positron::Positron()->GetProcessManager();
 
    pManager->AddProcess(thePositronMultipleScattering, -1, 1, 1);
    pManager->AddProcess(thePositronIonisation,         -1, 2, 2);
    pManager->AddProcess(thePositronBremsStrahlung,     -1, 3, 3);  
    pManager->AddProcess(theAnnihilation,                0,-1, 4);  

}

