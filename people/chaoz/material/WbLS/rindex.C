void rindex() 
{
    SetStyle();
    const int NUMENTRIES_water=60;
	// wavelength = 1240 [ev.nm] / E
    // 200 - 790 nm, every 10 nm
    double GeV = 1e9;
    double ENERGY_water[NUMENTRIES_water] = { 
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
    double waterls = 1.3492/1.33427;
    double RINDEX1[NUMENTRIES_water] = {
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
    
    double x[NUMENTRIES_water];
    double y[NUMENTRIES_water];
    for (int i=0; i<NUMENTRIES_water; i++) {
        x[i] = 1240/ENERGY_water[NUMENTRIES_water-1-i]; // nm
        y[i] = RINDEX1[NUMENTRIES_water-1-i];
    }
    
    TCanvas *c1 = new TCanvas("c1","c1",800,600);
    TGraph *g1 = new TGraph(NUMENTRIES_water,x,y);
    g1->Draw("AL");
    g1->SetLineWidth(2);
    g1->GetXaxis()->SetTitle("Wavelength [nm]");
    g1->GetYaxis()->SetTitle("Index of Refraction");
    g1->SetTitle("");
    
}


void SetStyle()
{
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetPalette(1);
    gStyle->SetTitleStyle(0);
    gStyle->SetTitleBorderSize(0);
    gStyle->SetTitleOffset(1.1, "x");
    gStyle->SetTitleOffset(1.25, "y");
    gStyle->SetHistLineWidth(2);
    gStyle->SetLegendBorderSize(0);
}
