/**
 * \class PhysicsConsOp
 *
 * \brief Optical Physics
 *
 * taken from geant4.9.5.p01/examples/extended/optical/wls
 *
 * bv@bnl.gov Mon Aug 20 16:27:16 2012
 *
 */



#ifndef PHYSICSCONSOP_H
#define PHYSICSCONSOP_H

#include "G4OpWLS.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"

#include "G4OpMieHG.hh"
#include "G4OpRayleigh.hh"
#include "G4OpAbsorption.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4VPhysicsConstructor.hh"

namespace Cowbells {

class PhysicsConsOp : public G4VPhysicsConstructor 
{
public: 
    PhysicsConsOp();
    virtual ~PhysicsConsOp();

public: 

    virtual void ConstructParticle();
    virtual void ConstructProcess();

protected:

    G4OpWLS*             theWLSProcess;
    G4Scintillation*     theScintProcess;
    G4Cerenkov*          theCerenkovProcess;
    G4OpBoundaryProcess* theBoundaryProcess;
    G4OpAbsorption*      theAbsorptionProcess;
    G4OpRayleigh*        theRayleighScattering;
    G4OpMieHG*           theMieHGScatteringProcess;

};

}

#endif  // PHYSICSCONSOP_H
