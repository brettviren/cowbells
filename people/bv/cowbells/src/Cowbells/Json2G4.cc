#include "Cowbells/Json2G4.h"
#include "Cowbells/JsonUtil.h"
#include "Cowbells/SensitiveDetector.h"

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

#include <stdexcept>
#include <iostream>
using namespace std;

using Cowbells::get_int;
using Cowbells::get_num;


G4LogicalVolume* Cowbells::get_LogicalVolume(Json::Value val, G4LogicalVolume* def)
{
    if (val.isNull()) { return 0; }
    G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
    return lvs->GetVolume(val.asString(), false);
}

G4ThreeVector Cowbells::get_ThreeVector(Json::Value pos, G4ThreeVector def)
{
    if (pos.isNull()) { return def; }
    return G4ThreeVector(pos[0].asFloat(),pos[1].asFloat(),pos[2].asFloat());
}

G4RotationMatrix* Cowbells::get_RotationMatrix(Json::Value val, G4RotationMatrix* def)
{
    if (val.isNull()) { return 0; }

    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateX(degree * get_num(val["rotatex"]));
    rot->rotateY(degree * get_num(val["rotatey"]));
    rot->rotateZ(degree * get_num(val["rotatez"]));
    return rot;
}



Cowbells::Json2G4::Json2G4(FileList files)
    : m_files(files)
    , m_world(0)
{
    this->read();
}

Cowbells::Json2G4::~Json2G4()
{
}

Json::Value Cowbells::Json2G4::get(std::string path)
{
    return Cowbells::json_get_fitting(m_roots, path);
}

