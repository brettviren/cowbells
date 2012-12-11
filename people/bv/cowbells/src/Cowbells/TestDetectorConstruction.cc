#include "Cowbells/TestDetectorConstruction.h"
#include "Cowbells/SensitiveDetector.h"

#include <G4VPhysicalVolume.hh>

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include <G4LogicalVolumeStore.hh>
#include <G4SDManager.hh>

#include <iostream>
using namespace std;


Cowbells::TestDetectorConstruction::TestDetectorConstruction()
{
    cerr << "Creating Cowbells::TestDetectorConstruction" << endl;
}

Cowbells::TestDetectorConstruction::~TestDetectorConstruction()
{
    cerr << "Destructing Cowbells::TestDetectorConstruction" << endl;
}

G4VPhysicalVolume* Cowbells::TestDetectorConstruction::Construct()
{
    // copied from novice/N06

    G4double expHall_x;
    G4double expHall_y;
    G4double expHall_z;

    G4double tank_x;
    G4double tank_y;
    G4double tank_z;

    G4double bubble_x;
    G4double bubble_y;
    G4double bubble_z;

  expHall_x = expHall_y = expHall_z = 10.0*m;
  tank_x    = tank_y    = tank_z    =  5.0*m;
  bubble_x  = bubble_y  = bubble_z  =  0.5*m;

//	------------- Materials -------------

  G4double a, z, density;
  G4int nelements;

// Air
// 
  G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);

  G4Material* Air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
  Air->AddElement(N, 70.*perCent);
  Air->AddElement(O, 30.*perCent);

// Water
// 
  G4Element* H = new G4Element("Hydrogen", "H", z=1 , a=1.01*g/mole);

  G4Material* Water = new G4Material("Water", density= 1.0*g/cm3, nelements=2);
  Water->AddElement(H, 2);
  Water->AddElement(O, 1);

//
// ------------ Generate & Add Material Properties Table ------------
//
  const G4int nEntries = 32;

  G4double PhotonEnergy[nEntries] =
            { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
              2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
              2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
              2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
              2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
              3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
              3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
              3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };
//
// Water
//	      
  G4double RefractiveIndex1[nEntries] =
            { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
              1.346,  1.3465, 1.347,  1.3475, 1.348,
              1.3485, 1.3492, 1.35,   1.3505, 1.351,
              1.3518, 1.3522, 1.3530, 1.3535, 1.354,
              1.3545, 1.355,  1.3555, 1.356,  1.3568,
              1.3572, 1.358,  1.3585, 1.359,  1.3595,
              1.36,   1.3608};

  G4double Absorption1[nEntries] =
           {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
           15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
           45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
           52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
           30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
           17.500*m, 14.500*m };

  G4double ScintilFast[nEntries] =
            { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00 };
  G4double ScintilSlow[nEntries] =
            { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
              7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
              3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
              4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
              7.00, 6.00, 5.00, 4.00 };

  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

  myMPT1->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex1,nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption1,     nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("FASTCOMPONENT",PhotonEnergy, ScintilFast,     nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("SLOWCOMPONENT",PhotonEnergy, ScintilSlow,     nEntries)
        ->SetSpline(true);
  
  myMPT1->AddConstProperty("SCINTILLATIONYIELD",50./MeV);
  myMPT1->AddConstProperty("RESOLUTIONSCALE",1.0);
  myMPT1->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  myMPT1->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
  myMPT1->AddConstProperty("YIELDRATIO",0.8);

  const G4int NUMENTRIES_water = 60;

  G4double ENERGY_water[NUMENTRIES_water] = {
     1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
     1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
     1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
     1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
     1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
     2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
     2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
     2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
     2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
     2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
     3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
     3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
     3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
     4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV,
     5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV
  };

  //assume 100 times larger than the rayleigh scattering for now.
  G4double MIE_water[NUMENTRIES_water] = {
     167024.4*m, 158726.7*m, 150742  *m,
     143062.5*m, 135680.2*m, 128587.4*m,
     121776.3*m, 115239.5*m, 108969.5*m,
     102958.8*m, 97200.35*m, 91686.86*m,
     86411.33*m, 81366.79*m, 76546.42*m,
     71943.46*m, 67551.29*m, 63363.36*m,
     59373.25*m, 55574.61*m, 51961.24*m,
     48527.00*m, 45265.87*m, 42171.94*m,
     39239.39*m, 36462.50*m, 33835.68*m,
     31353.41*m, 29010.30*m, 26801.03*m,
     24720.42*m, 22763.36*m, 20924.88*m,
     19200.07*m, 17584.16*m, 16072.45*m,
     14660.38*m, 13343.46*m, 12117.33*m,
     10977.70*m, 9920.416*m, 8941.407*m,
     8036.711*m, 7202.470*m, 6434.927*m,
     5730.429*m, 5085.425*m, 4496.467*m,
     3960.210*m, 3473.413*m, 3032.937*m,
     2635.746*m, 2278.907*m, 1959.588*m,
     1675.064*m, 1422.710*m, 1200.004*m,
     1004.528*m, 833.9666*m, 686.1063*m
  };

  // gforward, gbackward, forward backward ratio
  G4double MIE_water_const[3]={0.99,0.99,0.8};

  myMPT1->AddProperty("MIEHG",ENERGY_water,MIE_water,NUMENTRIES_water)
        ->SetSpline(true);
  myMPT1->AddConstProperty("MIEHG_FORWARD",MIE_water_const[0]);
  myMPT1->AddConstProperty("MIEHG_BACKWARD",MIE_water_const[1]);
  myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO",MIE_water_const[2]);

  Water->SetMaterialPropertiesTable(myMPT1);

  // Set the Birks Constant for the Water scintillator

  Water->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

//
// Air
//
  G4double RefractiveIndex2[nEntries] =
            { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00 };

  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex2, nEntries);
  
  Air->SetMaterialPropertiesTable(myMPT2);

