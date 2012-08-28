/**
 * \class Event
 *
 * \brief An event described
 *
 * An event starts as some initial kinematics.  It can then be
 * processed through the simulation to produce hits and other "truth"
 * info.
 *
 * bv@bnl.gov Thu May 10 13:08:31 2012
 *
 */



#ifndef COWBELLS_EVENT_H
#define COWBELLS_EVENT_H


#include "HepMC/GenEvent.h"
#include "Cowbells/Hit.h"

#include <vector>

namespace Cowbells {

    /// Initial kinematics of one event
    typedef HepMC::GenEvent EventKinematics;

    // Record some truth info about steps
    class Step {
    public:
        int trackid; // g4 track id number
        int parentid; // track ID number of parent;
        int pdgid; // pdg particle id
        int mat1, mat2; // material index before/after the step
        float energy1, energy2; // kinetic energy before/after the step
        float dist; // the distance stepped
        float x1,y1,z1,x2,y2,z2;
        Step();
    };

    class Event {
    public:
        Event(Cowbells::EventKinematics* kin = 0);
        ~Event();

        void clear();

        // Set the event kinematics, takes ownership
        // void set_kinematics(Cowbells::EventKinematics* kin);
        // const Cowbells::EventKinematics* get_kinematics() const;

        // Post-simulation data:
        // void set_hits()
        // void set_...()

        
        // EventKinematics* m_kine;

        std::vector<Cowbells::Hit*> hc;
        std::vector<Cowbells::Step*> steps;
    };


}

#endif  // COWBELLS_EVENT_H
