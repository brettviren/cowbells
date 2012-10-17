#include "Cowbells/BuildByHand.h"
#include "Cowbells/SensitiveDetector.h"
#include "Cowbells/R2G4.h"
#include "Cowbells/Util.h"

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

#include <vector>

#include <iostream>
using namespace std;

Cowbells::BuildByHand::BuildByHand(std::string filename)
    : m_prop_file(filename)
{
}

Cowbells::BuildByHand::~BuildByHand()
{
  cerr << "Destructing BuildByHande" << endl;
}

namespace Cfg {

    double inch = 2.54 * cm;

    // tubdet 
    double tubdet_thickness = 0.25*inch;
    double tubdet_lid_thickness = 0.75*inch;

    // sample
    double sample_radius = 3.0*inch - tubdet_thickness;
    double sample_height = 6.0*inch;


    // tubdet window
    std::string tubdet_window_material = "Acrylic";
    double tubdet_window_full_radius = 0.5*2.750*inch;
    double tubdet_window_step_radius = 0.5*2.125*inch;
    double tubdet_window_thickness = tubdet_lid_thickness;
    double tubdet_window_step_fraction = 1.0/3.0;

    // pc = photocathode
    std::string tubdet_pc_material = "Acrylic";
    double tubdet_pc_radius = 0.5*2.0*inch;
    double tubdet_pc_thickness = 0.01*inch;

    std::string world_material = "Air";
    double world_size = 10*meter;

    bool make_beam_window = true;
    std::string beam_window_material = "Aluminum";
    double beam_window_radius = 2*cm;        // made up number
    double beam_window_thickness = 0.381*mm; // from TN05-001
    double beam_window_zoffset = -5*m;       // made up number 

    bool make_trigger_counters = true;
    double trigger_counter_width = 2*cm;
    double trigger_counter_thickness = 0.5*cm;
    double trigger_counter_pcthick =  1*mm;
    double trigger_counter_separation = 40*cm;
    double trigger_counter_offset = -20*cm;

    std::string tc_pc_material = "Glass";
    std::string tc_scint_material = "Scintillator";
    bool make_tub_detectors = true;
    std::string sample_material1 = "Water";
    std::string tub_material1 = "Teflon";
    std::string sample_material2 = "Water";
    std::string tub_material2 = "Aluminum";
    double tubdet_separation = 40.0*cm;
    double tubdet_offset = 0.0*cm;
}



G4LogicalVolume* Cowbells::BuildByHand::MakeTubDet(std::string mat_sample_name,
                                                   std::string mat_tub_name)
{
    
    // tub
    G4LogicalVolume* lv_tub = 0;
    {
        G4Material* mat = G4Material::GetMaterial(mat_tub_name);    
        std::string shape_name = "shape" + mat_tub_name;
        shape_name += "Tub";
        double rad = Cfg::sample_radius + Cfg::tubdet_thickness;
        double height = Cfg::sample_height + Cfg::tubdet_thickness + Cfg::tubdet_lid_thickness;
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
        double rad = Cfg::sample_radius;
        double height = Cfg::sample_height;
        double offset = -0.5*(Cfg::tubdet_lid_thickness - Cfg::tubdet_thickness);

        G4Tubs* shape = new G4Tubs(shape_name.c_str(), 0, rad, 0.5*height, 0, 2*M_PI);

        std::string lv_name = "lv" + mat_sample_name;
        lv_name += "Sample";
        lv_samp = new G4LogicalVolume(shape, mat, lv_name.c_str(), 0,0,0);
        
        std::string pv_name = "Sample" + mat_sample_name;
        G4VPhysicalVolume* pv
            = new G4PVPlacement(0,G4ThreeVector(0,0,offset), lv_samp, pv_name.c_str(),
                                lv_tub, false, 0);
        pv = 0;
    }

    // Window - shared by all tub dets
    std::string lv_tubdet_window_name = "lvTubdetWindow";
    G4LogicalVolume* lv_window 
        = G4LogicalVolumeStore::GetInstance()->GetVolume(lv_tubdet_window_name);
    if (!lv_window) {

        // window
        {
            G4Material* mat = G4Material::GetMaterial(Cfg::tubdet_window_material);
            std::string shape_name = "shapeTubdetWindow";
            double full_rad = Cfg::tubdet_window_full_radius;
            double step_rad = Cfg::tubdet_window_step_radius;
            double thick = Cfg::tubdet_window_thickness;
            double step_z = thick * Cfg::tubdet_window_step_fraction;// 1.0/3.0

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
            G4Material* mat = G4Material::GetMaterial(Cfg::tubdet_pc_material);
            std::string shape_name = "shapePC";
            double rad = Cfg::tubdet_pc_radius;
            double height = Cfg::tubdet_pc_thickness;
            double offset = Cfg::tubdet_lid_thickness - 0.5*height;

            G4Tubs* shape = new G4Tubs(shape_name.c_str(), 0, rad, 0.5*height, 0, 2*M_PI);
            
            G4LogicalVolume* lv = new G4LogicalVolume(shape, mat, "lvTubPC", 0,0,0);
            G4VPhysicalVolume* pv
                = new G4PVPlacement(0,G4ThreeVector(0,0,offset), lv, "PC", 
                                    lv_window, false, 0);
            pv = 0;
        }
    }
    
    { // place window in lid
        double sample_offset = 0.5*(Cfg::tubdet_lid_thickness - Cfg::tubdet_thickness);
        double offset = 0.5*Cfg::sample_height - sample_offset;

        G4VPhysicalVolume* pv_window_in_lid
            = new G4PVPlacement(0,G4ThreeVector(0,0,offset), lv_window, "TubdetWindow",
                                lv_tub, false, 0);
        pv_window_in_lid = 0;
    }

    return lv_tub;
}


