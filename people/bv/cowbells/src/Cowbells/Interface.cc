#include "Cowbells/Interface.h"
#include "Cowbells/BuildFromRoot.h"
#include "Cowbells/PhysicsList.h"
#include "Cowbells/PrimaryGenerator.h"
#include "Cowbells/SensitiveDetector.h"

#include <G4RunManager.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SDManager.hh>

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

    // fixme: need to make this more flexible to allow for different
    // types of primary generation schemes
    cerr << "Cowbells::Interface::configure initialize PrimaryGenerator" << endl;
    m_primgen = new Cowbells::PrimaryGenerator();
    m_runmgr->SetUserAction(m_primgen);
}
        

void Cowbells::Interface::register_lvsd(const char* logvol, const char* sensdet)
{
    string sdname(sensdet);
    string lvname(logvol);

    if (sdname == "SensitiveDetector") {
        G4LogicalVolume* lv = G4LogicalVolumeStore::GetInstance()->GetVolume(lvname.c_str());
        if (!lv) { 
            cerr << "Cowbells::Interface::register_lvsd: ERROR: no such logical volume \"" 
                 << lvname << "\"" << endl;
            return;
        }

        string hcname = lvname+"HC";
        Cowbells::SensitiveDetector* csd = new Cowbells::SensitiveDetector(sdname.c_str(), hcname.c_str());
        G4SDManager::GetSDMpointer()->AddNewDetector(csd);
        lv->SetSensitiveDetector(csd);
        cout << "Cowbells::Interface::register_lvsd(\""<<logvol
             <<"\",\"" <<sensdet<<"\")" << endl;
    }
    // else if ( some other SD called for) {...}
    else {
        cerr << "Cowbells::Interface::register_lvsd(\""<<logvol
             <<"\",\"" <<sensdet<<"\"): ERROR: unknown sensitive detector" << endl;
        return;
    }


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

    m_runmgr->SetNumberOfEventsToBeStored(1);

    //if (kin) m_primgen->set(kin);
    m_runmgr->BeamOn(1);

    // fixme: dig out result and return
    cerr << "Cowbells::Interface::simulate dumping g4event:" << endl;
    const G4Event* event = m_runmgr->GetPreviousEvent(1);
    if (!event) {
        cerr << "Cowbells::Interface::simulate dumping got null event!:" << endl;        
        return;
    }
    event->Print();
}


Cowbells::Interface* Cowbells::interface()
{
    static Cowbells::Interface* interface = 0;
    if (!interface) {
        interface = new Cowbells::Interface();
    }
    return interface;
}
