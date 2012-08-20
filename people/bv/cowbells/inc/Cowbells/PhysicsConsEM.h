#ifndef PHYSICSCONSEM_H
#define PHYSICSCONSEM_H
#include "G4VPhysicsConstructor.hh"

#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

namespace Cowbells {
class PhysicsConsEM : public G4VPhysicsConstructor 
{
public: 
    PhysicsConsEM();
    virtual ~PhysicsConsEM();

public: 

    virtual void ConstructParticle();
    virtual void ConstructProcess();

protected:
    // Gamma physics
    G4PhotoElectricEffect* thePhotoEffect;
    G4ComptonScattering* theComptonEffect;
    G4GammaConversion* thePairProduction;
  
    // Electron physics
    G4eMultipleScattering* theElectronMultipleScattering;
    G4eIonisation* theElectronIonisation;
    G4eBremsstrahlung* theElectronBremsStrahlung;
  
    //Positron physics
    G4eMultipleScattering* thePositronMultipleScattering;
    G4eIonisation* thePositronIonisation; 
    G4eBremsstrahlung* thePositronBremsStrahlung;  
    G4eplusAnnihilation* theAnnihilation;

};
}

#endif  // PHYSICSCONSEM_H
