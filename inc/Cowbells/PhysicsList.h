/**
 * \class PhysicsList
 *
 * \brief A G4VUserPhysicsList for Cowbells
 *
 * The phyiscs to use can be controlled by a string.  Either "all" (or
 * null) can be specified and all physics will be used or a comma
 * separated list can be given to the constructor made up of:
 *
 *  "em" - E&M physics
 *  "op" - optical physics
 *  "had" - hadronic physics
 *
 * bv@bnl.gov Sun May  6 11:17:00 2012
 *
 */



#ifndef COWBELLS_PHYSICSLIST_H
#define COWBELLS_PHYSICSLIST_H

#include <G4VModularPhysicsList.hh>

#include <vector>
#include <string>

namespace Cowbells {

    class PhysicsList : public G4VModularPhysicsList {
    public:
        typedef std::vector<std::string> ConfigPhysicsList;

        PhysicsList(ConfigPhysicsList phys_list, double default_cut = 0.1*mm);
        virtual ~PhysicsList();

        void SetCuts();

        virtual void ConstructParticle();
        virtual void ConstructProcess();


    private:
        PhysicsList(const PhysicsList& rhs);
        PhysicsList& operator=(const PhysicsList& rhs);
        
    };

}
#endif  // COWBELLS_PHYSICSLIST_H
