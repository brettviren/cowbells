#ifndef PHYSICSCONSGENERAL_H
#define PHYSICSCONSGENERAL_H

#include "G4VPhysicsConstructor.hh"
#include "G4Decay.hh"

namespace Cowbells {

    class PhysicsConsGeneral : public G4VPhysicsConstructor
    {
    public: 
        PhysicsConsGeneral();
        virtual ~PhysicsConsGeneral();

    public: 
        virtual void ConstructParticle();
        virtual void ConstructProcess();

    protected:
        G4Decay* fDecayProcess;
        
    };
}

#endif  // PHYSICSCONSGENERAL_H
