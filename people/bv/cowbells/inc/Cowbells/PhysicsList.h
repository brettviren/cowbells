/**
 * \class PhysicsList
 *
 * \brief A G4VUserPhysicsList for Cowbells
 *
 * bv@bnl.gov Sun May  6 11:17:00 2012
 *
 */



#ifndef COWBELLS_PHYSICSLIST_H
#define COWBELLS_PHYSICSLIST_H

#include <G4VModularPhysicsList.hh>

namespace Cowbells {

    class PhysicsList : public G4VModularPhysicsList {
    public:
        PhysicsList();
        virtual ~PhysicsList();

        void SetCuts();

    private:
        PhysicsList(const PhysicsList& rhs);
        PhysicsList& operator=(const PhysicsList& rhs);
        
    };

}
#endif  // COWBELLS_PHYSICSLIST_H
