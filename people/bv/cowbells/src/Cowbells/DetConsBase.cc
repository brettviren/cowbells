#include "Cowbells/DetConsBase.h"
#include "Cowbells/SensitiveDetector.h"
#include "Cowbells/R2G4.h"
#include "Cowbells/Util.h"
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SDManager.hh>

#include <vector>

#include <iostream>
using namespace std;

Cowbells::DetConsBase::DetConsBase(std::string properties_filename)
    : m_prop_file(properties_filename)
{
}

Cowbells::DetConsBase::~DetConsBase()
{
}

G4VPhysicalVolume* Cowbells::DetConsBase::Construct()
{
    G4VPhysicalVolume* world = this->ConstructGeometry();
    {
        bool ok = Cowbells::AddMaterialProperties(m_prop_file);
        if (!ok) return 0;
    }
    {
        bool ok = Cowbells::AddOpticalSurfaces(m_prop_file);
        if (!ok) return 0;
    }

    this->RegisterSensDets();
    Cowbells::dump(world, 0);
    Cowbells::dump_lvs();
    Cowbells::dump_pvs();
    return world;
}


void Cowbells::DetConsBase::add_sensdet(std::string lvname,
                                        std::vector<std::string> touchables,
                                        std::string hcname,
                                        std::string sdname)
{
    if (hcname.empty()) {
        hcname = lvname + "HC";
    }

    if (sdname.empty()) {
        sdname = "SensitiveDetector";
    }

    // if (sdname != "SensitiveDetector") {
    //     cerr << "Cowbells::DetConsBase::add_sensdet currently only supports Cowbells::SensitiveDetector" << endl;
    //     return;
    // }

    Cowbells::SensitiveDetector* csd = 
        new Cowbells::SensitiveDetector(sdname.c_str(), hcname.c_str(), touchables);
    m_lvsd[lvname] = csd;


    cout << "Sensitive Detector: " << sdname 
         << ", LV: " << lvname << ", "
         << ", HC: " << hcname << ", " 
         << touchables.size()-1 << " touchables" << endl; 
    for (size_t ind = 0; ind < touchables.size(); ++ind) {
        if (!ind) continue;
        cout << "\t" << ind << ": " << touchables[ind] << endl;
    }
}

void Cowbells::DetConsBase::RegisterSensDets()
{
    LVSDMap_t::iterator it, done = m_lvsd.end();

    for (it = m_lvsd.begin(); it != done; ++it) {
        string lvname = it->first;
        G4VSensitiveDetector* sd = it->second;
        if (!sd) {
            cerr << "No sensitive detector for LV=" << lvname << endl;
            assert (sd);
        }
        G4SDManager::GetSDMpointer()->AddNewDetector(sd);
        G4LogicalVolume* lv = G4LogicalVolumeStore::GetInstance()->GetVolume(lvname.c_str());
        if (!lv) {
            cerr << "No LV for " << lvname << endl;
            assert (lv);
        }
        lv->SetSensitiveDetector(sd);
        cout << "Registered SD \"" << sd->GetName() 
             << "\" with logical volume \"" << lvname << "\"" << endl;
    }
        
}
