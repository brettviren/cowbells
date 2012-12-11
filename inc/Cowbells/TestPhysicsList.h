/**
 * \class TestPhysicsList
 *
 * \brief Rip from N06
 *
 *
 * bv@bnl.gov Tue Jul 17 10:30:36 2012
 *
 */

#ifndef TESTPHYSICSLIST_H
#define TESTPHYSICSLIST_H

#include "G4VUserPhysicsList.hh"
class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;

namespace Cowbells {
    class TestPhysicsList : public G4VUserPhysicsList
    {
    public:
        TestPhysicsList();
        ~TestPhysicsList();

    public:
        void ConstructParticle();
        void ConstructProcess();

        void SetCuts();

        //these methods Construct particles
        void ConstructBosons();
        void ConstructLeptons();
        void ConstructMesons();
        void ConstructBaryons();

        //these methods Construct physics processes and register them
        void ConstructGeneral();
        void ConstructEM();
        void ConstructOp();

        //for the Messenger 
        void SetVerbose(G4int);
        void SetNbOfPhotonsCerenkov(G4int);

    private:
        G4Cerenkov*          theCerenkovProcess;
        G4Scintillation*     theScintillationProcess;
        G4OpAbsorption*      theAbsorptionProcess;
        G4OpRayleigh*        theRayleighScatteringProcess;
        G4OpMieHG*           theMieHGScatteringProcess;
        G4OpBoundaryProcess* theBoundaryProcess;
    };

}


#endif  // TESTPHYSICSLIST_H
