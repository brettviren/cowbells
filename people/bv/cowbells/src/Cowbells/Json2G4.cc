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
#include <G4RotationMatrix.hh>
#include <stdexcept>

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
        cerr << "Element added: " << symbol << ": "
             << ele.toStyledString() << endl;
    }
}

void Cowbells::Json2G4::materials(Json::Value mats)
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

        cerr << "Material added: " << name << ": " << mat.toStyledString() << endl;
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
            cout << "Set " << matname << "/" << propname
                 << "[" << npoints << "] = " << propval << endl;
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
        cout << "Set " << matname << "/" << propname
             << "[" << npoints << "] : (" << y[0] << " - "
             << y[npoints-1] << ")" << endl;
        delete [] x;
        delete [] y;
    }
}

float get_num(Json::Value val, float def=0.0);
float get_num(Json::Value val, float def)
{
    if (val.isNull()) { return def; }
    return val.asFloat();
}

float get_int(Json::Value val, int def=0);
float get_int(Json::Value val, int def)
{
    if (val.isNull()) { return def; }
    return val.asInt();
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


void Cowbells::Json2G4::volumes(Json::Value vols)
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
}

static G4RotationMatrix* get_rotation(Json::Value val)
{
    if (val.isNull()) { return 0; }

    G4RotationMatrix* rot = new G4RotationMatrix();
    rot->rotateX(degree * get_num(val["rotatex"]));
    rot->rotateY(degree * get_num(val["rotatey"]));
    rot->rotateZ(degree * get_num(val["rotatez"]));
    return rot;
}

static G4ThreeVector get_position(Json::Value pos)
{
    if (pos.isNull()) { return G4ThreeVector(); }
    return G4ThreeVector(get_num(pos["x"]), get_num(pos["y"]), get_num(pos["z"]));
}

G4LogicalVolume* get_lv(Json::Value val)
{
    if (val.isNull()) { return 0; }
    G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
    return lvs->GetVolume(val.asString(), false);
}

void Cowbells::Json2G4::placements(Json::Value placed)
{
    int nplaced = placed.size();
    for (int ind = 0; ind < nplaced; ++ind) {
        Json::Value pl = placed[ind];

        G4LogicalVolume *lvmother = get_lv(pl["mother"]);
        G4LogicalVolume *lvdaughter = get_lv(pl["daughter"]);

        G4RotationMatrix* rot = get_rotation(pl["rot"]);
        G4ThreeVector pos = get_position(pl["pos"]);

        new G4PVPlacement(rot, pos, lvdaughter, pl["name"].asString(),
                          lvmother, false, get_int(pl["copy"]));
    }
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

void Cowbells::Json2G4::surfaces(Json::Value surfs)
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

    for (size_t iroot = 0; iroot<m_roots.size(); ++iroot) {
        Json::Value root = m_roots[iroot];
        for (int ipart=0; parts[ipart].size(); ++ipart) {
            Json::Value val = root[parts[ipart]];
            if (val.isNull()) { continue; }

            cerr << "Loading section \"" << parts[ipart] 
                 << "\" from file \"" << m_files[iroot] << "\"" << endl;

            try {
                if (parts[ipart] == "elements")   this->elements(val);
                if (parts[ipart] == "materials")  this->materials(val);
                if (parts[ipart] == "optical")    this->optical(val);
                if (parts[ipart] == "volumes")    this->volumes(val);
                if (parts[ipart] == "placements") this->placements(val);
                if (parts[ipart] == "surfaces")   this->surfaces(val);
                if (parts[ipart] == "sensitive")  this->sensitive(val);
            }
            catch (const runtime_error& re) {
                cerr << "Failed: " << re.what() << endl;
                throw;
            }
        }
    }
}