//
//	------------- Volumes --------------

// The experimental Hall
//
  G4Box* expHall_box = new G4Box("World",expHall_x,expHall_y,expHall_z);

  G4LogicalVolume* expHall_log
    = new G4LogicalVolume(expHall_box,Air,"World",0,0,0);

  G4VPhysicalVolume* expHall_phys
    = new G4PVPlacement(0,G4ThreeVector(),expHall_log,"World",0,false,0);

// The Water Tank
//	
  G4Box* waterTank_box = new G4Box("Tank",tank_x,tank_y,tank_z);

  G4LogicalVolume* waterTank_log
    = new G4LogicalVolume(waterTank_box,Water,"Tank",0,0,0);

  G4VPhysicalVolume* waterTank_phys
    = new G4PVPlacement(0,G4ThreeVector(),waterTank_log,"Tank",
                        expHall_log,false,0);

// The Air Bubble
//   
  G4Box* bubbleAir_box = new G4Box("Bubble",bubble_x,bubble_y,bubble_z);

  G4LogicalVolume* bubbleAir_log
    = new G4LogicalVolume(bubbleAir_box,Air,"Bubble",0,0,0);

//G4VPhysicalVolume* bubbleAir_phys =
      new G4PVPlacement(0,G4ThreeVector(0,2.5*m,0),bubbleAir_log,"Bubble0",
                        waterTank_log,false,0);
      new G4PVPlacement(0,G4ThreeVector(0,-2.5*m,0),bubbleAir_log,"Bubble1",
                        waterTank_log,false,1);

//	------------- Surfaces --------------
//
// Water Tank
//
  G4OpticalSurface* OpWaterSurface = new G4OpticalSurface("WaterSurface");
  OpWaterSurface->SetType(dielectric_dielectric);
  OpWaterSurface->SetFinish(ground);
  OpWaterSurface->SetModel(unified);

  new G4LogicalBorderSurface("WaterSurface",
                                 waterTank_phys,expHall_phys,OpWaterSurface);

// Air Bubble
//
  G4OpticalSurface* OpAirSurface = new G4OpticalSurface("AirSurface");
  OpAirSurface->SetType(dielectric_dielectric);
  OpAirSurface->SetFinish(polished);
  OpAirSurface->SetModel(glisur);

  G4LogicalSkinSurface* AirSurface = 
	  new G4LogicalSkinSurface("AirSurface", bubbleAir_log, OpAirSurface);

  G4OpticalSurface* opticalSurface = dynamic_cast <G4OpticalSurface*>
        (AirSurface->GetSurface(bubbleAir_log)->GetSurfaceProperty());

  if (opticalSurface) opticalSurface->DumpInfo();

//
// Generate & Add Material Properties Table attached to the optical surfaces
//
  const G4int num = 2;
  G4double Ephoton[num] = {2.034*eV, 4.136*eV};

  //OpticalWaterSurface 
  G4double RefractiveIndex[num] = {1.35, 1.40};
  G4double SpecularLobe[num]    = {0.3, 0.3};
  G4double SpecularSpike[num]   = {0.2, 0.2};
  G4double Backscatter[num]     = {0.2, 0.2};

  G4MaterialPropertiesTable* myST1 = new G4MaterialPropertiesTable();
  
  myST1->AddProperty("RINDEX",                Ephoton, RefractiveIndex, num);
  myST1->AddProperty("SPECULARLOBECONSTANT",  Ephoton, SpecularLobe,    num);
  myST1->AddProperty("SPECULARSPIKECONSTANT", Ephoton, SpecularSpike,   num);
  myST1->AddProperty("BACKSCATTERCONSTANT",   Ephoton, Backscatter,     num);

  OpWaterSurface->SetMaterialPropertiesTable(myST1);

  //OpticalAirSurface
  G4double Reflectivity[num] = {0.3, 0.5};
  G4double Efficiency[num]   = {0.8, 1.0};

  G4MaterialPropertiesTable *myST2 = new G4MaterialPropertiesTable();

  myST2->AddProperty("REFLECTIVITY", Ephoton, Reflectivity, num);
  myST2->AddProperty("EFFICIENCY",   Ephoton, Efficiency,   num);

  OpAirSurface->SetMaterialPropertiesTable(myST2);


  this->RegisterSensDets();

//always return the physical World
  return expHall_phys;
}        


void Cowbells::TestDetectorConstruction::add_sensdet(std::string lvname,
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

    Cowbells::SensitiveDetector* csd = 
      new Cowbells::SensitiveDetector(sdname.c_str(), hcname.c_str());
    m_lvsd[lvname] = csd;
}

void Cowbells::TestDetectorConstruction::RegisterSensDets()
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
