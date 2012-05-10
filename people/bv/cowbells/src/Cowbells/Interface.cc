#include "Cowbells/Interface.h"
#include "Cowbells/BuildFromRoot.h"
#include "Cowbells/PhysicsList.h"
#include "Cowbells/PrimaryGenerator.h"

#include "G4RunManager.hh"

#include <iostream>
using namespace std;

Cowbells::Interface::Interface()
    : m_runmgr(new G4RunManager())
    , m_primgen(0)
{
}

Cowbells::Interface::~Interface()
{
    delete(m_runmgr);
    delete(m_primgen);          // fixme: or does G4 do it?
}

void Cowbells::Interface::configure(const char* geofile)
{
    cerr << "Cowbells::Interface::configure initialize BuildFromRoot(\"" << geofile << "\")" << endl;
    m_runmgr->SetUserInitialization(new Cowbells::BuildFromRoot(geofile));
    cerr << "Cowbells::Interface::configure initialize PhysicsList" << endl;
    m_runmgr->SetUserInitialization(new Cowbells::PhysicsList());
    cerr << "Cowbells::Interface::configure initialize PrimaryGenerator" << endl;
    m_primgen = new Cowbells::PrimaryGenerator();
    m_runmgr->SetUserAction(m_primgen);
}
        
void Cowbells::Interface::initialize()
{
    cerr << "Cowbells::Interface::initialize" << endl;
    m_runmgr->Initialize();
}

//void Cowbells::Interface::simulate(const Cowbells::EventKinematics* kin)
void Cowbells::Interface::simulate()
{
    cerr << "Cowbells::Interface::simulate" << endl;
    //if (kin) m_primgen->set(kin);
    m_runmgr->BeamOn(1);
    // fixme: dig out result and return
}


Cowbells::Interface* Cowbells::interface()
{
    static Cowbells::Interface* interface = 0;
    if (!interface) {
        interface = new Cowbells::Interface();
    }
    return interface;
}
