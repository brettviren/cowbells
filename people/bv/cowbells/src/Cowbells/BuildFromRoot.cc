#include "Cowbells/BuildFromRoot.h"
#include "Cowbells/SensitiveDetector.h"

#include <RootGM/volumes/Factory.h>
#include <Geant4GM/volumes/Factory.h>

#include <TGeoManager.h>
#include <TKey.h>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TGraph.h>
#include <G4VPhysicalVolume.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4MaterialTable.hh>
#include <G4Material.hh>

// for registering sensitive detectors
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SDManager.hh>

#include <vector>

#include <iostream>
using namespace std;

Cowbells::BuildFromRoot::BuildFromRoot(std::string filename)
    : m_filename(filename)
{
}

Cowbells::BuildFromRoot::~BuildFromRoot()
{
}

static G4Material* get_mat(const G4MaterialTable& mattab, std::string matname)
{
    size_t nmat = mattab.size();
    for (size_t imat=0; imat < nmat; ++imat) {
        G4Material* mat = mattab[imat];
        if (mat->GetName() == matname.c_str()) { 
            return mat;
        }
    }
    return 0;
}

G4VPhysicalVolume* Cowbells::BuildFromRoot::Construct()
{
    G4VPhysicalVolume* world = this->ConstructGeometry();
    this->AddMaterialProperties();
    this->RegisterSensDets();
    return world;
}

G4VPhysicalVolume* Cowbells::BuildFromRoot::ConstructGeometry()
{
    TGeoManager* geo = TGeoManager::Import(m_filename.c_str());

    // Import geometry from Root to VGM
    RootGM::Factory rtFactory;
    rtFactory.SetDebug(0);
    rtFactory.Import(geo->GetTopNode());
    cerr << "Loaded ROOT geometry" << endl;

    // Export VGM geometry to Geant4
    Geant4GM::Factory g4Factory;
    g4Factory.SetDebug(0);
    rtFactory.Export(&g4Factory);
    G4VPhysicalVolume * world = g4Factory.World();
    cerr << "Converted to Geant4 geometry" << endl;

    return world;
}

void Cowbells::BuildFromRoot::AddMaterialProperties()
{
    // Tack on any properties.  

    // This is a vector<G4Material*>
    const G4MaterialTable& mattab = *G4Material::GetMaterialTable();

    // Expect TDirectory hiearchy like
    // properties/MATERIALNAME/PROPERTYNAME where properties are
    // expressed as TGraphs.
    TFile* propfile = TFile::Open(m_filename.c_str());

    // FIXME: make the property directory configurable
    const char* prop_dir_name = "properties";
    TDirectory* propdir = dynamic_cast<TDirectory*>(propfile->Get(prop_dir_name));

    TList* lom = propdir->GetListOfKeys();
    int nmats = lom->GetSize();
    for (int imat=0; imat < nmats; ++imat) {
        TKey* mkey = (TKey*)lom->At(imat);
        TDirectoryFile* matdir = dynamic_cast<TDirectoryFile*>(mkey->ReadObj());
        if (!matdir) {
            cerr << "Failed to get directory at " << imat << endl;
            cerr << "Thing is called " << lom->At(imat)->GetName() << endl;
            continue;
        }
        std::string matname = matdir->GetName();
        G4Material* mat = get_mat(mattab, matname);
        if (!mat) {
            cerr << "No G4 material named \"" << matname << "\" found, skipping setting its properties" << endl;
            continue;
        }
        
        G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
        mat->SetMaterialPropertiesTable(mpt);

        TList* lop = matdir->GetListOfKeys();
        int nlops = lop->GetSize();
        for (int ilop=0; ilop < nlops; ++ilop) {
            TKey* pkey = (TKey*)lop->At(ilop);
            TGraph* prop = dynamic_cast<TGraph*>(pkey->ReadObj());
            std::string propname = prop->GetName();
            int proplen = prop->GetN();
            if (proplen == 1) { // scalar
                double propval = prop->GetY()[0];
                mpt->AddConstProperty(propname.c_str(), propval);
                cerr << "Set " << matname << "/" << propname
                     << "[" << proplen << "] = " << propval << endl;
            }
            else {              // vector
                double* propvals = prop->GetY();
                mpt->AddProperty(propname.c_str(), prop->GetX(), propvals, proplen);
                cerr << "Set " << matname << "/" << propname
                     << "[" << proplen << "] : (" << propvals[0] << " - " << propvals[proplen-1] << ")" << endl;
            }
        } // loop over properties
        
    } // loop over materials

}

void Cowbells::BuildFromRoot::add_sensdet(std::string lvname,
                                          std::string hcname,
                                          std::string sdname)
{
    if (hcname.empty()) {
        hcname = lvname + "HC";
    }

    if (sdname.empty()) {
        sdname = "SensitiveDetector";
    }

    if (sdname != "SensitiveDetector") {
        cerr << "Cowbells::BuildFromRoot::add_sensdet currently only supports Cowbells::SensitiveDetector" << endl;
        return;
    }

    Cowbells::SensitiveDetector* csd = new Cowbells::SensitiveDetector(sdname.c_str(), hcname.c_str());
    m_lvsd[lvname] = csd;
}

void Cowbells::BuildFromRoot::RegisterSensDets()
{


    LVSDMap_t::iterator it, done = m_lvsd.end();

    for (it = m_lvsd.begin(); it != done; ++it) {
        string lvname = it->first;
        G4VSensitiveDetector* sd = it->second;
        G4SDManager::GetSDMpointer()->AddNewDetector(sd);
        G4LogicalVolume* lv = G4LogicalVolumeStore::GetInstance()->GetVolume(lvname.c_str());
        lv->SetSensitiveDetector(sd);
        cerr << "Registered SD " << sd->GetName() << " with " << lvname << endl;
    }
        
}
