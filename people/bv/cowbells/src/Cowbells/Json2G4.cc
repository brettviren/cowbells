#include "Cowbells/Json2G4.h"
#include "Cowbells/JsonUtil.h"

#include <G4VPhysicalVolume.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4MaterialTable.hh>
#include <G4Material.hh>
#include <G4Tubs.hh>
#include <G4Polycone.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>
#include <G4RotationMatrix.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SDManager.hh>

#include <iostream>
using namespace std;


Cowbells::Json2G4::Json2G4(FileList files)
    : m_files(files)
{
    this->read();
}

Cowbells::Json2G4::~Json2G4()
{
}

void Cowbells::Json2G4::read() 
{
    if (m_roots.size() == m_files.size()) { return; }

    m_roots.clear();
    for (size_t ind=0; ind < m_files.size(); ++ind) {
        Json::Value root = Cowbells::json_parse_file(m_files[ind]); // may throw
        m_roots.push_back(root);
    }
}


static G4Element* GetElementBySymbol(string symbol, bool warn = true);
static G4Element* GetElementBySymbol(string symbol, bool warn)
{
    const G4ElementTable& et = *G4Element::GetElementTable();
    for (size_t ind = 0; ind < et.size(); ++ind) {
        G4Element* ele = et[ind];
        string symname = ele->GetSymbol();
        if (symname == symbol) {
            return ele;
        }
    }
    if (warn) {
        cerr << "Failed to find element with symbol " << symbol << endl;
    }
    return 0;
}

void Cowbells::Json2G4::elements(Json::Value eles)
{
    int neles = eles.size();
    Json::ValueIterator it = eles.begin(); 
    for (int count = 0; count<neles; ++count, ++it) {
        string symbol = it.key().asString();
        Json::Value ele = (*it);
        string name = ele["name"].asString();
        
        G4Element* g4ele = GetElementBySymbol(symbol, false);
        if (g4ele) { 
            cerr << "Element " << name << " already defined" << endl;
            continue;
        }

        g4ele = new G4Element(name, symbol,
                              ele["z"].asInt(),
                              ele["a"].asFloat() *g/mole);
        cerr << "Element added: " << symbol << ": "
             << (*it).toStyledString() << endl;
    }
}

void Cowbells::Json2G4::materials(Json::Value mats)
{
    int nmats = mats.size();
    Json::ValueIterator it = mats.begin();
    for (int count=0; count<nmats; ++count, ++it) {
        string name = it.key().asString();

        G4Material* g4mat = G4Material::GetMaterial(name, false);
        if (g4mat) {
            cerr << "Material " << name << " already defined" << endl;
            continue;
        }

        Json::Value mat = (*it);
        Json::Value eles = mat["elements"];
        int neles = eles.size();
        g4mat = new G4Material(name, mat["density"].asFloat(), neles);


        Json::ValueIterator eit = eles.begin();
        for (int iele=0; iele<neles; ++iele, ++eit) {
            Json::Value quant = *eit;
            string symbol = eit.key().asString();
            G4Element* g4ele = GetElementBySymbol(symbol);
            if (quant.isInt()) {
                g4mat->AddElement(g4ele, quant.asInt());
            }
            else {
                g4mat->AddElement(g4ele, quant.asFloat());
            }
        }

        cerr << "Material added: " << name << ": " << (*it).toStyledString() << endl;
    }
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

void Cowbells::Json2G4::optical(Json::Value props)
{
    const G4MaterialTable& mattab = *G4Material::GetMaterialTable();

    int nprops = props.size();
    Json::ValueIterator it = props.begin();
    for (int count = 0; count < nprops; ++count, ++it) {
        string matname = it.key().asString();

        G4Material* mat = get_mat(mattab, matname);
        G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
        mat->SetMaterialPropertiesTable(mpt);

        Json::Value matprop = (*it);

        int nmats = matprop.size();
        Json::ValueIterator mit = matprop.begin();
        for (int imat=0; imat<nmats; ++imat, ++mit) {
            string propname = mit.key().asString();
            Json::Value prop = (*mit);

            int n = prop["x"].size();

            if (n == 1) { // scalar
                double propval = prop["y"][0].asFloat();
                mpt->AddConstProperty(propname.c_str(), propval);
                cout << "Set " << matname << "/" << propname
                     << "[" << n << "] = " << propval << endl;
                continue;
            }

            double *x = new double[n];
            double *y = new double[n];
            for (int i=0; i<n; ++i) {
                x[i] = prop["x"][i].asFloat();
                y[i] = prop["y"][i].asFloat();
            }
            mpt->AddProperty(propname.c_str(), x,y,n);
            cout << "Set " << matname << "/" << propname
                 << "[" << n << "] : (" << y[0] << " - "
                 << y[n-1] << ")" << endl;
            delete [] x;
            delete [] y;
        }
    }
}

void Cowbells::Json2G4::volumes(Json::Value v)
{
}

void Cowbells::Json2G4::placements(Json::Value v)
{
}

void Cowbells::Json2G4::surfaces(Json::Value surfs)
{
    int nsurfs = surfs.size();
    Json::ValueIterator it = surfs.begin();
    for (int count = 0; count < nsurfs; ++count, ++it) {
        string surfname = it.key().asString();

        G4OpticalSurface* opsurf = new G4OpticalSurface(surfname.c_str());
    }
}

void Cowbells::Json2G4::sensitive(Json::Value v)
{
}

void Cowbells::Json2G4::make()
{
    string parts[] = {
        "elements", "materials", "optical", "volumes",
        "placements", "surfaces", "sensitive", "",
    };

    for (size_t ind = 0; ind<m_roots.size(); ++ind) {
        Json::Value root = m_roots[ind];
        for (int ind=0; parts[ind].size(); ++ind) {
            Json::Value val = root[parts[ind]];
            if (val.isNull()) { continue; }
            if (parts[ind] == "elements")   this->elements(val);
            if (parts[ind] == "materials")  this->materials(val);
            if (parts[ind] == "optical")    this->optical(val);
            if (parts[ind] == "volumes")    this->volumes(val);
            if (parts[ind] == "placements") this->placements(val);
            if (parts[ind] == "surfaces")   this->surfaces(val);
            if (parts[ind] == "sensitive")  this->sensitive(val);
        }
    }

}
