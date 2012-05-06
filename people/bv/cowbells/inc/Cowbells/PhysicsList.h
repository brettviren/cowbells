/**
 * \class PhysicsList
 *
 * \brief A G4VUserPhysicsList for Cowbells
 *
 * bv@bnl.gov Sun May  6 11:17:00 2012
 *
 */



#ifndef PHYSICSLIST_H
#define PHYSICSLIST_H

#include <G4VModularPhysicsList.hh>

namespace Cowbells {

    class PhysicsList : public G4VModularPhysicsList {
    public:
        PhysicsList();
        virtual ~PhysicsList();

        void SetCuts();
    };

}
#endif  // PHYSICSLIST_H
