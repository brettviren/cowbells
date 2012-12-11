{
    // properties of WbLS
    a = 1.01*g/mole;
    G4Element* elH = new G4Element("Hydrogen","H", 1,a);
    a = 16.00*g/mole;
    G4Element* elO = new G4Element("Oxygen","O", 8,a);
    a = 32.065*g/mole;
    G4Element *elS = new G4Element("Sulfur","S",16,a);
    a = 14.007*g/mole;
    G4Element *elN = new G4Element("Nitrogen","N",7,a);
    a = 12.01*g/mole;
    G4Element *elC = new G4Element("Carbon","C",6,a);
    
	density = 0.9945*g/cm3;
    G4Material* Water = new G4Material("Water",density,5);
    // ~10% Lab Based LS
    Water->AddElement(elH, 0.659);
    Water->AddElement(elO, 0.309);
    Water->AddElement(elS, 0.0009);
    Water->AddElement(elN, 0.000058);
    Water->AddElement(elC, 0.031);
	
	const G4int NUMENTRIES_water=60;
	// wavelength = 1240 [ev.nm] / E
    // 200 - 790 nm, every 10 nm
    G4double ENERGY_water[NUMENTRIES_water] = { 
        1.56962e-09*GeV, 1.58974e-09*GeV, 1.61039e-09*GeV, 1.63157e-09*GeV, 
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
        5.39129e-09*GeV, 5.63635e-09*GeV, 5.90475e-09*GeV, 6.19998e-09*GeV 
    };
	
    //Minfang's index refraction for WbLS
    G4double waterls = 1.3492/1.33427;
    G4double RINDEX1[NUMENTRIES_water] = {
        1.32885*waterls, 1.32906*waterls, 1.32927*waterls, 1.32948*waterls, 
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
        1.39074*waterls, 1.39956*waterls, 1.41075*waterls, 1.42535*waterls
    };
	
    //Minfang's absorption length for WbLS
    G4double ABSORPTION_water[NUMENTRIES_water] = {
        7.9741*cm*ABWFF, 9.02933*cm*ABWFF, 10.4065*cm*ABWFF, 12.2793*cm*ABWFF, 
        14.974*cm*ABWFF, 19.184*cm*ABWFF, 26.6874*cm*ABWFF, 40.128*cm*ABWFF, 
        59.7291*cm*ABWFF, 79.1575*cm*ABWFF, 95.7229*cm*ABWFF, 106.218*cm*ABWFF, 
        112.507*cm*ABWFF, 120.462*cm*ABWFF, 145.254*cm*ABWFF, 158.894*cm*ABWFF, 
        169.348*cm*ABWFF, 179.237*cm*ABWFF, 186.752*cm*ABWFF, 221.993*cm*ABWFF, 
        365.279*cm*ABWFF, 550.429*cm*ABWFF, 709.167*cm*ABWFF, 795.864*cm*ABWFF, 
        871.504*cm*ABWFF, 1037.87*cm*ABWFF, 1132.69*cm*ABWFF, 1201.08*cm*ABWFF, 
        1508.48*cm*ABWFF, 2390.09*cm*ABWFF, 3230.6*cm*ABWFF, 3795.21*cm*ABWFF, 
        4513.81*cm*ABWFF, 6037.13*cm*ABWFF, 6544.36*cm*ABWFF, 5550.56*cm*ABWFF, 
        5040.82*cm*ABWFF, 4574.07*cm*ABWFF, 4116.67*cm*ABWFF, 3714.29*cm*ABWFF, 
        3250*cm*ABWFF, 2806.82*cm*ABWFF, 2386.47*cm*ABWFF, 1937.25*cm*ABWFF, 
        1520*cm*ABWFF, 1064.66*cm*ABWFF, 500.507*cm*ABWFF, 370.037*cm*ABWFF, 
        225.571*cm*ABWFF, 137.989*cm*ABWFF, 106.58*cm*ABWFF, 22.1118*cm*ABWFF, 
        13.3941*cm*ABWFF, 12.6969*cm*ABWFF, 12.8165*cm*ABWFF, 13.1955*cm*ABWFF, 
        13.2277*cm*ABWFF, 12.6725*cm*ABWFF, 13.429*cm*ABWFF, 16.1433*cm*ABWFF,
    };
    
    //Minfang' scattering length for WbLS
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
    
    // PPO emmission spectra 350 - 500 nm, per nm	
    G4double ppo_ev[181]={2.48*eV,2.48497*eV,2.48996*eV,2.49497*eV,2.5*eV,2.50505*eV,2.51012*eV,2.51521*eV,2.52033*eV,2.52546*eV,2.53061*eV,2.53579*eV,2.54098*eV,2.5462*eV,2.55144*eV,2.5567*eV,2.56198*eV,2.56729*eV,2.57261*eV,2.57796*eV,2.58333*eV,2.58873*eV,2.59414*eV,2.59958*eV,2.60504*eV,2.61053*eV,2.61603*eV,2.62156*eV,2.62712*eV,2.6327*eV,2.6383*eV,2.64392*eV,2.64957*eV,2.65525*eV,2.66094*eV,2.66667*eV,2.67241*eV,2.67819*eV,2.68398*eV,2.6898*eV,2.69565*eV,2.70153*eV,2.70742*eV,2.71335*eV,2.7193*eV,2.72527*eV,2.73128*eV,2.73731*eV,2.74336*eV,2.74945*eV,2.75556*eV,2.76169*eV,2.76786*eV,2.77405*eV,2.78027*eV,2.78652*eV,2.79279*eV,2.7991*eV,2.80543*eV,2.81179*eV,2.81818*eV,2.8246*eV,2.83105*eV,2.83753*eV,2.84404*eV,2.85057*eV,2.85714*eV,2.86374*eV,2.87037*eV,2.87703*eV,2.88372*eV,2.89044*eV,2.8972*eV,2.90398*eV,2.9108*eV,2.91765*eV,2.92453*eV,2.93144*eV,2.93839*eV,2.94537*eV,2.95238*eV,2.95943*eV,2.96651*eV,2.97362*eV,2.98077*eV,2.98795*eV,2.99517*eV,3.00242*eV,3.00971*eV,3.01703*eV,3.02439*eV,3.03178*eV,3.03922*eV,3.04668*eV,3.05419*eV,3.06173*eV,3.06931*eV,3.07692*eV,3.08458*eV,3.09227*eV,3.1*eV,3.10777*eV,3.11558*eV,3.12343*eV,3.13131*eV,3.13924*eV,3.14721*eV,3.15522*eV,3.16327*eV,3.17136*eV,3.17949*eV,3.18766*eV,3.19588*eV,3.20413*eV,3.21244*eV,3.22078*eV,3.22917*eV,3.2376*eV,3.24607*eV,3.25459*eV,3.26316*eV,3.27177*eV,3.28042*eV,3.28912*eV,3.29787*eV,3.30667*eV,3.31551*eV,3.3244*eV,3.33333*eV,3.34232*eV,3.35135*eV,3.36043*eV,3.36957*eV,3.37875*eV,3.38798*eV,3.39726*eV,3.40659*eV,3.41598*eV,3.42541*eV,3.4349*eV,3.44444*eV,3.45404*eV,3.46369*eV,3.47339*eV,3.48315*eV,3.49296*eV,3.50282*eV,3.51275*eV,3.52273*eV,3.53276*eV,3.54286*eV,3.55301*eV,3.56322*eV,3.57349*eV,3.58382*eV,3.5942*eV,3.60465*eV,3.61516*eV,3.62573*eV,3.63636*eV,3.64706*eV,3.65782*eV,3.66864*eV,3.67953*eV,3.69048*eV,3.70149*eV,3.71257*eV,3.72372*eV,3.73494*eV,3.74622*eV,3.75758*eV,3.769*eV,3.78049*eV,3.79205*eV,3.80368*eV,3.81538*eV,3.82716*eV,3.83901*eV,3.85093*eV,3.86293*eV,3.875*eV};
    G4double ppo_emm[181]={2541.54,3124.19,3061.53,3037.8,3698.67,2682.4,3551.49,2759.86,3549.34,3434.85,3779.92,4100.42,3573.68,4359.18,4937.79,3916.84,4559.36,4359.95,4504.64,4744.8,5043.93,5654.4,4739.68,5577.59,4973.95,5114.24,5886.61,6572.44,7289.24,6713.17,7610.77,7296.81,7752.18,7209.37,8106.91,8652.75,8943.43,9692.26,10176.6,10746.3,10344.3,11747.2,12292.9,12706.5,13556,14827.9,13919.1,15198.1,16352.7,18168,17326.5,19102.6,19121.2,19297.9,20172.8,21279.9,22889.7,22660.3,24429.1,25578.1,26114.2,27410.1,28629.8,30848.9,31421.5,33385.8,34967.8,36638.8,39489,41119.8,41312.2,46175.8,47118.9,48920,51622.4,54791.6,58833.2,62620.6,64913.7,65227.5,69206.7,71265.4,74701.7,75612.7,78768.2,82629.5,86460.1,89618.8,93470.3,97671.7,100672,106714,112347,116281,126265,129929,136126,144722,153333,158643,163860,174231,178955,183509,191875,197852,202880,205720,206079,210369,213376,219420,223711,230360,240491,246300,265669,275737,289692,303614,318762,343541,355450,356684,365008,364466,371400,373686,365578,363652,364942,357897,345898,337882,328196,326912,332908,349242,364969,388441,409957,445645,470487,484744,490104,470286,449700,417960,389115,367229,342705,309227,277736,246470,219790,213914,205007,213101,218003,215775,199186,160937,117169,70198.4,41187.8,23908.2,15660,10989,7110.49,5920.07,3889.67,3030.48,2725.32,2430.16,2282.4,1926.76,2187.97,1826.96,1279.74,1489.21,1777.18};
    
    // PPO absorption spectra
    G4double wls_abs_factor = 0.7;
    G4double wls_abs[NUMENTRIES_water]={
        1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
        1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
        1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
        1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
        1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
        1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
        1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 
        1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 0.0226002*wls_abs_factor*cm, 0.01*wls_abs_factor*cm, 
        0.01*wls_abs_factor*cm, 0.01*wls_abs_factor*cm, 0.01*wls_abs_factor*cm, 0.0170268*wls_abs_factor*cm, 0.0356053*wls_abs_factor*cm, 0.0634218*wls_abs_factor*cm, 
        0.156279*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm, 1e+09*wls_abs_factor*cm
    };
    
    // material tables
    G4MaterialPropertiesTable *myMPT1 = new G4MaterialPropertiesTable();
    // ....
    myMPT1->AddConstProperty("SCINTILLATIONYIELD", 90./MeV);
    myMPT1->AddConstProperty("FASTTIMECONSTANT", 1.23*ns);
    myMPT1->AddConstProperty("SLOWTIMECONSTANT", 9.26*ns);
    myMPT1->AddConstProperty("YIELDRATIO", 0.26);
    myMPT1->AddProperty("FASTCOMPONENT", ppo_ev, ppo_emm, 181);
    myMPT1->AddProperty("SLOWCOMPONENT", ppo_ev, ppo_emm, 181);
    // PPO as WLS
    myMPT1->AddProperty("WLSABSLENGTH", ENERGY_water, wls_abs, NUMENTRIES_water);
    myMPT1->AddProperty("WLSCOMPONENT", ppo_ev, ppo_emm, 181);
    myMPT1->AddConstProperty("WLSTIMECONSTANT", 1.5*ns);
    
    Water->GetIonisation()->SetBirksConstant(0.124*mm/MeV);
    Water->SetMaterialPropertiesTable(myMPT1);
    
}