/**
 * \class PrimaryGenerator
 *
 * \brief A G4 primary generator action
 *
 * Manages a queue of kinematics
 *
 * bv@bnl.gov Thu May 10 12:58:31 2012
 *
 */



#ifndef PRIMARYGENERATOR_H
#define PRIMARYGENERATOR_H

#include "Cowbells/Event.h"
#include "G4VUserPrimaryGeneratorAction.hh"

namespace Cowbells {
    class PrimaryGenerator : public G4VUserPrimaryGeneratorAction {
    public:
        PrimaryGenerator();
        virtual ~PrimaryGenerator();

        // Required interface
        void GeneratePrimaries(G4Event*);

        /// Add event kinematics to use for next event.  The object
        /// should live for the life of the processing.
        void set(const Cowbells::EventKinematics* kin) { m_kine = kin; }

    private:
        const Cowbells::EventKinematics* m_kine;
    };
}


#endif  // PRIMARYGENERATOR_H
