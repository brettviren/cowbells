/**
 * \class Interface
 *
 * \brief Main interface to the Cowbells simulation.
 *
 * Set up and interact with the simulation through this class.
 *
 * bv@bnl.gov Mon May  7 15:04:03 2012
 *
 */



#ifndef COWBELLS_INTERFACE_H
#define COWBELLS_INTERFACE_H

#include <Cowbells/Event.h>

class G4RunManager;


namespace Cowbells {

    class PrimaryGenerator;

    class Interface {
    public:
        Interface();
        ~Interface();

        void activate_physics_list();

        /// Configure the simulation with a (ROOT) geometry file.
        void configure(const char* geofile);
        void dump_geometry(void);

        /// Access the G4RunManager
        G4RunManager* runMgr() { return m_runmgr; }

        /// Initialize the simulation
        void initialize();

        /// Register a sensitive detector to handle hits in a logical
        /// volume.  Note, this MUST be done after initalize()
        void register_lvsd(const char* logvol, const char* sensdet = "SensitiveDetector");
        
        /// Simulate the event kinematics
        void simulate();
        //void simulate(const Cowbells::EventKinematics* event = 0);

    private:
        G4RunManager* m_runmgr;
        Cowbells::PrimaryGenerator *m_primgen;
    };

Cowbells::Interface* interface();
}

#endif  // COWBELLS_INTERFACE_H
