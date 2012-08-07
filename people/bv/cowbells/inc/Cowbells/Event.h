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
    };
}

#endif  // COWBELLS_EVENT_H
