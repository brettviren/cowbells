#include "WCSimDetectorConstruction.hh"
#include "WCSimTuningParameters.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpBoundaryProcess.hh"

void WCSimDetectorConstruction::ConstructMaterials()
{
  //****Materials Definitions****

  G4double density;
  G4double a;

  //---Vacuum

  density     = universe_mean_density;              //from PhysicalConstants.h
  G4double pressure    = 1.e-19*pascal;
  G4double temperature = 0.1*kelvin;
  a = 1.01*g/mole;
  G4Material* Vacuum = 
    new G4Material("Vacuum", 1., a, density,
                   kStateGas,temperature,pressure);

  //---Water
  
 //  a = 1.01*g/mole;
//   G4Element* elH 
//     = new G4Element("Hydrogen","H", 1,a);
  
//   a = 16.00*g/mole;
//   G4Element* elO 
//     = new G4Element("Oxygen","O", 8,a);
  



  //X. Qian Feb 7th 2012, change to LS-Water
  //Hydrogen
  a = 1.01*g/mole;
  G4Element* elH 
    = new G4Element("Hydrogen","H", 1,a);
  //Oxegen
  a = 16.00*g/mole;
  G4Element* elO 
    = new G4Element("Oxygen","O", 8,a);
  //Sulfer
  a = 32.065*g/mole;
  G4Element *elS 
    = new G4Element("Sulfur","S",16,a);
  //Nitrogen
  a = 14.007*g/mole;
  G4Element *elN 
    = new G4Element("Nitrogen","N",7,a);
  //Carbon
  a = 12.01*g/mole;
  G4Element *elC
    = new G4Element("Carbon","C",6,a);


  density = 0.9945*g/cm3;
  G4Material* Water 
    = new G4Material("Water",density,5);
  Water->AddElement(elH,0.659);
  Water->AddElement(elO,0.309);
  Water->AddElement(elS,0.0009);
  Water->AddElement(elN,0.000058);
  Water->AddElement(elC,0.031);

 //  density = 1.00*g/cm3;
//   G4Material* Water 
//     = new G4Material("Water",density,2);
//   Water->AddElement(elH, 2);
//   Water->AddElement(elO, 1);


 //---Gd doped Water

 a = 157.25*g/mole;
 G4Element* Gd
    = new G4Element("Gadolinium","Gd", 64,a);

 density = 1.00*g/cm3;
 G4Material* DopedWater
    = new G4Material("Doped Water",density,2);
 DopedWater->AddMaterial(Water,99.9*perCent);
 DopedWater->AddElement(Gd,0.1*perCent);

 //---Ice 
 
 density = 0.92*g/cm3;//Ice
 G4Material* Ice = new G4Material("Ice",density,2);
 Ice->AddElement(elH, 2);
 Ice->AddElement(elO, 1);

 //---Steel
  
 //  a= 12.01*g/mole;
//   G4Element* elC 
//     = new G4Element("Carbon","C", 6,a);
  
  a = 55.85*g/mole;
  G4Element* elFe
    = new G4Element("Iron","Fe", 26,a);
  
  density = 7.8*g/cm3;
  G4Material* Steel
    = new G4Material("Steel",density,2);
  Steel->AddElement(elC, 1.*perCent);
  Steel->AddElement(elFe, 99.*perCent);
  
  //---Stainless steel 304L (simple example, particular alloy can be different)
  
  a = 51.9961*g/mole;
  G4Element* elCr = new G4Element("Chromium", "Cr", 24., a);
  
  a = 58.6934*g/mole;
  G4Element* elNi = new G4Element("Nickel", "Ni", 28., a);

  a = 54.938*g/mole;
  G4Element* elMn = new G4Element("Manganese", "Mn", 25., a); 

  a = 30.974*g/mole;
  G4Element* elP = new G4Element("Phosphore", "P", 15., a);

  a = 28.09*g/mole;
  G4Element* elSi = new G4Element("Silicon", "Si", 14., a); 

 //  a = 32.065*g/mole;
//   G4Element* elS = new G4Element("Sulphur", "S", 16., a);

  density = 7.81*g/cm3;
  G4Material* StainlessSteel = new G4Material("StainlessSteel", density, 8);
 
  StainlessSteel->AddElement(elFe, 70.44*perCent);
  StainlessSteel->AddElement(elCr, 18*perCent);
  StainlessSteel->AddElement(elC,  0.08*perCent);
  StainlessSteel->AddElement(elNi, 8*perCent); 
  StainlessSteel->AddElement(elP, 0.45*perCent);
  StainlessSteel->AddElement(elSi,  1*perCent);
  StainlessSteel->AddElement(elMn, 2*perCent);
  StainlessSteel->AddElement(elS, 0.03*perCent);
  
  G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable();
   
  const G4int nEntries = 2;
  G4double photonEnergy[nEntries] = {1.*eV , 7.*eV};
  
  //G4double rindex_Steel[nEntries] = {1.462 , 1.462}; // No I haven't gone mad
  G4double abslength_Steel[nEntries] = {.001*mm , .001*mm};
  //mpt->AddProperty("RINDEX", photonEnergy, rindex_Steel, nEntries);
  mpt->AddProperty("ABSLENGTH", photonEnergy, abslength_Steel, nEntries);
  
  StainlessSteel->SetMaterialPropertiesTable(mpt);

  //---Solid Dry Ice

  density = 1.563*g/cm3;
  G4Material* DryIce = new G4Material("SolidDryIce", density, 2);
  DryIce->AddElement(elC, 1);
  DryIce->AddElement(elO, 2);



  //---Air
  
//   a = 14.01*g/mole;
//   G4Element* elN 
//     = new G4Element("Nitrogen","N", 7,a);
  
  density = 1.290*mg/cm3;
  G4Material* Air 
    = new G4Material("Air",density,2);
  Air->AddElement(elN, 70.*perCent);
  Air->AddElement(elO, 30.*perCent);
  
  //---Plastic
  
  density = 0.95*g/cm3;
  G4Material* Plastic
    = new G4Material("Plastic",density,2);
  Plastic->AddElement(elC, 1);
  Plastic->AddElement(elH, 2);

  //---Aluminum (element and material)

  a = 26.98*g/mole;
  G4Element* elAl = new G4Element("Aluminum", "Al", 13, a);  

  density = 2.7*g/cm3;
  G4Material* Aluminum
    = new G4Material("Aluminum",density,1);
  Aluminum->AddElement(elAl, 1);

  //---Black sheet

  density = 0.95*g/cm3;
  G4Material* Blacksheet
    = new G4Material("Blacksheet",density,2);
  Blacksheet->AddElement(elC, 1);
  Blacksheet->AddElement(elH, 2);

  //---Tyvek - jl145

  density = 0.38*g/cm3;  //cf. DuPont product handbook
  G4Material* Tyvek
    = new G4Material("Tyvek",density,2);
  Tyvek->AddElement(elC, 1);  //polyethylene
  Tyvek->AddElement(elH, 2);

  //---Glass
 
  density = 2.20*g/cm3;
  G4Material* SiO2 = new G4Material("SiO2",density,2);
  SiO2->AddElement(elSi, 1);
  SiO2->AddElement(elO , 2);

  a = 10.81*g/mole;
  G4Element* elB = new G4Element("Boron", "B", 5, a);  

  density = 2.46*g/cm3;
  G4Material* B2O3 = new G4Material("B2O3",density,2);
  B2O3->AddElement(elB, 2);
  B2O3->AddElement(elO, 3);

  a = 22.99*g/mole;
  G4Element* elNa = new G4Element("Sodium", "Na", 11, a);  

  density = 2.27*g/cm3;
  G4Material* Na2O = new G4Material("Na2O",density,2);
  Na2O->AddElement(elNa, 2);
  Na2O->AddElement(elO, 1);

  density = 4.00*g/cm3;
  G4Material* Al2O3 = new G4Material("Al2O3",density,2);
  Al2O3->AddElement(elAl, 2);
  Al2O3->AddElement(elO, 3);

//   G4Material* blackAcryl
//     = new G4Material("blackAcryl", density, 3);
//   blackAcryl -> AddElement(elH, 6);
//   blackAcryl -> AddElement(elC, 4);
//   blackAcryl -> AddElement(elO, 2);

    
  // adding acrylic as the WLS film Add X. Qian
  density = 1.18*g/cm3;
  G4Material* acrylic_wls_film = new G4Material("acrylic_wls_film",density,3);
  acrylic_wls_film->AddElement(elH,0.08055);
  acrylic_wls_film->AddElement(elC,0.59984);
  acrylic_wls_film->AddElement(elO,0.31961);

  density = 2.23*g/cm3;
  G4Material* Glass
    = new G4Material("Glass",density,4);
  //G4Material* Glass
  //= new G4Material("Glass",density,8);  //Put in 8 materials later
  
  Glass->AddMaterial(SiO2, 80.6*perCent);
  Glass->AddMaterial(B2O3, 13.0*perCent);
  Glass->AddMaterial(Na2O, 4.0*perCent);
  Glass->AddMaterial(Al2O3, 2.4*perCent);
  //Glass->AddMaterial(Al2O3, 2.3*perCent);  
  //Put in 2.3 percent if the other 4 materials = 0.1 percent

  //---Rock
 
  //  a = 16.00*g/mole;  G4Element* elO  = new G4Element("Oxygen","O", 8,a);
  //  a = 28.09*g/mole;  G4Element* elSi = new G4Element("Silicon", "Si", 14., a); 
  //  a = 26.98*g/mole;  G4Element* elAl = new G4Element("Aluminum", "Al", 13, a);  
  //  a = 55.85*g/mole;  G4Element* elFe = new G4Element("Iron","Fe", 26,a);
  a = 40.08*g/mole;  G4Element* elCa = new G4Element("Calcium","Ca", 20,a);
  //  a = 22.99*g/mole;  G4Element* elNa = new G4Element("Sodium", "Na", 11, a);  
  a = 39.10*g/mole;  G4Element* elK = new G4Element("Potassium","K", 19,a);
  a = 24.30*g/mole;  G4Element* elMg = new G4Element("Magnesium","Mg",12,a);
 
  density = 2.7*g/cm3; 
  G4Material* Rock = new G4Material("Rock", density, 8);
  
  //From Daya-Bay 
  Rock->AddElement(elO,  48.50*perCent);
  Rock->AddElement(elSi, 34.30*perCent);
  Rock->AddElement(elAl,  8.00*perCent);
  Rock->AddElement(elFe,  2.00*perCent);
  Rock->AddElement(elCa,  0.20*perCent);
  Rock->AddElement(elNa,  2.40*perCent);
  Rock->AddElement(elK,   4.50*perCent);
  Rock->AddElement(elMg,  0.10*perCent);

// -------------------------------------------------------------
// Generate & Add Material Properties Table
// -------------------------------------------------------------


  const G4int NUMENTRIES = 32;
 
  G4double PPCKOV[NUMENTRIES] =
    { 2.034E-9*GeV, 2.068E-9*GeV, 2.103E-9*GeV, 2.139E-9*GeV,
      2.177E-9*GeV, 2.216E-9*GeV, 2.256E-9*GeV, 2.298E-9*GeV,
      2.341E-9*GeV, 2.386E-9*GeV, 2.433E-9*GeV, 2.481E-9*GeV,
      2.532E-9*GeV, 2.585E-9*GeV, 2.640E-9*GeV, 2.697E-9*GeV,
      2.757E-9*GeV, 2.820E-9*GeV, 2.885E-9*GeV, 2.954E-9*GeV,
      3.026E-9*GeV, 3.102E-9*GeV, 3.181E-9*GeV, 3.265E-9*GeV,
      3.353E-9*GeV, 3.446E-9*GeV, 3.545E-9*GeV, 3.649E-9*GeV,
      3.760E-9*GeV, 3.877E-9*GeV, 4.002E-9*GeV, 4.136E-9*GeV };


  // default values
  /*
  G4double RINDEX1[NUMENTRIES] =
    { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
      1.346,  1.3465, 1.347,  1.3475, 1.348,
      1.3485, 1.3492, 1.35,   1.3505, 1.351,
      1.3518, 1.3522, 1.3530, 1.3535, 1.354,
      1.3545, 1.355,  1.3555, 1.356,  1.3568,
      1.3572, 1.358,  1.3585, 1.359,  1.3595,
      1.36,   1.3608};
  */
  
  // M Fechner,  values from refsg.F in apdetsim
  /*  G4double RINDEX1[NUMENTRIES] = 
    { 1.33332, 1.333364, 1.333396, 1.3343, 1.33465,
      1.33502, 1.3354, 1.33579, 1.3362, 1.33663, 1.33709,
      1.33756, 1.33806, 1.3386, 1.33915, 1.33974,
      1.34037, 1.34105, 1.34176, 1.34253, 1.34336,
      1.34425, 1.34521, 1.34626, 1.3474, 1.34864,
      1.35002, 1.35153, 1.35321, 1.35507, 1.35717, 1.35955 };
  */

   //From SFDETSIM water absorption
   const G4int NUMENTRIES_water=60;

   G4double ENERGY_water[NUMENTRIES_water] =
     { 1.56962e-09*GeV, 1.58974e-09*GeV, 1.61039e-09*GeV, 1.63157e-09*GeV, 
       1.65333e-09*GeV, 1.67567e-09*GeV, 1.69863e-09*GeV, 1.72222e-09*GeV, 
       1.74647e-09*GeV, 1.77142e-09*GeV,1.7971e-09*GeV, 1.82352e-09*GeV, 
       1.85074e-09*GeV, 1.87878e-09*GeV, 1.90769e-09*GeV, 1.93749e-09*GeV, 
       1.96825e-09*GeV, 1.99999e-09*GeV, 2.03278e-09*GeV, 2.06666e-09*GeV,
       2.10169e-09*GeV, 2.13793e-09*GeV, 2.17543e-09*GeV, 2.21428e-09*GeV, 
       2.25454e-09*GeV, 2.29629e-09*GeV, 2.33962e-09*GeV, 2.38461e-09*GeV, 
       2.43137e-09*GeV, 2.47999e-09*GeV, 2.53061e-09*GeV, 2.58333e-09*GeV, 
       2.63829e-09*GeV, 2.69565e-09*GeV, 2.75555e-09*GeV, 2.81817e-09*GeV, 
       2.88371e-09*GeV, 2.95237e-09*GeV, 3.02438e-09*GeV, 3.09999e-09*GeV,
       3.17948e-09*GeV, 3.26315e-09*GeV, 3.35134e-09*GeV, 3.44444e-09*GeV, 
       3.54285e-09*GeV, 3.64705e-09*GeV, 3.75757e-09*GeV, 3.87499e-09*GeV, 
       3.99999e-09*GeV, 4.13332e-09*GeV, 4.27585e-09*GeV, 4.42856e-09*GeV, 
       4.59258e-09*GeV, 4.76922e-09*GeV, 4.95999e-09*GeV, 5.16665e-09*GeV, 
       5.39129e-09*GeV, 5.63635e-09*GeV, 5.90475e-09*GeV, 6.19998e-09*GeV };



      // Air
   G4double RINDEX_air[NUMENTRIES_water] = 
     { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
       1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
       1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
       1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
       1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
       1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
       1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
       1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
       1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
       1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
       

   // M Fechner : new ; define the water refraction index using refsg.F 
   //from skdetsim using the whole range.  
   G4double waterls = 1.3492/1.33427;
   G4double RINDEX1[NUMENTRIES_water] = 
     {1.32885*waterls, 1.32906*waterls, 1.32927*waterls, 1.32948*waterls, 
      1.3297*waterls, 1.32992*waterls, 1.33014*waterls, 1.33037*waterls, 
      1.3306*waterls, 1.33084*waterls, 1.33109*waterls, 1.33134*waterls, 
      1.3316*waterls, 1.33186*waterls, 1.33213*waterls, 1.33241*waterls, 
      1.3327*waterls, 1.33299*waterls, 1.33329*waterls, 1.33361*waterls, 
      1.33393*waterls, 1.33427*waterls, 1.33462*waterls, 1.33498*waterls, 
      1.33536*waterls, 1.33576*waterls, 1.33617*waterls, 1.3366*waterls, 
      1.33705*waterls, 1.33753*waterls, 1.33803*waterls, 1.33855*waterls, 
      1.33911*waterls, 1.3397*waterls, 1.34033*waterls, 1.341*waterls, 
      1.34172*waterls, 1.34248*waterls, 1.34331*waterls, 1.34419*waterls, 
      1.34515*waterls, 1.3462*waterls, 1.34733*waterls, 1.34858*waterls, 
      1.34994*waterls, 1.35145*waterls, 1.35312*waterls, 1.35498*waterls, 
      1.35707*waterls, 1.35943*waterls, 1.36211*waterls, 1.36518*waterls, 
      1.36872*waterls, 1.37287*waterls, 1.37776*waterls, 1.38362*waterls, 
      1.39074*waterls, 1.39956*waterls, 1.41075*waterls, 1.42535*waterls};
   

    G4double ABWFF = 1.;

    // Get from the tuning parameters
    ABWFF = WCSimTuningParams->GetAbwff();

    //T. Akiri: Values from Skdetsim 
    
   //  G4double ABSORPTION_water[NUMENTRIES_water] =
//       {
//         16.1419*cm*ABWFF,  18.278*cm*ABWFF, 21.0657*cm*ABWFF, 24.8568*cm*ABWFF, 30.3117*cm*ABWFF, 
// 	38.8341*cm*ABWFF, 54.0231*cm*ABWFF, 81.2306*cm*ABWFF, 120.909*cm*ABWFF, 160.238*cm*ABWFF, 
// 	193.771*cm*ABWFF, 215.017*cm*ABWFF, 227.747*cm*ABWFF,  243.85*cm*ABWFF, 294.036*cm*ABWFF, 
// 	321.647*cm*ABWFF,  342.81*cm*ABWFF, 362.827*cm*ABWFF, 378.041*cm*ABWFF, 449.378*cm*ABWFF,
//         739.434*cm*ABWFF, 1114.23*cm*ABWFF, 1435.56*cm*ABWFF, 1611.06*cm*ABWFF, 1764.18*cm*ABWFF, 
// 	2100.95*cm*ABWFF,  2292.9*cm*ABWFF, 2431.33*cm*ABWFF,  3053.6*cm*ABWFF, 4838.23*cm*ABWFF, 
// 	6539.65*cm*ABWFF, 7682.63*cm*ABWFF, 9137.28*cm*ABWFF, 12220.9*cm*ABWFF, 15270.7*cm*ABWFF, 
// 	19051.5*cm*ABWFF, 23671.3*cm*ABWFF, 29191.1*cm*ABWFF, 35567.9*cm*ABWFF,   42583*cm*ABWFF,
//         49779.6*cm*ABWFF, 56465.3*cm*ABWFF,   61830*cm*ABWFF, 65174.6*cm*ABWFF, 66143.7*cm*ABWFF,   
// 	64820*cm*ABWFF,   61635*cm*ABWFF, 57176.2*cm*ABWFF, 52012.1*cm*ABWFF, 46595.7*cm*ABWFF, 
// 	41242.1*cm*ABWFF, 36146.3*cm*ABWFF, 31415.4*cm*ABWFF, 27097.8*cm*ABWFF, 23205.7*cm*ABWFF, 
// 	19730.3*cm*ABWFF, 16651.6*cm*ABWFF, 13943.6*cm*ABWFF, 11578.1*cm*ABWFF, 9526.13*cm*ABWFF
//       };
    
    //Minfang's absorption length
     // G4double ABSORPTION_water[NUMENTRIES_water] =
//       {16.1361*cm*ABWFF,18.27*cm*ABWFF,21.0544*cm*ABWFF,24.8407*cm*ABWFF,30.2878*cm*ABWFF,
//        38.797*cm*ABWFF,53.9741*cm*ABWFF,81.1622*cm*ABWFF,120.815*cm*ABWFF,160.132*cm*ABWFF,
//        193.633*cm*ABWFF,214.889*cm*ABWFF,227.592*cm*ABWFF,243.699*cm*ABWFF,293.817*cm*ABWFF,
//        321.409*cm*ABWFF,342.594*cm*ABWFF,362.554*cm*ABWFF,377.778*cm*ABWFF,449.145*cm*ABWFF,
//        739.182*cm*ABWFF,1113.94*cm*ABWFF,1435.09*cm*ABWFF,1610.46*cm*ABWFF,1762.75*cm*ABWFF,
//        2097.9*cm*ABWFF,2286.86*cm*ABWFF,2425.89*cm*ABWFF,3040.76*cm*ABWFF,4795.46*cm*ABWFF,
//        6433.02*cm*ABWFF,7483.91*cm*ABWFF,8785.83*cm*ABWFF,11447.3*cm*ABWFF,13682*cm*ABWFF,
//        16145.6*cm*ABWFF,18599.1*cm*ABWFF,20797.8*cm*ABWFF,22368.8*cm*ABWFF,23218.7*cm*ABWFF,
//        22700.6*cm*ABWFF,21185.1*cm*ABWFF,18936.1*cm*ABWFF,15863.1*cm*ABWFF,12667.3*cm*ABWFF,
//        8986.35*cm*ABWFF,4299.52*cm*ABWFF,3184.37*cm*ABWFF,1950.17*cm*ABWFF,1197.19*cm*ABWFF,
//        924.933*cm*ABWFF,193.637*cm*ABWFF,117.386*cm*ABWFF,111.227*cm*ABWFF,112.194*cm*ABWFF,
//        115.398*cm*ABWFF,115.551*cm*ABWFF,110.587*cm*ABWFF,116.941*cm*ABWFF,139.961*cm*ABWFF
//       };

     //Minfang's absorption length for Water LS
     G4double ABSORPTION_water[NUMENTRIES_water] =
       {7.9741*cm*ABWFF, 9.02933*cm*ABWFF, 10.4065*cm*ABWFF, 12.2793*cm*ABWFF, 14.974*cm*ABWFF, 
	19.184*cm*ABWFF, 26.6874*cm*ABWFF, 40.128*cm*ABWFF, 59.7291*cm*ABWFF, 79.1575*cm*ABWFF, 
	95.7229*cm*ABWFF, 106.218*cm*ABWFF, 112.507*cm*ABWFF, 120.462*cm*ABWFF, 145.254*cm*ABWFF, 
	158.894*cm*ABWFF, 169.348*cm*ABWFF, 179.237*cm*ABWFF, 186.752*cm*ABWFF, 221.993*cm*ABWFF, 
	365.279*cm*ABWFF, 550.429*cm*ABWFF, 709.167*cm*ABWFF, 795.864*cm*ABWFF, 871.504*cm*ABWFF, 
	1037.87*cm*ABWFF, 1132.69*cm*ABWFF, 1201.08*cm*ABWFF, 1508.48*cm*ABWFF, 2390.09*cm*ABWFF, 
	3230.6*cm*ABWFF, 3795.21*cm*ABWFF, 4513.81*cm*ABWFF, 6037.13*cm*ABWFF, 6544.36*cm*ABWFF, 
	5550.56*cm*ABWFF, 5040.82*cm*ABWFF, 4574.07*cm*ABWFF, 4116.67*cm*ABWFF, 3714.29*cm*ABWFF, 
	3250*cm*ABWFF, 2806.82*cm*ABWFF, 2386.47*cm*ABWFF, 1937.25*cm*ABWFF, 1520*cm*ABWFF, 
	1064.66*cm*ABWFF, 500.507*cm*ABWFF, 370.037*cm*ABWFF, 225.571*cm*ABWFF, 137.989*cm*ABWFF, 
	106.58*cm*ABWFF, 22.1118*cm*ABWFF, 13.3941*cm*ABWFF, 12.6969*cm*ABWFF, 12.8165*cm*ABWFF, 
	13.1955*cm*ABWFF, 13.2277*cm*ABWFF, 12.6725*cm*ABWFF, 13.429*cm*ABWFF, 16.1433*cm*ABWFF
      };

    
 //    //Xin Qian: proposed value for absorption length
//     G4double ABSORPTION_water[NUMENTRIES_water] =
//       {22.8154*cm*ABWFF, 28.6144*cm*ABWFF, 35.9923*cm*ABWFF, 45.4086*cm*ABWFF, 57.4650*cm*ABWFF,
//        72.9526*cm*ABWFF, 75*cm*ABWFF,      81.2317*cm*ABWFF, 120.901*cm*ABWFF, 160.243*cm*ABWFF,
//        193.797*cm*ABWFF, 215.045*cm*ABWFF, 227.786*cm*ABWFF, 243.893*cm*ABWFF, 294.113*cm*ABWFF,
//        321.735*cm*ABWFF, 342.931*cm*ABWFF, 362.967*cm*ABWFF, 378.212*cm*ABWFF, 449.602*cm*ABWFF,
//        740.143*cm*ABWFF, 1116.06*cm*ABWFF, 1438.78*cm*ABWFF, 1615.48*cm*ABWFF, 1769.86*cm*ABWFF,
//        2109.67*cm*ABWFF, 2304.13*cm*ABWFF, 2444.97*cm*ABWFF, 3076.83*cm*ABWFF, 4901.5*cm*ABWFF,
//        6666.57*cm*ABWFF, 7873.95*cm*ABWFF, 9433.81*cm*ABWFF, 10214.5*cm*ABWFF, 10845.8*cm*ABWFF,
//        15746.9*cm*ABWFF, 20201.8*cm*ABWFF, 22025.8*cm*ABWFF, 21142.2*cm*ABWFF, 15083.9*cm*ABWFF,
//        11751*cm*ABWFF,   8795.34*cm*ABWFF, 8741.23*cm*ABWFF, 7102.37*cm*ABWFF, 6060.68*cm*ABWFF,
//        4498.56*cm*ABWFF, 3039.56*cm*ABWFF, 2232.2*cm*ABWFF,  1938*cm*ABWFF,    1811.58*cm*ABWFF,
//        1610.32*cm*ABWFF, 1338.7*cm*ABWFF,  1095.3*cm*ABWFF,  977.525*cm*ABWFF, 965.258*cm*ABWFF,
//        1082.86*cm*ABWFF, 876.434*cm*ABWFF, 633.723*cm*ABWFF, 389.87*cm*ABWFF,  142.011*cm*ABWFF
//       };

  //  G4double ABSORPTION_water[NUMENTRIES_water] = //old
//      { 22.8154*cm*ABWFF, 28.6144*cm*ABWFF, 35.9923*cm*ABWFF, 45.4086*cm*ABWFF, 57.4650*cm*ABWFF,
//        72.9526*cm*ABWFF, 92.9156*cm*ABWFF, 118.737*cm*ABWFF, 152.255*cm*ABWFF, 195.925*cm*ABWFF,
//        202.429*cm*ABWFF, 224.719*cm*ABWFF, 236.407*cm*ABWFF, 245.700*cm*ABWFF, 289.017*cm*ABWFF,
//        305.810*cm*ABWFF, 316.456*cm*ABWFF, 326.797*cm*ABWFF, 347.222*cm*ABWFF, 414.938*cm*ABWFF,
//        636.943*cm*ABWFF, 934.579*cm*ABWFF, 1245.33*cm*ABWFF, 1402.52*cm*ABWFF, 1550.39*cm*ABWFF,
//        1745.20*cm*ABWFF, 1883.24*cm*ABWFF, 2016.13*cm*ABWFF, 2442.18*cm*ABWFF, 3831.28*cm*ABWFF,
//        4652.89*cm*ABWFF, 5577.04*cm*ABWFF, 6567.08*cm*ABWFF, 7559.88*cm*ABWFF, 8470.06*cm*ABWFF,
//        9205.54*cm*ABWFF, 9690.95*cm*ABWFF, 9888.36*cm*ABWFF, 9804.50*cm*ABWFF, 9482.17*cm*ABWFF,
//        8982.77*cm*ABWFF, 8369.39*cm*ABWFF, 7680.31*cm*ABWFF, 6902.11*cm*ABWFF, 6183.84*cm*ABWFF,
//        5522.27*cm*ABWFF, 4914.33*cm*ABWFF, 4357.09*cm*ABWFF, 3847.72*cm*ABWFF, 3383.51*cm*ABWFF,
//        2961.81*cm*ABWFF, 2580.08*cm*ABWFF, 2235.83*cm*ABWFF, 1926.66*cm*ABWFF, 1650.21*cm*ABWFF,
//        1404.21*cm*ABWFF, 1186.44*cm*ABWFF, 994.742*cm*ABWFF, 827.027*cm*ABWFF, 681.278*cm*ABWFF};

    
 //   G4double ABSORPTION_water[NUMENTRIES_water] = //new
//      {25.3504*cm, 31.7938*cm, 39.9915*cm, 50.454*cm, 63.85*cm, 
//       81.0584*cm, 103.24*cm, 131.93*cm, 169.172*cm, 217.694*cm, 
//       224.921*cm, 249.688*cm, 262.674*cm, 273*cm, 321.13*cm, 339.789*cm,
//       351.617*cm, 363.108*cm, 385.802*cm, 461.042*cm, 707.714*cm, 
//       1038.42*cm, 1383.7*cm, 1558.36*cm, 1722.65*cm, 1939.11*cm, 
//       2092.49*cm, 2240.14*cm, 2962.96*cm, 4967.03*cm, 6368.58*cm, 
//       8207.56*cm, 10634.2*cm, 13855.3*cm, 18157.3*cm, 23940.2*cm, 
//       31766*cm, 42431.6*cm, 57074.9*cm, 77335.9*cm, 105598*cm, 
//       145361*cm, 192434*cm, 183898*cm, 176087*cm, 168913*cm, 162301*cm, 
//       156187*cm, 150516*cm, 145243*cm, 140327*cm, 135733*cm, 131430*cm, 
//       127392*cm, 123594*cm, 120016*cm, 116640*cm, 113448*cm, 110426*cm, 
//       107562*cm};


   // M Fechner: Rayleigh scattering -- as of version 4.6.2 of GEANT,
   // one may use one's own Rayleigh scattering lengths (the buffer is no
   // longer overwritten for "water", see 4.6.2 release notes)

   // RAYFF = 1/ARAS, for those of you who know SKdetsim...
   // actually that's not quite right because the scattering models
   // are different; in G4 there is no scattering depolarization
   // std value at SK = 0.6. But Mie scattering is implemented
   // in SKdetsim and not in G4

   
  // april 2005 : reduced reflections, let's increase scattering...
  // sep 09: for the large detector like superK the old values are muc better
   //G4double RAYFF = 1.0/1.65;  //old
   //    G4double RAYFF = 1.0/1.5;  

   G4double RAYFF = 0.625;

   // Get from the tuning parameters
   RAYFF = WCSimTuningParams->GetRayff();
   //    printf("RAYFF: %f\n",RAYFF);

   //T. Akiri: Values from Skdetsim 
  //  G4double RAYLEIGH_water[NUMENTRIES_water] = {
//      386929*cm*RAYFF,  366249*cm*RAYFF,  346398*cm*RAYFF,  327355*cm*RAYFF,  309097*cm*RAYFF,  
//      291603*cm*RAYFF,  274853*cm*RAYFF,  258825*cm*RAYFF,  243500*cm*RAYFF,  228856*cm*RAYFF,  
//      214873*cm*RAYFF,  201533*cm*RAYFF,  188816*cm*RAYFF,  176702*cm*RAYFF,  165173*cm*RAYFF,
//      154210*cm*RAYFF,  143795*cm*RAYFF,  133910*cm*RAYFF,  124537*cm*RAYFF,  115659*cm*RAYFF,  
//      107258*cm*RAYFF, 99318.2*cm*RAYFF, 91822.2*cm*RAYFF,   84754*cm*RAYFF, 78097.3*cm*RAYFF, 
//      71836.5*cm*RAYFF,   65956*cm*RAYFF, 60440.6*cm*RAYFF, 55275.4*cm*RAYFF, 50445.6*cm*RAYFF,
//      45937*cm*RAYFF, 41735.2*cm*RAYFF, 37826.6*cm*RAYFF, 34197.6*cm*RAYFF, 30834.9*cm*RAYFF, 
//      27725.4*cm*RAYFF, 24856.6*cm*RAYFF, 22215.9*cm*RAYFF, 19791.3*cm*RAYFF, 17570.9*cm*RAYFF,   
//      15543*cm*RAYFF, 13696.6*cm*RAYFF, 12020.5*cm*RAYFF, 10504.1*cm*RAYFF, 9137.15*cm*RAYFF,
//      7909.45*cm*RAYFF,  6811.3*cm*RAYFF, 5833.25*cm*RAYFF,  4966.2*cm*RAYFF, 4201.36*cm*RAYFF, 
//      3530.28*cm*RAYFF, 2944.84*cm*RAYFF, 2437.28*cm*RAYFF, 2000.18*cm*RAYFF,  1626.5*cm*RAYFF, 
//      1309.55*cm*RAYFF, 1043.03*cm*RAYFF, 821.016*cm*RAYFF,  637.97*cm*RAYFF, 488.754*cm*RAYFF
//    };


   //Minfang' new scattering length for WA-LS
   G4double RAYLEIGH_water[NUMENTRIES_water] = {
     167024*cm*RAYFF, 158727*cm*RAYFF, 150742*cm*RAYFF, 143062*cm*RAYFF, 135680*cm*RAYFF, 
     128587*cm*RAYFF, 121776*cm*RAYFF, 115239*cm*RAYFF, 108969*cm*RAYFF, 102959*cm*RAYFF, 
     97200.4*cm*RAYFF, 91686.9*cm*RAYFF, 86411.3*cm*RAYFF, 81366.8*cm*RAYFF, 76546.4*cm*RAYFF, 
     71943.5*cm*RAYFF, 67551.3*cm*RAYFF, 63363.4*cm*RAYFF, 59373.2*cm*RAYFF, 55574.6*cm*RAYFF, 
     51961.2*cm*RAYFF, 48527*cm*RAYFF, 45265.9*cm*RAYFF, 42171.9*cm*RAYFF, 39239.4*cm*RAYFF, 
     36462.5*cm*RAYFF, 33835.7*cm*RAYFF, 31353.4*cm*RAYFF, 29010.3*cm*RAYFF, 26801*cm*RAYFF, 
     24720.4*cm*RAYFF, 22763.4*cm*RAYFF, 20924.9*cm*RAYFF, 19200.1*cm*RAYFF, 17584.2*cm*RAYFF, 
     16072.5*cm*RAYFF, 14660.4*cm*RAYFF, 13343.5*cm*RAYFF, 12117.3*cm*RAYFF, 10977.7*cm*RAYFF, 
     9920.42*cm*RAYFF, 8941.41*cm*RAYFF, 8036.71*cm*RAYFF, 7202.47*cm*RAYFF, 6434.93*cm*RAYFF, 
     5730.43*cm*RAYFF, 5085.43*cm*RAYFF, 4496.47*cm*RAYFF, 3960.21*cm*RAYFF, 3473.41*cm*RAYFF, 
     3032.94*cm*RAYFF, 2635.75*cm*RAYFF, 2278.91*cm*RAYFF, 1959.59*cm*RAYFF, 1675.06*cm*RAYFF, 
     1422.71*cm*RAYFF, 1200*cm*RAYFF, 1004.53*cm*RAYFF, 830*cm*RAYFF, 686.106*cm*RAYFF
   };



  //  G4double RAYLEIGH_water[NUMENTRIES_water] = {
//      167024.4*cm*RAYFF, 158726.7*cm*RAYFF, 150742*cm*RAYFF,
//      143062.5*cm*RAYFF, 135680.2*cm*RAYFF, 128587.4*cm*RAYFF,
//      121776.3*cm*RAYFF, 115239.5*cm*RAYFF, 108969.5*cm*RAYFF,
//      102958.8*cm*RAYFF, 97200.35*cm*RAYFF, 91686.86*cm*RAYFF,
//      86411.33*cm*RAYFF, 81366.79*cm*RAYFF, 76546.42*cm*RAYFF,
//      71943.46*cm*RAYFF, 67551.29*cm*RAYFF, 63363.36*cm*RAYFF,
//      59373.25*cm*RAYFF, 55574.61*cm*RAYFF, 51961.24*cm*RAYFF,
//      48527.00*cm*RAYFF, 45265.87*cm*RAYFF, 42171.94*cm*RAYFF,
//      39239.39*cm*RAYFF, 36462.50*cm*RAYFF, 33835.68*cm*RAYFF,
//      31353.41*cm*RAYFF, 29010.30*cm*RAYFF, 26801.03*cm*RAYFF,
//      24720.42*cm*RAYFF, 22763.36*cm*RAYFF, 20924.88*cm*RAYFF,
//      19200.07*cm*RAYFF, 17584.16*cm*RAYFF, 16072.45*cm*RAYFF,
//      14660.38*cm*RAYFF, 13343.46*cm*RAYFF, 12117.33*cm*RAYFF,
//      10977.70*cm*RAYFF, 9920.416*cm*RAYFF, 8941.407*cm*RAYFF,
//      8036.711*cm*RAYFF, 7202.470*cm*RAYFF, 6434.927*cm*RAYFF,
//      5730.429*cm*RAYFF, 5085.425*cm*RAYFF, 4496.467*cm*RAYFF,
//      3960.210*cm*RAYFF, 3473.413*cm*RAYFF, 3032.937*cm*RAYFF,
//      2635.746*cm*RAYFF, 2278.907*cm*RAYFF, 1959.588*cm*RAYFF,
//      1675.064*cm*RAYFF, 1422.710*cm*RAYFF, 1200.004*cm*RAYFF,
//      1004.528*cm*RAYFF, 833.9666*cm*RAYFF, 686.1063*cm*RAYFF
//    };

    //assume 100 times larger than the rayleigh scattering for now. 
   G4double MIE_water[NUMENTRIES_water] = {
     167024.4*m*RAYFF, 158726.7*m*RAYFF, 150742*m*RAYFF,
     143062.5*m*RAYFF, 135680.2*m*RAYFF, 128587.4*m*RAYFF,
     121776.3*m*RAYFF, 115239.5*m*RAYFF, 108969.5*m*RAYFF,
     102958.8*m*RAYFF, 97200.35*m*RAYFF, 91686.86*m*RAYFF,
     86411.33*m*RAYFF, 81366.79*m*RAYFF, 76546.42*m*RAYFF,
     71943.46*m*RAYFF, 67551.29*m*RAYFF, 63363.36*m*RAYFF,
     59373.25*m*RAYFF, 55574.61*m*RAYFF, 51961.24*m*RAYFF,
     48527.00*m*RAYFF, 45265.87*m*RAYFF, 42171.94*m*RAYFF,
     39239.39*m*RAYFF, 36462.50*m*RAYFF, 33835.68*m*RAYFF,
     31353.41*m*RAYFF, 29010.30*m*RAYFF, 26801.03*m*RAYFF,
     24720.42*m*RAYFF, 22763.36*m*RAYFF, 20924.88*m*RAYFF,
     19200.07*m*RAYFF, 17584.16*m*RAYFF, 16072.45*m*RAYFF,
     14660.38*m*RAYFF, 13343.46*m*RAYFF, 12117.33*m*RAYFF,
     10977.70*m*RAYFF, 9920.416*m*RAYFF, 8941.407*m*RAYFF,
     8036.711*m*RAYFF, 7202.470*m*RAYFF, 6434.927*m*RAYFF,
     5730.429*m*RAYFF, 5085.425*m*RAYFF, 4496.467*m*RAYFF,
     3960.210*m*RAYFF, 3473.413*m*RAYFF, 3032.937*m*RAYFF,
     2635.746*m*RAYFF, 2278.907*m*RAYFF, 1959.588*m*RAYFF,
     1675.064*m*RAYFF, 1422.710*m*RAYFF, 1200.004*m*RAYFF,
     1004.528*m*RAYFF, 833.9666*m*RAYFF, 686.1063*m*RAYFF
   };
   
   

   G4double MIE_water_const[3]={0.99,0.99,0.8};// gforward, gbackward, forward backward ratio


   //From SFDETSIM
   /*
   G4double RINDEX_glass[NUMENTRIES] =
     { 1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
       1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
       1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
       1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
       1.600, 1.600, 1.600, 1.600 };
   */
   // M Fechner : unphysical, I want to reduce reflections
   // back to the old value 1.55

   G4double RINDEX_glass[NUMENTRIES_water] =
     { 1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
       1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
       1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
       1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
       1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
       1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
       1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
       1.600, 1.600, 1.600, 1.600, 1.600, 1.600, 1.600,
       1.600, 1.600 }; 

   //G4double RINDEX_blacksheet[NUMENTRIES] =
   //{ 2.500, 2.500, 2.500, 2.500, 2.500, 2.500, 2.500,
   //  2.500, 2.500, 2.500, 2.500, 2.500, 2.500, 2.500,
   //  2.500, 2.500, 2.500, 2.500, 2.500, 2.500, 2.500,
   //  2.500, 2.500, 2.500, 2.500, 2.500, 2.500, 2.500,
   //  2.500, 2.500, 2.500, 2.500 };
   
   
   //G4double ABSORPTION1[NUMENTRIES] =
   //{344.8*cm,  408.2*cm,  632.9*cm,  917.4*cm, 1234.6*cm, 1388.9*cm,
   // 1515.2*cm, 1724.1*cm, 1886.8*cm, 2000.0*cm, 2631.6*cm, 3571.4*cm,
   // 4545.5*cm, 4761.9*cm, 5263.2*cm, 5263.2*cm, 5555.6*cm, 5263.2*cm,
   // 5263.2*cm, 4761.9*cm, 4545.5*cm, 4166.7*cm, 3703.7*cm, 3333.3*cm,
   // 3000.0*cm, 2850.0*cm, 2700.0*cm, 2450.0*cm, 2200.0*cm, 1950.0*cm,
   // 1750.0*cm, 1450.0*cm };
   
   /*   
   G4double ABSORPTION_glass[NUMENTRIES] = 
     { 100.0*cm, 110.0*cm, 120.0*cm, 130.0*cm, 140.0*cm, 150.0*cm, 160.0*cm,
       165.0*cm, 170.0*cm, 175.0*cm, 180.0*cm, 185.0*cm, 190.0*cm, 195.0*cm,
       200.0*cm, 200.0*cm, 200.0*cm, 200.0*cm, 200.0*cm, 195.0*cm, 190.0*cm,
       185.0*cm, 180.0*cm, 175.0*cm, 170.0*cm, 160.0*cm, 150.0*cm, 140.0*cm,
       130.0*cm, 120.0*cm, 110.0*cm, 100.0*cm };
   */
   // M Fechner : the quantum efficiency already takes glass abs into account

   G4double ABSORPTION_glass[NUMENTRIES_water]= 
     { 1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm, 1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
       1.0e9*cm, 1.0e9*cm };
   
   G4double BLACKABS_blacksheet[NUMENTRIES_water] =
     { 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 
       1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 
       1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm, 1.0e-9*cm,
       1.0e-9*cm, 1.0e-9*cm};
   
   
   //The following reflectivity for blacksheet is obtained from skdetsim
   //There is a fudge factor of 2.7 for the reflectivity of blacksheet
   //depending on whether SK1 or SK2 simulation is used.  
   //The BlackSheetFudgeFactor is set to true if you want to use the 
   //SK2 values, false if not.
   //G4double SK1SK2FF = 1.0;
   //G4bool BlackSheetFudgeFactor=false;
   //G4bool BlackSheetFudgeFactor=true;
   //   if (BlackSheetFudgeFactor) SK1SK2FF=SK1SK2FF*2.7;
   //if (BlackSheetFudgeFactor) SK1SK2FF=SK1SK2FF*1.55;

   //July 1, 2010, F. Beroz: changed SK1SK2FF to BSRFF to avoid confusion,
   // since this parameter is not from SK.

   G4double BSRFF = 1.0;
   
   // Get from the tuning parameters
   BSRFF = WCSimTuningParams->GetBsrff();

   G4double REFLECTIVITY_blacksheet[NUMENTRIES_water] =
     { 0.055*BSRFF, 0.055*BSRFF, 
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 
       0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 0.055*BSRFF, 
       0.055*BSRFF, 0.057*BSRFF, 0.059*BSRFF, 0.060*BSRFF, 
       0.059*BSRFF, 0.058*BSRFF, 0.057*BSRFF, 0.055*BSRFF, 
       0.050*BSRFF, 0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF, 
       0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF,
       0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF,
       0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF,
       0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF, 0.045*BSRFF,
       0.045*BSRFF, 0.045*BSRFF };

   /*
   G4double REFLECTIVITY_blacksheet[NUMENTRIES] =
     { 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.057*SK1SK2FF, 0.059*SK1SK2FF, 0.060*SK1SK2FF, 
       0.059*SK1SK2FF, 0.058*SK1SK2FF, 0.057*SK1SK2FF, 0.055*SK1SK2FF, 
       0.050*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 
       0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF,
       0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF };
   */
   
   /*
   G4double REFLECTIVITY_blacksheet[NUMENTRIES_water] =
     { 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 0.055*SK1SK2FF, 
       0.055*SK1SK2FF, 0.057*SK1SK2FF, 0.059*SK1SK2FF, 0.060*SK1SK2FF, 
       0.059*SK1SK2FF, 0.058*SK1SK2FF, 0.057*SK1SK2FF, 0.055*SK1SK2FF, 
       0.050*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 
       0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF,
       0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF,
       0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF,
       0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF, 0.045*SK1SK2FF ,
       0.045*SK1SK2FF, 0.045*SK1SK2FF };
   */

   //utter fiction at this stage
   G4double EFFICIENCY[NUMENTRIES_water] =
     { 0.001*m };
      
   //utter fiction at this stage, does not matter
   G4double RAYLEIGH_air[NUMENTRIES_water] =
     { 0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
       0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m};
      

   G4double RAYLEIGH_vacuum[NUMENTRIES_water] =
     { 10000*m,10000*m,10000*m,10000*m,10000*m,10000*m,
       10000*m,10000*m,10000*m,10000*m,10000*m,10000*m,
       10000*m,10000*m,10000*m,10000*m,10000*m,10000*m,
       10000*m,10000*m,10000*m,10000*m,10000*m,10000*m,
       10000*m,10000*m,10000*m,10000*m,10000*m,10000*m,
       10000*m,10000*m,10000*m,10000*m,10000*m,10000*m,
       10000*m,10000*m,10000*m,10000*m,10000*m,10000*m,
       10000*m,10000*m,10000*m,10000*m,10000*m,10000*m,
       10000*m,10000*m,10000*m,10000*m,10000*m,10000*m,
       10000*m,10000*m,10000*m,10000*m,10000*m,10000*m};

    //utter fiction at this stage, does not matter
   G4double MIE_air[NUMENTRIES_water] =
     { 0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,
       0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,
       0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,
       0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,
       0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,
       0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,
       0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,
       0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,
       0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,
       0.1*m,0.1*m,0.1*m,0.1*m,0.1*m,0.1*m};

   G4double wls_factor = 2.;

   // absorption length for acrylic
   G4double ABSORPTION_WLS[NUMENTRIES_water]={12.0106*wls_factor*cm, 12.0106*wls_factor*cm, 8.15463*wls_factor*cm, 8.1707*wls_factor*cm, 4.97081*wls_factor*cm, 8.15463*wls_factor*cm, 
					      8.15463*wls_factor*cm, 8.15463*wls_factor*cm, 6.17999*wls_factor*cm, 6.17284*wls_factor*cm, 6.17284*wls_factor*cm, 6.17284*wls_factor*cm, 
					      6.17284*wls_factor*cm, 4.15611*wls_factor*cm, 6.17284*wls_factor*cm, 6.16886*wls_factor*cm, 6.17131*wls_factor*cm, 4.96598*wls_factor*cm, 
					      4.15554*wls_factor*cm, 4.15543*wls_factor*cm, 4.96598*wls_factor*cm, 4.15386*wls_factor*cm, 4.15254*wls_factor*cm, 4.15301*wls_factor*cm, 
					      3.57002*wls_factor*cm, 3.57002*wls_factor*cm, 3.13035*wls_factor*cm, 3.13008*wls_factor*cm, 3.13008*wls_factor*cm, 2.78668*wls_factor*cm, 
					      2.78668*wls_factor*cm, 2.51134*wls_factor*cm, 2.51117*wls_factor*cm, 2.51109*wls_factor*cm, 2.28524*wls_factor*cm, 2.28489*wls_factor*cm, 
					      2.09661*wls_factor*cm, 1.93674*wls_factor*cm, 1.93674*wls_factor*cm, 1.79953*wls_factor*cm, 1.68047*wls_factor*cm, 1.48418*wls_factor*cm, 
					      1.40227*wls_factor*cm, 1.32881*wls_factor*cm, 1.20295*wls_factor*cm, 1.0533*wls_factor*cm, 0.936758*wls_factor*cm, 0.843439*wls_factor*cm, 
					      0.744562*wls_factor*cm, 0.666425*wls_factor*cm, 0.603125*wls_factor*cm, 0.527904*wls_factor*cm, 0.469379*wls_factor*cm, 0.408924*wls_factor*cm, 
					      0.333711*wls_factor*cm, 0.251208*wls_factor*cm, 0.181283*wls_factor*cm, 0.140244*wls_factor*cm, 0.110381*wls_factor*cm, 0.0695699*wls_factor*cm};

   //index of refraction of acrylic
   G4double RINDEX_WLS[NUMENTRIES_water]={1.48796, 1.48811, 1.48827, 1.48842, 1.48858, 1.48873, 
					  1.48889, 1.48905, 1.4892, 1.48936, 1.48952, 1.48982, 
					  1.49011, 1.49041, 1.4907, 1.491, 1.49129, 1.49159, 
					  1.49188, 1.49218, 1.49247, 1.49294, 1.49343, 1.49392, 
					  1.49441, 1.495, 1.49567, 1.49634, 1.497, 1.49767, 
					  1.49834, 1.49903, 1.49975, 1.50047, 1.50119, 1.5019, 
					  1.50301, 1.50439, 1.50577, 1.50783, 1.51063, 1.51343, 
					  1.51622, 1.51902, 1.52182, 1.52462, 1.52742, 1.53021, 
					  1.53301, 1.53583, 1.54504, 1.55425, 1.56345, 1.57266, 
					  1.58187, 1.59107, 1.60028, 1.60949, 1.61869, 1.6279};
   
   G4double wls_abs_factor = 0.7;

   G4double wls_abs[NUMENTRIES_water]={1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
				       1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
				       1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
				       1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
				       1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
				       1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
				       1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
				       1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 0.0226002*wls_abs_factor*cm, 0.01*wls_abs_factor*cm, 
				       0.01*wls_abs_factor*cm, 0.01*wls_abs_factor*cm, 0.01*wls_abs_factor*cm, 0.0170268*wls_abs_factor*cm, 0.0356053*wls_abs_factor*cm, 0.0634218*wls_abs_factor*cm, 
				       0.156279*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm};

   G4double wls_emi[NUMENTRIES_water]={0, 0, 0, 0, 0, 0, 
				       0, 0, 0, 0, 0, 0, 
				       0, 0, 0, 0, 0, 0, 
				       0, 0, 0, 0, 0, 0, 
				       0, 0, 0, 0, 0, 0, 
				       0, 0, 0.0199944, 0.0599985, 0.0999964, 0.159989, 
				       0.21999, 0.299984, 0.399986, 0.499987, 0.833304, 1.24995, 
				       1.59997, 1.5, 1.50008, 0.9, 0.266694, 0, 
				       0, 0, 0, 0, 0, 0, 
				       0, 0, 0, 0, 0, 0};

   G4double MIE_air_const[3]={0.99,0.99,0.8};// gforward, gbackward, forward backward ratio

   //Not used yet, fictional values
   //G4double SPECULARLOBECONSTANT1[NUMENTRIES] =
   //{ 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001 };
   
   //Not used yet, fictional values
   //G4double SPECULARSPIKECONSTANT1[NUMENTRIES] =
   //{ 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001 };
   
   //Not used yet, fictional values
   //G4double BACKSCATTERCONSTANT1[NUMENTRIES] =
   //{ 0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001, 0.001, 0.001, 0.001,
   //  0.001, 0.001, 0.001, 0.001 };
   
   G4double EFFICIENCY_blacksheet[NUMENTRIES_water] =
    { 0.0 };

   //	------------- Surfaces --------------

   OpWaterBSSurface =
     new G4OpticalSurface("WaterBSCellSurface");

   OpWaterBSSurface->SetType(dielectric_dielectric);
   OpWaterBSSurface->SetModel(unified);
   OpWaterBSSurface->SetFinish(groundfrontpainted);
   OpWaterBSSurface->SetSigmaAlpha(0.1);

   const G4int NUM = 2;
   //   G4double PP[NUM] =
   //{ 2.038E-9*GeV, 4.144E-9*GeV };

   G4double PP[NUM] = { 1.4E-9*GeV,6.2E-9*GeV};
   G4double RINDEX_blacksheet[NUM] =
     { 1.6, 1.6 };

   G4double SPECULARLOBECONSTANT[NUM] =
     { 0.3, 0.3 };
   G4double SPECULARSPIKECONSTANT[NUM] =
     { 0.2, 0.2 };
   G4double BACKSCATTERCONSTANT[NUM] =
     { 0.2, 0.2 };

   OpGlassCathodeSurface =
     new G4OpticalSurface("GlassCathodeSurface");
   OpGlassCathodeSurface->SetType(dielectric_dielectric);
   OpGlassCathodeSurface->SetModel(unified);
   //   OpGlassCathodeSurface->SetFinish(groundbackpainted);
   OpGlassCathodeSurface->SetFinish(polished);
   //OpGlassCathodeSurface->SetSigmaAlpha(0.002);
   // was 1.0
   // totally unphysical anyway 
   G4double RINDEX_cathode[NUM] =
     { 1.0, 1.0 };
   /*   
   G4double SPECULARLOBECONSTANT_glasscath[NUM] =
     { 1.0, 1.0 };
     // { 0.3, 0.3 };
   G4double SPECULARSPIKECONSTANT_glasscath[NUM] =
     { 0.0, 0.0 };
     //     { 0.2, 0.2 };
   G4double BACKSCATTERCONSTANT_glasscath[NUM] =
     {0.0, 0.0};
   //     { 0.2, 0.2 };
   */


   G4double RGCFF = 0.0;
   RGCFF = WCSimTuningParams->GetRgcff();
 
   G4double REFLECTIVITY_glasscath[NUM] =
     //{ 0.0+RGCFF, 0.0+RGCFF };
     //{ RGCFF, RGCFF };
     //{ 0.0, 0.0 };
     { 1.0*RGCFF, 1.0*RGCFF };


   G4double EFFICIENCY_glasscath[NUM] =
     { 0.0, 0.0 };


   // jl145 ----
   //
   OpWaterTySurface =
     new G4OpticalSurface("WaterTyCellSurface");

   OpWaterTySurface->SetType(dielectric_dielectric);
   OpWaterTySurface->SetModel(unified);
   OpWaterTySurface->SetFinish(groundbackpainted); //a guess, but seems to work
   OpWaterTySurface->SetSigmaAlpha(0.5); //cf. A. Chavarria's ~30deg

   G4double RINDEX_tyvek[NUM] =
     { 1.5, 1.5 }; // polyethylene permittivity is ~2.25
   G4double TySPECULARLOBECONSTANT[NUM] =
     { 0.75, 0.75 }; // crudely estimated from A. Chavarria's thesis
   G4double TySPECULARSPIKECONSTANT[NUM] =
     { 0.0, 0.0 };
   G4double TyBACKSCATTERCONSTANT[NUM] =
     { 0.0, 0.0 };
   // Lambertian prob is therefore 0.25

   G4double TyREFLECTIVITY[NUM] =
     { 0.94, 0.94 }; //cf. DuPont
   //
   // ----

   G4double ppo_ev[181]={2.48*eV,2.48497*eV,2.48996*eV,2.49497*eV,2.5*eV,2.50505*eV,2.51012*eV,2.51521*eV,2.52033*eV,2.52546*eV,2.53061*eV,2.53579*eV,2.54098*eV,2.5462*eV,2.55144*eV,2.5567*eV,2.56198*eV,2.56729*eV,2.57261*eV,2.57796*eV,2.58333*eV,2.58873*eV,2.59414*eV,2.59958*eV,2.60504*eV,2.61053*eV,2.61603*eV,2.62156*eV,2.62712*eV,2.6327*eV,2.6383*eV,2.64392*eV,2.64957*eV,2.65525*eV,2.66094*eV,2.66667*eV,2.67241*eV,2.67819*eV,2.68398*eV,2.6898*eV,2.69565*eV,2.70153*eV,2.70742*eV,2.71335*eV,2.7193*eV,2.72527*eV,2.73128*eV,2.73731*eV,2.74336*eV,2.74945*eV,2.75556*eV,2.76169*eV,2.76786*eV,2.77405*eV,2.78027*eV,2.78652*eV,2.79279*eV,2.7991*eV,2.80543*eV,2.81179*eV,2.81818*eV,2.8246*eV,2.83105*eV,2.83753*eV,2.84404*eV,2.85057*eV,2.85714*eV,2.86374*eV,2.87037*eV,2.87703*eV,2.88372*eV,2.89044*eV,2.8972*eV,2.90398*eV,2.9108*eV,2.91765*eV,2.92453*eV,2.93144*eV,2.93839*eV,2.94537*eV,2.95238*eV,2.95943*eV,2.96651*eV,2.97362*eV,2.98077*eV,2.98795*eV,2.99517*eV,3.00242*eV,3.00971*eV,3.01703*eV,3.02439*eV,3.03178*eV,3.03922*eV,3.04668*eV,3.05419*eV,3.06173*eV,3.06931*eV,3.07692*eV,3.08458*eV,3.09227*eV,3.1*eV,3.10777*eV,3.11558*eV,3.12343*eV,3.13131*eV,3.13924*eV,3.14721*eV,3.15522*eV,3.16327*eV,3.17136*eV,3.17949*eV,3.18766*eV,3.19588*eV,3.20413*eV,3.21244*eV,3.22078*eV,3.22917*eV,3.2376*eV,3.24607*eV,3.25459*eV,3.26316*eV,3.27177*eV,3.28042*eV,3.28912*eV,3.29787*eV,3.30667*eV,3.31551*eV,3.3244*eV,3.33333*eV,3.34232*eV,3.35135*eV,3.36043*eV,3.36957*eV,3.37875*eV,3.38798*eV,3.39726*eV,3.40659*eV,3.41598*eV,3.42541*eV,3.4349*eV,3.44444*eV,3.45404*eV,3.46369*eV,3.47339*eV,3.48315*eV,3.49296*eV,3.50282*eV,3.51275*eV,3.52273*eV,3.53276*eV,3.54286*eV,3.55301*eV,3.56322*eV,3.57349*eV,3.58382*eV,3.5942*eV,3.60465*eV,3.61516*eV,3.62573*eV,3.63636*eV,3.64706*eV,3.65782*eV,3.66864*eV,3.67953*eV,3.69048*eV,3.70149*eV,3.71257*eV,3.72372*eV,3.73494*eV,3.74622*eV,3.75758*eV,3.769*eV,3.78049*eV,3.79205*eV,3.80368*eV,3.81538*eV,3.82716*eV,3.83901*eV,3.85093*eV,3.86293*eV,3.875*eV};
   G4double ppo_emm[181]={2541.54,3124.19,3061.53,3037.8,3698.67,2682.4,3551.49,2759.86,3549.34,3434.85,3779.92,4100.42,3573.68,4359.18,4937.79,3916.84,4559.36,4359.95,4504.64,4744.8,5043.93,5654.4,4739.68,5577.59,4973.95,5114.24,5886.61,6572.44,7289.24,6713.17,7610.77,7296.81,7752.18,7209.37,8106.91,8652.75,8943.43,9692.26,10176.6,10746.3,10344.3,11747.2,12292.9,12706.5,13556,14827.9,13919.1,15198.1,16352.7,18168,17326.5,19102.6,19121.2,19297.9,20172.8,21279.9,22889.7,22660.3,24429.1,25578.1,26114.2,27410.1,28629.8,30848.9,31421.5,33385.8,34967.8,36638.8,39489,41119.8,41312.2,46175.8,47118.9,48920,51622.4,54791.6,58833.2,62620.6,64913.7,65227.5,69206.7,71265.4,74701.7,75612.7,78768.2,82629.5,86460.1,89618.8,93470.3,97671.7,100672,106714,112347,116281,126265,129929,136126,144722,153333,158643,163860,174231,178955,183509,191875,197852,202880,205720,206079,210369,213376,219420,223711,230360,240491,246300,265669,275737,289692,303614,318762,343541,355450,356684,365008,364466,371400,373686,365578,363652,364942,357897,345898,337882,328196,326912,332908,349242,364969,388441,409957,445645,470487,484744,490104,470286,449700,417960,389115,367229,342705,309227,277736,246470,219790,213914,205007,213101,218003,215775,199186,160937,117169,70198.4,41187.8,23908.2,15660,10989,7110.49,5920.07,3889.67,3030.48,2725.32,2430.16,2282.4,1926.76,2187.97,1826.96,1279.74,1489.21,1777.18};

   G4MaterialPropertiesTable *myMPT1 = new G4MaterialPropertiesTable();
   // M Fechner : new   ; wider range for lambda
   myMPT1->AddProperty("RINDEX", ENERGY_water, RINDEX1, NUMENTRIES_water);
   myMPT1->AddProperty("ABSLENGTH",ENERGY_water, ABSORPTION_water, NUMENTRIES_water);
   // M Fechner: new, don't let G4 compute it.
   myMPT1->AddProperty("RAYLEIGH",ENERGY_water,RAYLEIGH_water,NUMENTRIES_water);
   
   myMPT1->AddProperty("WLSABSLENGTH",ENERGY_water,wls_abs,NUMENTRIES_water);
   myMPT1->AddProperty("WLSCOMPONENT",ppo_ev,ppo_emm,181);
   myMPT1->AddConstProperty("WLSTIMECONSTANT",1.5*ns);

   myMPT1->AddConstProperty("SCINTILLATIONYIELD",90./MeV);
   //myMPT1->AddConstProperty("SCINTILLATIONYIELD",0./MeV);
   //myMPT1->AddConstProperty("SCINTILLATIONYIELD",1800./MeV);
   myMPT1->AddConstProperty("RESOLUTIONSCALE",1.0);
   myMPT1->AddConstProperty("FASTTIMECONSTANT",1.23*ns);
   myMPT1->AddConstProperty("SLOWTIMECONSTANT",9.26*ns);
   myMPT1->AddConstProperty("YIELDRATIO",0.26);
   // myMPT1->AddConstProperty("YIELDRATIO",1.0);
   

   myMPT1->AddProperty("FASTCOMPONENT",ppo_ev,ppo_emm,181);
   myMPT1->AddProperty("SLOWCOMPONENT",ppo_ev,ppo_emm,181);

   Water->GetIonisation()->SetBirksConstant(0.124*mm/MeV);
   //Water->GetIonisation()->SetBirksConstant(0.*mm/MeV);

     // myMPT1->AddProperty("MIEHG",ENERGY_water,MIE_water,NUMENTRIES_water);
//    myMPT1->AddConstProperty("MIEHG_FORWARD",MIE_water_const[0]);
//    myMPT1->AddConstProperty("MIEHG_BACKWARD",MIE_water_const[1]);
//    myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO",MIE_water_const[2]);

   Water->SetMaterialPropertiesTable(myMPT1);
   //Gd doped water has the same optical properties as pure water
   DopedWater->SetMaterialPropertiesTable(myMPT1);
   // myMPT1->DumpTable();
   
   G4MaterialPropertiesTable *myMPT2 = new G4MaterialPropertiesTable();
   myMPT2->AddProperty("RINDEX", ENERGY_water, RINDEX_air, NUMENTRIES_water);
   // M Fechner : what is that ?????
   myMPT2->AddProperty("ABSLENGTH", ENERGY_water, BLACKABS_blacksheet, NUMENTRIES_water);
   myMPT2->AddProperty("RAYLEIGH",ENERGY_water, RAYLEIGH_air, NUMENTRIES_water);

      // myMPT2->AddProperty("MIEHG",ENERGY_water,MIE_air,NUMENTRIES_water);
//    myMPT2->AddConstProperty("MIEHG_FORWARD",MIE_air_const[0]);
//    myMPT2->AddConstProperty("MIEHG_BACKWARD",MIE_air_const[1]);
//    myMPT2->AddConstProperty("MIEHG_FORWARD_RATIO",MIE_air_const[2]);


   Air->SetMaterialPropertiesTable(myMPT2);
   
   G4MaterialPropertiesTable *myMPT3 = new G4MaterialPropertiesTable();
   myMPT3->AddProperty("ABSLENGTH", ENERGY_water, BLACKABS_blacksheet, NUMENTRIES_water);
   myMPT3->AddProperty("REFLECTIVITY", ENERGY_water, REFLECTIVITY_blacksheet, NUMENTRIES_water);
   myMPT3->AddProperty("EFFICIENCY",   ENERGY_water, EFFICIENCY, NUMENTRIES_water);
   Plastic->SetMaterialPropertiesTable(myMPT3);
   
   G4MaterialPropertiesTable *myMPT4 = new G4MaterialPropertiesTable();
   myMPT4->AddProperty("ABSLENGTH", ENERGY_water, BLACKABS_blacksheet, NUMENTRIES_water);
   Blacksheet->SetMaterialPropertiesTable(myMPT4);
   
   G4MaterialPropertiesTable *myMPT5 = new G4MaterialPropertiesTable();
   myMPT5->AddProperty("RINDEX", ENERGY_water, RINDEX_glass, NUMENTRIES_water);
   myMPT5->AddProperty("ABSLENGTH",ENERGY_water, ABSORPTION_glass, NUMENTRIES_water);
   Glass->SetMaterialPropertiesTable(myMPT5);
    
   // jl145 ----
   // Abs legnth is same as blacksheet, very small.
   G4MaterialPropertiesTable *myMPT6 = new G4MaterialPropertiesTable();
   myMPT6->AddProperty("ABSLENGTH", ENERGY_water, BLACKABS_blacksheet, NUMENTRIES_water);
   Tyvek->SetMaterialPropertiesTable(myMPT6);


     //acrylic part
   G4MaterialPropertiesTable *myMPT7 = new G4MaterialPropertiesTable();
   myMPT7->AddProperty("RINDEX",ENERGY_water,RINDEX_WLS,NUMENTRIES_water);
   myMPT7->AddProperty("ABSLENGTH",ENERGY_water,ABSORPTION_WLS,NUMENTRIES_water);
   myMPT7->AddProperty("WLSABSLENGTH",ENERGY_water,wls_abs,NUMENTRIES_water);
   myMPT7->AddProperty("WLSCOMPONENT",ppo_ev,ppo_emm,181);
   // XQ 11/28/11 this is for PPO
   // check wether use exponential in the WCSimPhysicsList.cc
   myMPT7->AddConstProperty("WLSTIMECONSTANT",3.5*ns);
   
   acrylic_wls_film->SetMaterialPropertiesTable(myMPT7);


   //	------------- Surfaces --------------

   // Blacksheet
   G4MaterialPropertiesTable *myST1 = new G4MaterialPropertiesTable();
   myST1->AddProperty("RINDEX", ENERGY_water, RINDEX_blacksheet, NUMENTRIES_water);
   myST1->AddProperty("SPECULARLOBECONSTANT", PP, SPECULARLOBECONSTANT, NUM);
   myST1->AddProperty("SPECULARSPIKECONSTANT", PP, SPECULARSPIKECONSTANT, NUM);
   myST1->AddProperty("BACKSCATTERCONSTANT", PP, BACKSCATTERCONSTANT, NUM);
   myST1->AddProperty("REFLECTIVITY", ENERGY_water, REFLECTIVITY_blacksheet, NUMENTRIES_water);
   myST1->AddProperty("EFFICIENCY", ENERGY_water, EFFICIENCY_blacksheet, NUMENTRIES_water);
   OpWaterBSSurface->SetMaterialPropertiesTable(myST1);

   //Glass to Cathode surface inside PMTs
   G4MaterialPropertiesTable *myST2 = new G4MaterialPropertiesTable();
   myST2->AddProperty("RINDEX", PP, RINDEX_cathode, NUM);
   //myST2->AddProperty("RINDEX",ENERGY_water,RINDEX1, NUMENTRIES_water);
   //   myST2->AddProperty("SPECULARLOBECONSTANT", PP, SPECULARLOBECONSTANT_glasscath, NUM);
   //   myST2->AddProperty("SPECULARSPIKECONSTANT", PP, SPECULARSPIKECONSTANT_glasscath, NUM);
   //   myST2->AddProperty("BACKSCATTERCONSTANT", PP, BACKSCATTERCONSTANT_glasscath, NUM);
   myST2->AddProperty("REFLECTIVITY", PP, REFLECTIVITY_glasscath, NUM);
   myST2->AddProperty("EFFICIENCY", PP, EFFICIENCY_glasscath, NUM);
   //myST2->AddProperty("ABSLENGTH", PP, abslength_paint , NUM);
   OpGlassCathodeSurface->SetMaterialPropertiesTable(myST2);

   //Tyvek - jl145
   G4MaterialPropertiesTable *myST3 = new G4MaterialPropertiesTable();
   myST3->AddProperty("RINDEX", PP, RINDEX_tyvek, NUM);
   myST3->AddProperty("SPECULARLOBECONSTANT", PP, TySPECULARLOBECONSTANT, NUM);
   myST3->AddProperty("SPECULARSPIKECONSTANT", PP, TySPECULARSPIKECONSTANT, NUM);
   myST3->AddProperty("BACKSCATTERCONSTANT", PP, TyBACKSCATTERCONSTANT, NUM);
   myST3->AddProperty("REFLECTIVITY", PP, TyREFLECTIVITY, NUM);
   myST3->AddProperty("EFFICIENCY", PP, EFFICIENCY_blacksheet, NUM);
   //use same efficiency as blacksheet, which is 0
   OpWaterTySurface->SetMaterialPropertiesTable(myST3);



   //XQ, 11/29/11 add in the stuff for WLSP
   
       //	------------- Materials -------------
    
   // G4double a, z, density;
   G4double z;
    G4int nelements,natoms;

// Plastic
// 
  G4Element* H = new G4Element("Hydrogen", "H", z=1 , a=1.01*g/mole);
  G4Element* C = new G4Element("Carbon"  , "C" , z=6 , a= 12.01*g/mole);
  G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
  G4Element* Si = new G4Element("Silicon", "Si", z=14, a=28.09*g/mole);
  
 G4Material* Plastic_wlsp = new G4Material("Plastic_wlsp", density=1.032*g/cm3, nelements=2);
  Plastic_wlsp->AddElement(C, natoms=9);
  Plastic_wlsp->AddElement(H, natoms=10);

  G4Material* BC408 = new G4Material("BC408", density=1.032*g/cm3, nelements=2);
  BC408->AddElement(C, natoms=9);
  BC408->AddElement(H, natoms=10);

  G4Material* BC408_high = new G4Material("BC408_high", density=1.032*g/cm3, nelements=2);
  BC408_high->AddElement(C, natoms=9);
  BC408_high->AddElement(H, natoms=10);

  G4Material* BC482 = new G4Material("BC482", density=1.032*g/cm3, nelements=2);
  BC482->AddElement(C, natoms=9);
  BC482->AddElement(H, natoms=10);


  //
// ------------ Generate & Add Material Properties Table ------------
//
  const G4int nEntries_wlsp = 32;

  //this number need to be changed
  

  G4double PhotonEnergy[nEntries_wlsp] =
            { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
              2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
              2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
              2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
              2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
              3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
              3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
              3.760*eV, 3.877*eV, 4.002*eV, 4.201*eV };

  const G4int nEntries2 = 34;

  G4double PhotonEnergy2[nEntries2] =
            { 1.0*eV, 2.068*eV, 2.103*eV, 2.139*eV,
              2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
              2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
              2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
              2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
              3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
              3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
              3.760*eV, 3.877*eV, 4.002*eV, 4.201*eV, 5.0*eV, 7.0*eV};


//-----------Beginning of WLS Plate Properties------------------

G4double ri = 1.59; // The actual refractive index will be a 
	              //function of wavelength, this will be changed 
	              //in the future

G4double RefractiveIndexPlastic[nEntries2] =
            { ri, ri, ri, ri, ri,
              ri, ri, ri, ri, ri,
              ri, ri, ri, ri, ri,
              ri, ri, ri, ri, ri,
              ri, ri, ri, ri, ri,
              ri, ri, ri, ri, ri,
              ri, ri, ri, ri};

G4double AbsorptionPlastic[nEntries2] =
          {400.0*cm,  400.0*cm, 400.0*cm, 400.0*cm, 400.0*cm, 400.0*cm,
           400.0*cm,  400.0*cm, 400.0*cm, 400.0*cm, 400.0*cm, 400.0*cm,
           400.0*cm,  400.0*cm, 400.0*cm, 400.0*cm, 400.0*cm, 400.0*cm,
           400.0*cm,  400.0*cm, 400.0*cm, 269.0*mm, 240.0*mm, 165.0*mm,
           130.0*mm, 100.0*mm, 65.0*mm, 35.0*mm, 20.0*mm, 5.0*mm,
           1.0*mm, 1.0*mm, 1.0*mm, 1.0*mm}; //Attenuation for PVT(Plastic), BC484 and //BC408, this is just ordinary optical absorption, 
//no wavelength shifting properties yet.

 
// UV -> Blue Wavelength shifting plastic properties
 
 G4double dye_concentration = 1.0;
 G4double d408 = (0.25/dye_concentration)*cm; //This is a scaling factor 
 //to adjust the absorption curve data to the dye concentration in 
 //our plastic, 0.25*cm seems correct for our testing drum data, for 
 //standard dye concentration
 
 G4double AbsBC408[nEntries2] = 
   { 100*m, 100*m, 100*m, 100*m, 100*m,
     100*m, 100*m, 100*m, 100*m, 100*m,
     100*m, 100*m, 100*m, 100*m, 100*m,
     100*m, 100*m, 100*m, 100*m, 100*m,
     10.0*d408, 3.0*d408, 2.5*d408, 2.0*d408, 2.0*d408,
     2.0*d408, 2.1*d408, 2.2*d408, 2.5*d408, 2.8*d408,
     5.0*d408, 10.0*d408, 100*m, 100*m}; //This is the UV to blue wavelength shifting plastic
 
 dye_concentration = 15.0;
 d408 = (0.25/dye_concentration)*cm;
 G4double AbsBC408_high[nEntries2] = 
   { 100*m, 100*m, 100*m, 100*m, 100*m,
     100*m, 100*m, 100*m, 100*m, 100*m,
     100*m, 100*m, 100*m, 100*m, 100*m,
     100*m, 100*m, 100*m, 100*m, 100*m,
     10.0*d408, 3.0*d408, 2.5*d408, 2.0*d408, 2.0*d408,
     2.0*d408, 2.1*d408, 2.2*d408, 2.5*d408, 2.8*d408,
     5.0*d408, 10.0*d408, 100*m, 100*m}; //This is the UV to blue wavelength shifting plastic


 G4double BC408Emission[nEntries_wlsp] =
            { 0, 0, 0, 0,
              0, 0.01, 0.02, 0.03,
              0.04, 0.05, 0.06, 0.07,
              0.08, 0.18, 0.30, 0.50,
              0.55, 0.80, 1.0, 1.0,
              0.90, 0.80, 0.44, 0.20,
              0.18, 0.10, 0.03, 0,
              0, 0, 0, 0 };


  G4MaterialPropertiesTable* myMPT3_wlsp = new G4MaterialPropertiesTable();
  myMPT3_wlsp->AddProperty("RINDEX", PhotonEnergy2, RefractiveIndexPlastic, nEntries2);
  myMPT3_wlsp->AddProperty("WLSABSLENGTH",PhotonEnergy2,AbsBC408,nEntries2);
  myMPT3_wlsp->AddProperty("WLSCOMPONENT",PhotonEnergy,BC408Emission,nEntries_wlsp);
  myMPT3_wlsp->AddConstProperty("WLSTIMECONSTANT", 2.1*ns);
  myMPT3_wlsp->AddProperty("ABSLENGTH", PhotonEnergy2, AbsorptionPlastic,     nEntries2);

  
  G4MaterialPropertiesTable* myMPT4_wlsp = new G4MaterialPropertiesTable();
  myMPT4_wlsp->AddProperty("RINDEX", PhotonEnergy2, RefractiveIndexPlastic, nEntries2);
  myMPT4_wlsp->AddProperty("WLSABSLENGTH",PhotonEnergy2,AbsBC408_high,nEntries2);
  myMPT4_wlsp->AddProperty("WLSCOMPONENT",PhotonEnergy,BC408Emission,nEntries_wlsp);
  myMPT4_wlsp->AddConstProperty("WLSTIMECONSTANT", 2.1*ns);
  myMPT4_wlsp->AddProperty("ABSLENGTH", PhotonEnergy2, AbsorptionPlastic,     nEntries2);
  

  BC408->SetMaterialPropertiesTable(myMPT3_wlsp);
  BC408_high->SetMaterialPropertiesTable(myMPT4_wlsp);



  G4Material* Tyvek_wlsp = new G4Material("Tyvek_wlsp", density=1.032*g/cm3, nelements=2);
  Tyvek_wlsp->AddElement(C, natoms=9);
  Tyvek_wlsp->AddElement(H, natoms=10);
}
