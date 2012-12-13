#include "Cowbells/R2G4.h"

#include "Cowbells/Util.h"

#include <G4VPhysicalVolume.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4MaterialTable.hh>
#include <G4Material.hh>


#include <G4OpticalSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>

#include <TGraph.h>
#include <TNamed.h>
#include <TDirectoryFile.h>
#include <TFile.h>
#include <TKey.h>
#include <TList.h>

#include <iostream>
#include <string>
using namespace std;

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



bool Cowbells::AddMaterialProperties(std::string filename)
{
    // Tack on any properties.  

    // This is a vector<G4Material*>
    const G4MaterialTable& mattab = *G4Material::GetMaterialTable();

    // Expect TDirectory hiearchy like
    // properties/MATERIALNAME/PROPERTYNAME where properties are
    // expressed as TGraphs.
    TFile* propfile = TFile::Open(filename.c_str());

    // FIXME: make the property directory configurable
    const char* prop_dir_name = "properties";
    TDirectory* propdir = dynamic_cast<TDirectory*>(propfile->Get(prop_dir_name));

    TList* lom = propdir->GetListOfKeys();
    int nmats = lom->GetSize();
    for (int imat=0; imat < nmats; ++imat) {
        TKey* mkey = (TKey*)lom->At(imat);
        std::string matname = mkey->GetName();
        TDirectoryFile* matdir = dynamic_cast<TDirectoryFile*>(mkey->ReadObj());
        if (!matdir) {
            cerr << "Failed to get materials directory " << matname << endl;
            return false;
        }
        G4Material* mat = get_mat(mattab, matname);
        if (!mat) {
            cerr << "No G4 material named \"" << matname << "\"" << endl;
            return false;
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
                cout << "Set " << matname << "/" << propname
                     << "[" << proplen << "] = " << propval << endl;
            }
            else {              // vector
                double* propvals = prop->GetY();
                mpt->AddProperty(propname.c_str(), prop->GetX(), propvals, proplen);
                cout << "Set " << matname << "/" << propname
                     << "[" << proplen << "] : (" << propvals[0] << " - "
                     << propvals[proplen-1] << ")" << endl;
            }
        } // loop over properties
        
    } // loop over materials
    return true;
}

/*
  surfaces/SURFACENAME/parameters/PARAM_TSTRING
  surfaces/SURFACENAME/properties/PROP_TGRAPH
*/
bool Cowbells::AddOpticalSurfaces(std::string filename)
{
    TFile* fp = TFile::Open(filename.c_str());

    const char* surf_dir_name = "surfaces";
    const char* pro_dir_name = "properties";
    const char* par_dir_name = "parameters";

    TDirectory* surfaces_dir = dynamic_cast<TDirectory*>(fp->Get(surf_dir_name));
    if (!surfaces_dir) {
        cerr << "Failed to get directory " << surf_dir_name 
             << " from " << filename
             << endl;
        return false;
    }

    TList* los = surfaces_dir->GetListOfKeys();
    int nsurfs = los->GetSize();
    for (int isurf=0; isurf < nsurfs; ++isurf) { // /surfaces/SURFACE/
        TKey* skey = (TKey*)los->At(isurf);
        string surfname = skey->GetName();

        TDirectoryFile* surfdir = dynamic_cast<TDirectoryFile*>(skey->ReadObj());
        if (!surfdir) {
            cerr << "Failed to get surface directory " << surfname << endl;
            return false;
        }

        cout << "Setting optical surface: " << surfname << endl;

        string first = "", second = "";
        G4OpticalSurface* opsurf = new G4OpticalSurface(surfname.c_str());

        {                       // /surfaces/SURFACE/parameters/
            TDirectoryFile* pardir = dynamic_cast<TDirectoryFile*>(surfdir->Get(par_dir_name));
            TList* lop = pardir->GetListOfKeys();
            for (int ipar=0; ipar<lop->GetSize(); ++ipar) {
                TKey* pkey = (TKey*)lop->At(ipar);
                TNamed* par = dynamic_cast<TNamed*>(pkey->ReadObj());
                if (!par) {
                    cerr << "Failed to get TNamed parameter at " 
                         << ipar << " / " <<  pkey->GetName() << endl;
                    continue;
                }

                cout << "\toptical parameter " << par->GetName() 
                     << " = " << par->GetTitle() << endl;

                if (par->GetName() == string("first")) {
                    first = par->GetTitle();
                    continue;
                }
                if (par->GetName() == string("second")) {
                    second = par->GetTitle();
                    continue;
                }
                

                bool ok = SetOpSurfParameter(*opsurf, par->GetName(), par->GetTitle());
                if (!ok) return false;

            }
        }

        {                       // /surfaces/SURFACE/properties/
            TDirectoryFile* prodir = dynamic_cast<TDirectoryFile*>(surfdir->Get(pro_dir_name));

            TList* lop = prodir->GetListOfKeys();
            for (int ipro=0; ipro<lop->GetSize(); ++ipro) {
                TKey* pkey = (TKey*)lop->At(ipro);
                TGraph* pro = dynamic_cast<TGraph*>(pkey->ReadObj());
                if (!pro) {
                    cerr << "Failed to get TGraph optical surface property at " 
                         << ipro << " / " <<  pkey->GetName() << endl;
                    return false;
                }

                int nprop = pro->GetN();
                cout << "\toptical property " << pro->GetName() 
                     << " [" << nprop << "]: "
                     << "("<< pro->GetX()[0] << "," << pro->GetY()[0] << ") --> "
                     << "("<< pro->GetX()[nprop-1] << "," << pro->GetY()[nprop-1] << ") "
                     << endl;

                bool ok = SetOpSurfProperty(*opsurf, *pro);
                if (!ok) return false;

            }
        }

        {
            bool ok = SetLogicalSurface(*opsurf, first, second);
            if (!ok) return false;
        }
    } // loop over surfaces

    return true;
}

