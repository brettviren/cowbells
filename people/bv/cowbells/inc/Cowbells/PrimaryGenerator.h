/**
 * \class PrimaryGenerator
 *
 * \brief A G4 primary generator action
 *
 * This class generates the primary kinematics based on a given
 * description string.  The description is in a URL'ish form:
 *
 * <scheme>:<descriptor>
 *
 * The scheme is one of:
 *
 *  file:/path/to/kinematics/file.txt
 *
 *  gun:/<pdgcode>/<vx,vy,vz>/<px,py,pz>
 *
 *  ball:/<pdgcode>/<vx,vy,vz>/<totalenergy>
 *
 * All numerical quantites are assumed to be in the G4 system of units.
 *
 * bv@bnl.gov Thu May 10 12:58:31 2012
 *
 */



#ifndef PRIMARYGENERATOR_H
#define PRIMARYGENERATOR_H

#include "Cowbells/Event.h"
#include "G4VUserPrimaryGeneratorAction.hh"

namespace Cowbells {

    class Generatelet {
    public:
        virtual ~Generatelet() {}
        virtual void generate(G4Event* event) = 0;
    };


    class PrimaryGenerator : public G4VUserPrimaryGeneratorAction {
    public:
        PrimaryGenerator(const char* kindesc = 0);
        virtual ~PrimaryGenerator();

        // Required interface
        void GeneratePrimaries(G4Event*);

        void SetKinDesc(const char* kindesc);

    private:

        Cowbells::Generatelet* m_gen;

    };
}


#endif  // PRIMARYGENERATOR_H
