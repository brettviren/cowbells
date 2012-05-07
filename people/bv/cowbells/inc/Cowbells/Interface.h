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



#ifndef INTERFACE_H
#define INTERFACE_H

#include <G4RunManager.hh>

namespace Cowbells {

    class Interface {
    public:
        Interface();
        ~Interface();

        /// Configure the simulation with a (ROOT) geometry file.
        void configure(const char* geofile);
        
        /// Access the G4RunManager
        G4RunManager* runMgr() { return m_runmgr; }

    private:
        G4RunManager* m_runmgr;
    };
}

#endif  // INTERFACE_H