void Cowbells::Json2G4::read() 
{
    if (m_roots.size() == m_files.size()) { return; }

    m_roots.clear();
    for (size_t ind=0; ind < m_files.size(); ++ind) {
        Json::Value root = Cowbells::json_parse_file(m_files[ind]); // may throw
        m_roots.push_back(root);
        cerr << "Read config file #" << ind+1 << ": " << m_files[ind] << endl;
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

int Cowbells::Json2G4::elements(Json::Value eles)
{
    int neles = eles.size();
    for (int ind = 0; ind<neles; ++ind) {
        Json::Value ele = eles[ind];

        string name = ele["name"].asString();
        string symbol = ele["symbol"].asString();
        G4Element* g4ele = GetElementBySymbol(symbol, false);
        if (g4ele) { 
            cerr << "Element " << name << " already defined" << endl;
            continue;
        }

        g4ele = new G4Element(name, symbol,
                              ele["z"].asInt(),
                              ele["a"].asFloat() *g/mole);
        //cerr << "Element added: " << symbol << ": "
        //     << ele.toStyledString() << endl;
    }
    return neles;
}

int Cowbells::Json2G4::materials(Json::Value mats)
{
    int nmats = mats.size();
    for (int ind=0; ind<nmats; ++ind) {
        Json::Value mat = mats[ind];
        string name = mat["name"].asString();

        G4Material* g4mat = G4Material::GetMaterial(name, false);
        if (g4mat) {
            cerr << "Material " << name << " already defined" << endl;
            continue;
        }

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

        //cerr << "Material added: " << name << ": " << mat.toStyledString() << endl;
    }
    return nmats;
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

int Cowbells::Json2G4::optical(Json::Value props)
{
    const G4MaterialTable& mattab = *G4Material::GetMaterialTable();

    int nprops = props.size();
    for (int iprop = 0; iprop < nprops; ++iprop) {

        Json::Value prop = props[iprop];
        string matname = prop["matname"].asString();

        G4Material* mat = get_mat(mattab, matname);
        G4MaterialPropertiesTable* mpt = mat->GetMaterialPropertiesTable();
        if (!mpt) {
            mpt = new G4MaterialPropertiesTable();
            mat->SetMaterialPropertiesTable(mpt);
        }

        string propname = prop["propname"].asString();

        int npoints = prop["x"].size();
        
        // scalar
        if (npoints == 1) { 
            double propval = prop["y"][0].asFloat();
            mpt->AddConstProperty(propname.c_str(), propval);
            //cout << "Set " << matname << "/" << propname
            //     << "[" << npoints << "] = " << propval << endl;
            continue;
        }

        // vector
        double *x = new double[npoints];
        double *y = new double[npoints];
        for (int ind=0; ind<npoints; ++ind) {
            x[ind] = prop["x"][ind].asFloat();
            y[ind] = prop["y"][ind].asFloat();
        }
        mpt->AddProperty(propname.c_str(), x, y, npoints);
        //cout << "Set " << matname << "/" << propname
        //     << "[" << npoints << "] : (" << y[0] << " - "
        //     << y[npoints-1] << ")" << endl;
        delete [] x;
        delete [] y;
    }
    return nprops;
}

// Make a G4VSolid based on shape data in "v"
G4VSolid* make_solid(Json::Value v)
{
    string type = v["type"].asString();
    string name = v["name"].asString();
    
    if (type == "box") {
        return new G4Box(name, get_num(v["x"]), get_num(v["y"]), get_num(v["z"]));
    }
    if (type == "tubs") {
        return new G4Tubs(name, get_num(v["rmin"]), get_num(v["rmax"]),
                          get_num(v["dz"]),get_num(v["sphi"]), get_num(v["dphi"], 360*degree));
    }
    if (type == "polycone") {
        if (!v["planes"].isNull()) {
            int nplanes = v["planes"].size();
            double *zplane = new double[nplanes];
            double *rinner = new double[nplanes];
            double *router = new double[nplanes];
            
            for (int ind=0; ind<nplanes; ++ind) {
                Json::Value plane = v["planes"][ind];
                zplane[ind] = plane["zplane"][ind].asFloat();
                rinner[ind] = plane["rinner"][ind].asFloat();
                router[ind] = plane["router"][ind].asFloat();
            }
            G4Polycone* pc =  new G4Polycone(name, get_num(v["phistart"]), 
                                             get_num(v["phitotal"], 360*degree), nplanes,
                                             zplane, rinner, router);
            delete [] zplane;
            delete [] rinner;
            delete [] router;
            return pc;
        }
        if (!v["rz"].isNull()) {
            int nrzs = v["rz"].size();
            double *r = new double[nrzs];
            double *z = new double[nrzs];

            for (int ind=0; ind<nrzs; ++ind) {
                Json::Value rz = v["rz"][ind];
                r[ind] = rz["r"].asFloat();
                z[ind] = rz["z"].asFloat();
            }
            G4Polycone* pc = new G4Polycone(name, get_num(v["phistart"]), 
                                            get_num(v["phitotal"], 360*degree), nrzs, r, z);
            delete [] r;
            delete [] z;
            return pc;
        }
        return 0;
    }
    cerr << "Failed to make solid of type \"" << type << "\" named \"" << name << "\"" << endl;
    return 0;
}


int Cowbells::Json2G4::volumes(Json::Value vols)
{
    int nvols = vols.size();
    for (int ivol=0; ivol < nvols; ++ivol) {
        Json::Value vol = vols[ivol];

        string matname = vol["material"].asString();
        G4Material* mat = G4Material::GetMaterial(matname, false);

        string lvname = vol["name"].asString();
        G4VSolid* solid = make_solid(vol["shape"]);
        new G4LogicalVolume(solid, mat, lvname);
    }
    return nvols;
}

int Cowbells::Json2G4::placements(Json::Value placed)
{
    int nplaced = placed.size();
    for (int ind = 0; ind < nplaced; ++ind) {
        Json::Value pl = placed[ind];

        G4LogicalVolume *lvmother = get_LogicalVolume(pl["mother"]);
        G4LogicalVolume *lvdaughter = get_LogicalVolume(pl["daughter"]);

        G4RotationMatrix* rot = get_RotationMatrix(pl["rot"]);
        G4ThreeVector pos = get_ThreeVector(pl["pos"]);

        G4VPhysicalVolume* pv = 
            new G4PVPlacement(rot, pos, lvdaughter, pl["name"].asString(),
                              lvmother, false, get_int(pl["copy"]));

        if (!lvmother) {        // no belly button
            if (m_world) {
                cerr << "Warning: replacing world volume: " << m_world->GetName() 
                     << " with: " << pv->GetName() << endl;
                m_world = pv;
            }
        }
    }
    return nplaced;
}

static void set_model(G4OpticalSurface& opsurf, string model)
{
    const char* models[] = { "glisur", "unified", "LUT", 0 };
    for (int ind=0; models[ind]; ++ind) {
        if (model == models[ind]) {
            opsurf.SetModel((G4OpticalSurfaceModel)ind);
            return;
        }
    }
    cerr << "Unknown optical surface model: " << model
         << " for surface " << opsurf.GetName() << endl;
}

static void set_type(G4OpticalSurface& opsurf, string type)
{
    const char* types[] = { "dielectric_metal", "dielectric_dielectric",
                            "dielectric_LUT", "firsov", "x_ray", 0 };
    for (int ind=0; types[ind]; ++ind) {
        if (type == types[ind]) {
            opsurf.SetType((G4SurfaceType)ind);
            return;
        }
    }
    cerr << "Unknown optical surface type: " << type
         << " for surface " << opsurf.GetName() << endl;
    return;
}

static void set_finish(G4OpticalSurface& opsurf, string finish)
{
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
        if (finish == finishes[ind]) {
            opsurf.SetFinish((G4OpticalSurfaceFinish)ind);
            return;
        }
    }
    cerr << "Unknown optical surface finish:  " << finish
         << " for surface " << opsurf.GetName() << endl;
}


static void surface_parameters(G4OpticalSurface& opsurf, Json::Value params)
{
    { 
        Json::Value val = params["model"];
        if (!val.isNull()) {
            set_model(opsurf, val.asString());
        }
    }
    {
        Json::Value val = params["type"];
        if (!val.isNull()) {
            set_type(opsurf, val.asString());
        }
    }
    {
        Json::Value val = params["finish"];
        if (!val.isNull()) {
            set_finish(opsurf, val.asString());
        }
    }
    {
        Json::Value val = params["polish"];
        if (!val.isNull()) {
            opsurf.SetPolish(val.asFloat());
        }
    }
    {
        Json::Value val = params["sigmaalpha"];
        if (!val.isNull()) {
            opsurf.SetSigmaAlpha(val.asFloat());
        }
    }
}
static void surface_property(G4OpticalSurface& opsurf, string propname, Json::Value prop)
{
    // fixme: could check better if property is consistent with model and type
    const char* known[] = {
        "RINDEX","REALRINDEX","IMAGINARYRINDEX",
        "REFLECTIVITY","EFFICIENCY","TRANSMITTANCE",
        "SPECULARLOBECONSTANT","SPECULARSPIKECONSTANT","BACKSCATTERCONSTANT",
        0 };
    int ind=0;
    while (true) {
        if (!known[ind]) {
            cerr << "Unknown optical surface property " << propname
                 << " for surface " << opsurf.GetName() << endl;
            return;
        }
        if (propname == known[ind]) break;
        ++ind;
    }

    G4MaterialPropertiesTable* mattab = opsurf.GetMaterialPropertiesTable();
    if (!mattab) {
        mattab = new G4MaterialPropertiesTable();
        opsurf.SetMaterialPropertiesTable(mattab);
    }

    int npoints = prop["x"].size();
    double *x = new double[npoints];
    double *y = new double[npoints];
    for (int ind=0; ind<npoints; ++ind) {
        x[ind] = prop["x"][ind].asFloat();
        y[ind] = prop["y"][ind].asFloat();
    }
    mattab->AddProperty(propname.c_str(), x, y, npoints);
    delete [] x;
    delete [] y;
}

static void make_surface(Json::Value surf)
{
    if (surf["name"].isNull()) {
        cerr << "Got badly formed surface data: " << surf.toStyledString() << endl;
    }
    string surfname = surf["name"].asString();
    G4OpticalSurface* opsurf = new G4OpticalSurface(surfname.c_str());

    Json::Value params = surf["parameters"];
    surface_parameters(*opsurf, params);

    Json::Value props = surf["properties"];
    Json::ValueIterator pit = props.begin();
    for (size_t ind=0; ind<props.size(); ++ind, ++pit) {
        string propname = pit.key().asString();
        surface_property(*opsurf, propname, *pit);
    }
}

int Cowbells::Json2G4::surfaces(Json::Value surfs)
{
    int nsurfs = surfs.size();
    for (int isurf = 0; isurf < nsurfs; ++isurf) {
        Json::Value surf = surfs[isurf];
        if (surf.isNull()) {
            cerr << "Surface #" << isurf << " is null!?" << endl;
            continue;
        }
        make_surface(surf);
    }
    return nsurfs;
}

int Cowbells::Json2G4::sensitive(Json::Value sens)
{
    G4SDManager* sdm = G4SDManager::GetSDMpointer();

    int nsens = sens.size();
    for (int ind=0; ind<nsens; ++ind) {
        Json::Value sdv = sens[ind];

        string sdname = sdv["name"].asString();
        string hcname = sdv["hcname"].asString();
        string lvname = sdv["logvol"].asString();

        Json::Value touchables = sdv["touchables"];
        int ntouchables = touchables.size();
        vector<string> tv;
        for (int itouch=0; itouch<ntouchables; ++itouch) {
            tv.push_back(touchables[itouch].asString());
        }
        
        Cowbells::SensitiveDetector* csd = new Cowbells::SensitiveDetector(sdname, hcname, tv);
        sdm->AddNewDetector(csd);
        G4LogicalVolume* lv = G4LogicalVolumeStore::GetInstance()->GetVolume(lvname);
        if (!lv) {
            cerr << "No LV for " << lvname << " for sensitive detector " << sdname << endl;
            assert (lv);
        }
        lv->SetSensitiveDetector(csd);
        //cout << "Registered SD \"" << csd->GetName() 
        //     << "\" with logical volume \"" << lvname << "\"" << endl;
    }
    return nsens;
}

G4VPhysicalVolume* Cowbells::Json2G4::construct_detector()
{
    string parts[] = {
        "elements", "materials", "optical", "volumes",
        "placements", "surfaces", "sensitive", "",
    };

    for (size_t iroot = 0; iroot<m_roots.size(); ++iroot) {
        Json::Value root = m_roots[iroot];
        for (int ipart=0; parts[ipart].size(); ++ipart) {
            Json::Value val = root[parts[ipart]];
            if (val.isNull()) { continue; }

            cerr << "Loading section \"" << parts[ipart] 
                 << "\" from file \"" << m_files[iroot] << "\"" << endl;

            int nmade = 0;
            try {
                if (parts[ipart] == "elements")   nmade = this->elements(val);
                if (parts[ipart] == "materials")  nmade = this->materials(val);
                if (parts[ipart] == "optical")    nmade = this->optical(val);
                if (parts[ipart] == "volumes")    nmade = this->volumes(val);
                if (parts[ipart] == "placements") nmade = this->placements(val);
                if (parts[ipart] == "surfaces")   nmade = this->surfaces(val);
                if (parts[ipart] == "sensitive")  nmade = this->sensitive(val);
            }
            catch (const runtime_error& re) {
                cerr << "Failed: " << re.what() << endl;
                throw;
            }
            cerr << "\tloaded " << nmade << endl;
        }
    }
    return m_world;
}
