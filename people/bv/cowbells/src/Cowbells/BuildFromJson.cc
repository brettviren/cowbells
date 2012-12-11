#include "Cowbells/BuildFromJson.h"
#include "Cowbells/JsonUtil.h"
#include "Cowbells/strutil.h"
#include "Cowbells/Util.h"
#include "Cowbells/SensitiveDetector.h"

#include <G4VPhysicalVolume.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4MaterialTable.hh>
#include <G4Material.hh>
#include <G4Tubs.hh>
#include <G4Polycone.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>
#include <G4RotationMatrix.hh>

// for registering sensitive detectors
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SDManager.hh>

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

using namespace std;

Cowbells::BuildFromJson::BuildFromJson()
{
}

Cowbells::BuildFromJson::~BuildFromJson()
{
}


void Cowbells::BuildFromJson::addfile(std::string filename)
{
    m_config_files.push_back(filename);
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


void Cowbells::BuildFromJson::MakeElements(Json::Value eles)
{
    int neles = eles.size();
    Json::ValueIterator it = eles.begin(); 
    for (int count = 0; count<neles; ++count, ++it) {
        string symbol = it.key().asString();
        Json::Value ele = (*it);
        string name = ele["name"].asString();
        
        G4Element* g4ele = GetElementBySymbol(symbol);
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

void Cowbells::BuildFromJson::MakeMaterials(Json::Value mats)
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

// get json object value
Json::Value Cowbells::BuildFromJson::gjov(string path)
{
    Json::Value ret = Cowbells::json_get_fitting(m_roots, path);
    if (ret.isNull()) {
        cerr << "Failed to find configuration item \"" << path << "\"" << endl;
        throw invalid_argument("bad configuration");
    }
    return ret;
}

double Cowbells::BuildFromJson::asDistance(string path)
{
    Json::Value val = gjov(path);

    double num = val["value"].asFloat();
    string unit = val["unit"].asString();
    if (unit == "m" || unit == "meter") {
        return num*m;
    }
    if (unit == "cm") {
        return num*cm;
    }
    if (unit == "mm") {
        return num*mm;
    }
    if (unit == "inch") {
        return num*2.54*cm;
    }
    cerr << "Unknown unit: \"" << unit << "\" for \"" << path << "\"" << endl;
    return 0;
}

string Cowbells::BuildFromJson::asString(string path)
{
    Json::Value val = gjov(path);
    return val.asString();
}

bool Cowbells::BuildFromJson::asBool(string path)
{
    Json::Value val = gjov(path);
    return val.asBool();
}



G4LogicalVolume* Cowbells::BuildFromJson::MakeTubDet(std::string mat_sample_name,
                                                     std::string mat_tub_name)
{
    double sample_radius = asDistance("detector/tubdets/sample_radius");
    double sample_height = asDistance("detector/tubdets/sample_height");
    double tubdet_wall_thickness = asDistance("detector/tubdets/wall_thickness");
    double tubdet_lid_thickness = asDistance("detector/tubdets/lid_thickness");

    // tub
    G4LogicalVolume* lv_tub = 0;
    {
        G4Material* mat = G4Material::GetMaterial(mat_tub_name);    
        std::string shape_name = "shape" + mat_tub_name;
        shape_name += "Tub";
        double rad = sample_radius + tubdet_wall_thickness;
        double height = sample_height + tubdet_wall_thickness + tubdet_lid_thickness;
        G4Tubs* shape = new G4Tubs(shape_name.c_str(), 0, rad, 0.5*height, 0, 2*M_PI);
        std::string lv_name = "lv" + mat_tub_name;
        lv_name += "Tub";
        lv_tub = new G4LogicalVolume(shape, mat, lv_name.c_str(), 0,0,0);
    }

    // sample
    {
        G4LogicalVolume* lv_samp = 0;
        G4Material* mat = G4Material::GetMaterial(mat_sample_name);
        std::string shape_name = "shape" + mat_sample_name;
        shape_name += "Sample";
        double rad = sample_radius;
        double height = sample_height;
        double offset = -0.5*(tubdet_lid_thickness - tubdet_wall_thickness);

        G4Tubs* shape = new G4Tubs(shape_name.c_str(), 0, rad, 0.5*height, 0, 2*M_PI);

        std::string lv_name = "lv" + mat_sample_name;
        lv_name += "Sample";
        lv_samp = new G4LogicalVolume(shape, mat, lv_name.c_str(), 0,0,0);
        
        std::string pv_name = "Sample" + mat_sample_name;
        new G4PVPlacement(0,G4ThreeVector(0,0,offset), lv_samp, pv_name.c_str(),
                          lv_tub, false, 0);
    }

    // Window - shared by all tub dets
    std::string lv_tubdet_window_name = "lvTubdetWindow";
    G4LogicalVolume* lv_window 
        = G4LogicalVolumeStore::GetInstance()->GetVolume(lv_tubdet_window_name, false);
    if (!lv_window) {

        // window
        {
            string matname = asString("detector/tubdets/window_material");
            G4Material* mat = G4Material::GetMaterial(matname);
            std::string shape_name = "shapeTubdetWindow";
            double full_rad = asDistance("detector/tubdets/window_full_radius");
            double step_rad = asDistance("detector/tubdets/window_step_radius");
            double thick = asDistance("detector/tubdets/window_thickness");
            double step_z = gjov("detector/tubdets/window_step_fraction").asFloat();
            cerr << "step_z = " << step_z << endl;
            step_z *= thick;
            double z_planes[4] = { 0.0, step_z, step_z, thick };
            double r_min[4] = { 0.0, 0.0, 0.0, 0.0 };
            double r_max[4] = { step_rad, step_rad, full_rad, full_rad };

            G4Polycone * shape
                = new G4Polycone(shape_name.c_str(), 0, 2*M_PI, 4, z_planes, r_min, r_max);
            
            lv_window 
                = new G4LogicalVolume(shape, mat, lv_tubdet_window_name.c_str(), 0,0,0);
        }

        // pc in window
        {
            string matname = asString("detector/tubdets/pc_material");
            G4Material* mat = G4Material::GetMaterial(matname);
            std::string shape_name = "shapePC";
            double rad = asDistance("detector/tubdets/pc_radius");
            double height = asDistance("detector/tubdets/pc_thickness");
            double offset = tubdet_lid_thickness - 0.5*height;

            G4Tubs* shape = new G4Tubs(shape_name.c_str(), 0, rad, 0.5*height, 0, 2*M_PI);
            
            G4LogicalVolume* lv = new G4LogicalVolume(shape, mat, "lvTubPC", 0,0,0);
            new G4PVPlacement(0,G4ThreeVector(0,0,offset), lv, "PC", 
                              lv_window, false, 0);
        }
    }
    
    { // place window in lid
        double sample_offset = 0.5*(tubdet_lid_thickness - tubdet_wall_thickness);
        double offset = 0.5*sample_height - sample_offset - 0.034;

        new G4PVPlacement(0,G4ThreeVector(0,0,offset), lv_window, "TubdetWindow",
                          lv_tub, false, 0);
    }

    return lv_tub;
}


G4VPhysicalVolume* Cowbells::BuildFromJson::MakeGeometry()
{

    // world
    G4VPhysicalVolume* world_pv = 0;
    {
        double size = asDistance("detector/world/size");
        string matname = asString("detector/world/material");
        G4Material* mat = G4Material::GetMaterial(matname);

        G4Box* shape = new G4Box("shapeWorld", size,size,size);
        G4LogicalVolume* lv
            = new G4LogicalVolume(shape, mat, "lvWorld", 0,0,0);
        G4VPhysicalVolume* pv
            = new G4PVPlacement(0,G4ThreeVector(),lv,"World",0,false,0);
        world_pv = pv;
    }

    
    // beam window
    if (! gjov("detector/beam_window").isNull()) {

        double rad = asDistance("detector/beam_window/radius");
        double thick = asDistance("detector/beam_window/thickness");
        double zoff = asDistance("detector/beam_window/zoff");
        string matname = asString("detector/beam_window/material");
        G4Material* mat = G4Material::GetMaterial(matname);

        G4Tubs* shape = new G4Tubs("shapeBeamWindow",0,rad,0.5*thick,0,2*M_PI);

        G4LogicalVolume* lv 
            = new G4LogicalVolume(shape, mat, "lvBeamWindow", 0,0,0);
        
        new G4PVPlacement(0,G4ThreeVector(0,0,zoff),lv,"BeamWindow",
                          world_pv->GetLogicalVolume(),false,0);
    }
            
                                                  
    // tc = trigger counters
    if (! gjov("detector/trigger_counters").isNull()) {
        double width = asDistance("detector/trigger_counters/width");
        double thick = asDistance("detector/trigger_counters/thickness");
        double pcthick = asDistance("detector/trigger_counters/pc_thickness");
        double zsep = asDistance("detector/trigger_counters/separation");
        double zoff = asDistance("detector/trigger_counters/zoff");
        string pcmatname = asString("detector/trigger_counters/pc_material");
        string scintmatname = asString("detector/trigger_counters/scint_material");
        G4Material* mat_tcpc = G4Material::GetMaterial(pcmatname);
        G4Material* mat_scint = G4Material::GetMaterial(scintmatname);

        G4Box* shape_tcpc 
            = new G4Box("shapeTCPC", 0.5*width+pcthick,0.5*width+pcthick,0.5*thick+pcthick);
        G4Box* shape_scint
            = new G4Box("shapeTCScint", 0.5*width,0.5*width,0.5*thick);

        G4LogicalVolume* lv_tcpc 
            = new G4LogicalVolume(shape_tcpc, mat_tcpc, "lvTCPC", 0,0,0);
        G4LogicalVolume* lv_scint
            = new G4LogicalVolume(shape_scint, mat_scint, "lvTCScint", 0,0,0);

        new G4PVPlacement(0, G4ThreeVector(), lv_scint, "TriggerCounterScint",
                          lv_tcpc, false, 0);

        for (int tcnum = 0; tcnum < 3; ++tcnum) {
            G4ThreeVector pos(0,0,zsep*tcnum + zoff);
            new G4PVPlacement(0, pos, lv_tcpc, "TriggerCounter",
                              world_pv->GetLogicalVolume(), false, tcnum);
        }
    }

    // Tub detectors
    if (! gjov("detector/tubdets").isNull()) {

        string sampmat1 = gjov("detector/samples")[0].asString();
        string sampmat2 = gjov("detector/samples")[1].asString();
        string tubmat1 = gjov("detector/tubdets/tub_materials")[0].asString();
        string tubmat2 = gjov("detector/tubdets/tub_materials")[1].asString();

        G4LogicalVolume* lv_tub1 = MakeTubDet(sampmat1,tubmat1);
        G4LogicalVolume* lv_tub2 = MakeTubDet(sampmat2,tubmat2);

        string tubname1 = "Tub" + tubmat1;
        string tubname2 = "Tub" + tubmat2;

        double zsep = asDistance("detector/tubdets/separation");
        double zoff = asDistance("detector/tubdets/separation");

        G4RotationMatrix* rot = new G4RotationMatrix();
        rot->rotateX(90*deg);
        new G4PVPlacement(rot,G4ThreeVector(0,0,zoff), lv_tub1, tubname1.c_str(),
                          world_pv->GetLogicalVolume(), false, 0);
        new G4PVPlacement(rot,G4ThreeVector(0,0,zoff+zsep), lv_tub2, tubname2.c_str(), 
                          world_pv->GetLogicalVolume(), false, 0);
    }

    return world_pv;
}

void Cowbells::BuildFromJson::MakeSensitive(Json::Value sens)
{
    int nsens = sens.size();
    for (int count=0; count<nsens; ++count) {
        Json::Value sd = sens[count];
        string hcname = sd["hitcollection"].asString();
        string lvname = sd["volume"].asString();
        Json::Value touchables = sd["touchables"];
        vector<string> tv; tv.push_back("");
        for (size_t ntouch = 0; ntouch < touchables.size(); ++ntouch) {
            string tpath = touchables[(int)ntouch].asString();
            tv.push_back(tpath);
        }        

        Cowbells::SensitiveDetector* csd = 
            new Cowbells::SensitiveDetector("SensitiveDetector", hcname.c_str(), tv);

        G4SDManager::GetSDMpointer()->AddNewDetector(csd);
        G4LogicalVolume* lv = G4LogicalVolumeStore::GetInstance()->GetVolume(lvname.c_str());
        if (!lv) {
            cerr << "No LV for " << lvname << endl;
            assert (lv);
        }
        lv->SetSensitiveDetector(csd);
        cout << "Registered SD \"" << csd->GetName() 
             << "\" with logical volume \"" << lvname << "\"" << endl;
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

void Cowbells::BuildFromJson::MakeMaterialProperties(Json::Value props)
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

void Cowbells::BuildFromJson::MakeOpticalSurfaces(Json::Value surfs)
{


}

G4VPhysicalVolume* Cowbells::BuildFromJson::Construct()
{
    for (size_t ind=0; ind < m_config_files.size(); ++ind) {
        Json::Value root = Cowbells::json_parse_file(m_config_files[ind]); // may throw
        m_roots.push_back(root);
    }

    for (size_t ind = 0; ind<m_roots.size(); ++ind) {
        Json::Value root = m_roots[ind];
        Json::Value ele = root["elements"];
        if (ele.isNull()) {
            continue;
        }
        cerr << "Making elements from root #" << ind << endl;
        this->MakeElements(ele);
    }
    for (size_t ind = 0; ind<m_roots.size(); ++ind) {
        Json::Value root = m_roots[ind];
        Json::Value mat = root["materials"];
        if (mat.isNull()) {
            continue;
        }
        cerr << "Making materials from root #" << ind << endl;
        this->MakeMaterials(mat);
    }

    G4VPhysicalVolume* top = this->MakeGeometry();
    Cowbells::dump(top);

    for (size_t ind = 0; ind<m_roots.size(); ++ind) {
        Json::Value root = m_roots[ind];
        Json::Value props = root["matprops"];
        if (props.isNull()) {
            continue;
        }
        cerr << "Making material properties from root #" << ind << endl;
        this->MakeMaterialProperties(props);
    }

    for (size_t ind = 0; ind<m_roots.size(); ++ind) {
        Json::Value root = m_roots[ind];
        Json::Value opsurfs = root["opsurfs"];
        if (opsurfs.isNull()) {
            continue;
        }
        cerr << "Making optical surfaces from root #" << ind << endl;
        this->MakeOpticalSurfaces(opsurfs);
    }
   
    for (size_t ind = 0; ind<m_roots.size(); ++ind) {
        Json::Value root = m_roots[ind];

        Json::Value sens = root["sensitives"];
        if (sens.isNull()) {
            continue;
        }
        cerr << "Making sensitive detectors from root #" << ind << endl;
        this->MakeSensitive(sens);
    }


    return top;
}


        
