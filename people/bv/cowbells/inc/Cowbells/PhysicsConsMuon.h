#ifndef PHYSICSCONSMUON_H
#define PHYSICSCONSMUON_H


#include "G4VPhysicsConstructor.hh"
#include "G4MuMultipleScattering.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4MuIonisation.hh"
#include "G4hIonisation.hh"

#include "G4MuonMinusCaptureAtRest.hh"

namespace Cowbells {

    class PhysicsConsMuon : public G4VPhysicsConstructor
    {
    public: 
        PhysicsConsMuon(const G4String& name="muon");
        virtual ~PhysicsConsMuon();

    public: 

        virtual void ConstructParticle();
        virtual void ConstructProcess();

    protected:
        // Muon physics
        G4MuIonisation*         fMuPlusIonisation;
        G4MuMultipleScattering* fMuPlusMultipleScattering;
        G4MuBremsstrahlung*     fMuPlusBremsstrahlung ;
        G4MuPairProduction*     fMuPlusPairProduction;

        G4MuIonisation*         fMuMinusIonisation;
        G4MuMultipleScattering* fMuMinusMultipleScattering;
        G4MuBremsstrahlung*     fMuMinusBremsstrahlung ;
        G4MuPairProduction*     fMuMinusPairProduction;

        G4MuonMinusCaptureAtRest* fMuMinusCaptureAtRest;

    };


}

#endif  // PHYSICSCONSMUON_H
