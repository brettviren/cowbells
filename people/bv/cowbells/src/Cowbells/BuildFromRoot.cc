#include "Cowbells/BuildFromRoot.h"

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

#include <vector>

#include <iostream>
using namespace std;

Cowbells::BuildFromRoot::BuildFromRoot(const char* root_geom_filename)
 : m_geomfilename(root_geom_filename)
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
    TGeoManager* geo = TGeoManager::Import(m_geomfilename);

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

    // Tack on any properties.  

    // This is a vector<G4Material*>
    const G4MaterialTable& mattab = *G4Material::GetMaterialTable();

    // Expect TDirectory hiearchy like
    // properties/MATERIALNAME/PROPERTYNAME where properties are
    // expressed as TGraphs.
    TFile* propfile = TFile::Open(m_geomfilename);

    // FIXME: make the property directory configurable
    TDirectory* propdir = dynamic_cast<TDirectory*>(propfile->Get("properties"));

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

    return world;
}