bool Cowbells::SetOpSurfParameter(G4OpticalSurface& opsurf,
                                  std::string name, std::string value)
{
    if (name == "model") {
        // Must match G4OpticalSurfaceModel enum
        const char* models[] = { "glisur", "unified", "LUT", 0 };
        for (int ind=0; models[ind]; ++ind) {
            if (value == models[ind]) {
                // no more model in g4.9.6
                // https://geant4.web.cern.ch/geant4/support/ReleaseNotes4.9.6.html
                // opsurf.SetModel((G4OpticalSurfaceModel)ind);
                return true;
            }
        }
        cerr << "Unknown optical surface model: " << value 
             << " for surface " << opsurf.GetName() << endl;
        return false;
    }


    if (name == "type") {
        // Must match G4SurfaceType
        const char* types[] = { "dielectric_metal", "dielectric_dielectric",
                                "dielectric_LUT", "firsov", "x_ray", 0 };
        for (int ind=0; types[ind]; ++ind) {
            if (value == types[ind]) {
                opsurf.SetType((G4SurfaceType)ind);
                return true;
            }
        }
        cerr << "Unknown optical surface type: " << value 
             << " for surface " << opsurf.GetName() << endl;
        return false;
    }
    
    if (name == "finish") {
        // Must match G4OpticalSurfaceFinish
        const char* finishes[] = { 
            "polished", "polishedfrontpainted", "polishedbackpainted",
            "ground", "groundfrontpainted", "groundbackpainted",
            "polishedlumirrorair", "polishedlumirrorglue", 
            "polishedair", "polishedteflonair", "polishedtioair", "polishedtyvekair",      
            "polishedvm2000air", "polishedvm2000glue", "etchedlumirrorair",     
            "etchedlumirrorglue", "etchedair", "etchedteflonair",       
            "etchedtioair", "etchedtyvekair", "etchedvm2000air", "etchedvm2000glue",     
            "groundlumirrorair", "groundlumirrorglue", "groundair",            
            "groundteflonair", "groundtioair", "groundtyvekair",       
            "groundvm2000air", "groundvm2000glue", 0 };
        for (int ind=0; finishes[ind]; ++ind) {
            if (value == finishes[ind]) {
                opsurf.SetFinish((G4OpticalSurfaceFinish)ind);
                return true;
            }
        }
        cerr << "Unknown optical surface finish:  " << value
             << " for surface " << opsurf.GetName() << endl;
        return false;
    }

    if (name == "polish") {
        opsurf.SetPolish(atof(value.c_str()));
    }

    if (name == "sigmaalpha") {
        opsurf.SetSigmaAlpha(atof(value.c_str()));
    }


    cerr << "Unknown optical surface parameter:  " << name 
         << " = " << value << endl;
    return false;
}

bool Cowbells::SetOpSurfProperty(G4OpticalSurface& opsurf, TGraph& prop)
{
    string pname = prop.GetName();

    // fixme: could check better if property is consistent with model and type
    {
        const char* known[] = {
            "RINDEX","REALRINDEX","IMAGINARYRINDEX",
            "REFLECTIVITY","EFFICIENCY","TRANSMITTANCE",
            "SPECULARLOBECONSTANT","SPECULARSPIKECONSTANT","BACKSCATTERCONSTANT",
            0 };
        int ind=0;
        while (true) {
            if (!known[ind]) {
                cerr << "Unknown optical surface property " << pname 
                     << " for surface " << opsurf.GetName() << endl;
                return false;
            }
            if (pname == known[ind]) break;
            ++ind;
        }
    }
    

    G4MaterialPropertiesTable* mattab = opsurf.GetMaterialPropertiesTable();
    if (!mattab) {
        mattab = new G4MaterialPropertiesTable();
        opsurf.SetMaterialPropertiesTable(mattab);
    }

    mattab->AddProperty(prop.GetName(), prop.GetX(), prop.GetY(), prop.GetN());
    return true;
}

bool Cowbells::SetLogicalSurface(G4OpticalSurface& opsurf, 
                                              std::string first, std::string second)
{
    string surfname = opsurf.GetName();

    if (first == "") {
        cerr << "No first volume specified for optical surface " << surfname << endl;
        return false;
    }
        
    if (second == "") {
        G4LogicalVolume* lv = G4LogicalVolumeStore::GetInstance()->GetVolume(first.c_str());
        if (!lv) {
            cerr << "No logical volume " << first << endl;
            return false;
        }
        new G4LogicalSkinSurface(surfname.c_str(), lv, &opsurf);
        cout << "G4LogicalSkinSurface(\"" << surfname << "\",\""<<first<<"\")" << endl;
        return true;
    }

    G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();

    G4VPhysicalVolume* pv1 = pvs->GetVolume(first.c_str());
    if (!pv1) {
        cerr << "No first physical volume: " << first << endl;
        return false;
    }

    G4VPhysicalVolume* pv2 = pvs->GetVolume(second.c_str());
    if (!pv2) {
        cerr << "No second physical volume: " << second << endl;
        return false;
    }

    // intentional leak
    new G4LogicalBorderSurface(surfname, pv1, pv2, &opsurf);
    cout << "G4LogicalBorderSurface(\"" << surfname 
         << "\",\""<<first << "\",\""<<second <<"\")"  << endl;

    return true;
}