G4VPhysicalVolume* Cowbells::BuildByHand::Construct()
{
    // Material
    // need: Acrylic Air, Aluminum, Glass, Scintillator, Teflon, Water, WBLS
    G4Element* H = new G4Element("Hydrogen", "H", 1 , 1.01*g/mole);
    G4Element* C = new G4Element("Carbon"  , "C", 6 , 12.01*g/mole);
    G4Element* N = new G4Element("Nitrogen", "N", 7 , 14.01*g/mole);
    G4Element* O = new G4Element("Oxygen"  , "O", 8 , 16.00*g/mole);
    G4Element* F = new G4Element("Florine" , "F", 9 , 19.00*g/mole);
    G4Element* S = new G4Element("Sulfur"  , "S",16 , 32.07*g/mole);

    G4Material* Acrylic = new G4Material("Acrylic", 1.18*g/cm3, 3);
    Acrylic->AddElement(C, 5);
    Acrylic->AddElement(H, 8);
    Acrylic->AddElement(O, 2);

    G4Material* Air = new G4Material("Air", 1.29*mg/cm3, 2);
    Air->AddElement(N, 70.*perCent);
    Air->AddElement(O, 30.*perCent);

    G4Material* Aluminum = new G4Material("Aluminum", 13., 26.98*g/mole, 2.7*g/cm3);

    G4Material* Glass = new G4Material("Glass", 1.032*g/cm3, 2);
    Glass->AddElement(C, 91.533*perCent);
    Glass->AddElement(H,  8.467*perCent);

    G4Material* Scinti = new G4Material("Scintillator", 1.032*g/cm3, 2);
    Scinti->AddElement(C, 9);
    Scinti->AddElement(H, 10);

    G4Material* Teflon = new G4Material("Teflon", 2.2*g/cm3, 2);
    Teflon->AddElement(C,0.759814);
    Teflon->AddElement(F,0.240186);

    G4Material* Water = new G4Material("Water", 1.0*g/cm3, 2);
    Water->AddElement(H, 2);
    Water->AddElement(O, 1);

    G4Material* WbLS = new G4Material("WBLS", 0.9945*g/cm3, 5);
    WbLS->AddElement(H, 0.1097);
    WbLS->AddElement(O, 0.8234);
    WbLS->AddElement(S, 0.0048);
    WbLS->AddElement(N, 0.0001);
    WbLS->AddElement(C, 0.0620);


    // world
    G4VPhysicalVolume* world_pv = 0;
    {
        double size = Cfg::world_size;
        G4Material* mat = G4Material::GetMaterial(Cfg::world_material);

        G4Box* shape = new G4Box("shapeWorld", size,size,size);
        G4LogicalVolume* lv
            = new G4LogicalVolume(shape, mat, "lvWorld", 0,0,0);
        G4VPhysicalVolume* pv
            = new G4PVPlacement(0,G4ThreeVector(),lv,"World",0,false,0);
        world_pv = pv;
    }

    
    // beam window
    if (Cfg::make_beam_window) {
        double rad = Cfg::beam_window_radius; //2*cm,         # made up number
        double thick = Cfg::beam_window_thickness; //0.381*mm,  # from TN05-001
        double zoff = Cfg::beam_window_zoffset;    //-5*m # made up number
        G4Material* mat = G4Material::GetMaterial(Cfg::beam_window_material);

        G4Tubs* shape = new G4Tubs("shapeBeamWindow",0,rad,0.5*thick,0,2*M_PI);

        G4LogicalVolume* lv 
            = new G4LogicalVolume(shape, mat, "lvBeamWindow", 0,0,0);
        
        G4VPhysicalVolume *pv
            = new G4PVPlacement(0,G4ThreeVector(0,0,zoff),lv,"BeamWindow",
                                world_pv->GetLogicalVolume(),false,0);
        pv = 0;
    }
            
                                                  
    // tc = trigger counters
    if (Cfg::make_trigger_counters) {
        double width = Cfg::trigger_counter_width; //2*cm
        double thick = Cfg::trigger_counter_thickness; //0.5*cm
        double pcthick = Cfg::trigger_counter_pcthick; // 1*mm
        double zsep = Cfg::trigger_counter_separation; //40*cm
        double zoff = Cfg::trigger_counter_offset; //-20*cm
        G4Material* mat_tcpc = G4Material::GetMaterial(Cfg::tc_pc_material);
        G4Material* mat_scint = G4Material::GetMaterial(Cfg::tc_scint_material);        

        G4Box* shape_tcpc 
            = new G4Box("shapeTCPC", 0.5*width+pcthick,0.5*width+pcthick,0.5*thick+pcthick);
        G4Box* shape_scint
            = new G4Box("shapeTCScint", 0.5*width,0.5*width,0.5*thick);

        G4LogicalVolume* lv_tcpc 
            = new G4LogicalVolume(shape_tcpc, mat_tcpc, "lvTCPC", 0,0,0);
        G4LogicalVolume* lv_scint
            = new G4LogicalVolume(shape_scint, mat_scint, "lvTCScint", 0,0,0);

        G4VPhysicalVolume* scint_in_pc
            = new G4PVPlacement(0, G4ThreeVector(), lv_scint, "TriggerCounterScint",
                                lv_tcpc, false, 0);
        scint_in_pc = 0;

        for (int tcnum = 0; tcnum < 3; ++tcnum) {
            G4ThreeVector pos(0,0,zsep*tcnum + zoff);
            G4VPhysicalVolume* pv 
                = new G4PVPlacement(0, pos, lv_tcpc, "TriggerCounter",
                                world_pv->GetLogicalVolume(), false, tcnum);
            pv = 0;
        }
    }

    // Tub detectors
    if (Cfg::make_tub_detectors) {
        G4LogicalVolume* lv_tub1 = MakeTubDet(Cfg::sample_material1, Cfg::tub_material1);
        G4LogicalVolume* lv_tub2 = MakeTubDet(Cfg::sample_material2, Cfg::tub_material2);

        string tubname1 = "Tub" + Cfg::tub_material1;
        string tubname2 = "Tub" + Cfg::tub_material2;

        double zsep = Cfg::tubdet_separation;
        double zoff = Cfg::tubdet_offset;

        G4RotationMatrix* rot = new G4RotationMatrix();
        rot->rotateX(90*deg);
        G4VPhysicalVolume* pv_tub1 
            = new G4PVPlacement(rot,G4ThreeVector(0,0,zoff), lv_tub1, tubname1.c_str(),
                                world_pv->GetLogicalVolume(), false, 0);
        G4VPhysicalVolume* pv_tub2
            = new G4PVPlacement(rot,G4ThreeVector(0,0,zoff+zsep), lv_tub2, tubname2.c_str(), 
                            world_pv->GetLogicalVolume(), false, 0);
        pv_tub1 = pv_tub2 = 0;
    }

    {
        bool ok = Cowbells::AddMaterialProperties(m_prop_file);
        if (!ok) return 0;
    }
    {
        bool ok = Cowbells::AddOpticalSurfaces(m_prop_file);
        if (!ok) return 0;
    }

    Cowbells::dump(world_pv, 0);
    Cowbells::dump_lvs();
    Cowbells::dump_pvs();



    // sensitive detectors

    // Tub Detectors
    {
        string lvname = "lvTubPC";

        std::vector<std::string> paths; // fake it until you make it
        paths.push_back("");
        paths.push_back("World:0/TubTeflon:0/TubdetWindow:0/PC:0");
        paths.push_back("World:0/TubAluminum:0/TubdetWindow:0/PC:0");

        Cowbells::SensitiveDetector* sd = 
            new Cowbells::SensitiveDetector("/cowbells/tub","tub_hc", paths);
        G4SDManager::GetSDMpointer()->AddNewDetector(sd);
        G4LogicalVolume* lv = G4LogicalVolumeStore::GetInstance()->GetVolume(lvname.c_str());
        if (!lv) {
            cerr << "No LV for " << lvname << endl;
            assert (lv);
        }
        lv->SetSensitiveDetector(sd);
    }

    // Trigger counters
    {
        string lvname = "lvTCPC";


        std::vector<std::string> paths;
        paths.push_back("");
        paths.push_back("World:0/TriggerCounter:0");
        paths.push_back("World:0/TriggerCounter:1");
        paths.push_back("World:0/TriggerCounter:2");

        Cowbells::SensitiveDetector* sd = 
            new Cowbells::SensitiveDetector("/cowbells/tc","tc_hc", paths);
        G4SDManager::GetSDMpointer()->AddNewDetector(sd);
        G4LogicalVolume* lv = G4LogicalVolumeStore::GetInstance()->GetVolume(lvname.c_str());
        if (!lv) {
            cerr << "No LV for " << lvname << endl;
            assert (lv);
        }
        lv->SetSensitiveDetector(sd);
    }

    return world_pv;
}

