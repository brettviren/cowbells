void wls() 
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
    double cm = 0.01.;
    double wls_abs_factor = 0.7;
    double wls_abs[NUMENTRIES_water]={
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
    
    double x[NUMENTRIES_water];
    double y[NUMENTRIES_water];
    for (int i=0; i<NUMENTRIES_water; i++) {
        x[i] = 1240/ENERGY_water[NUMENTRIES_water-1-i]; // nm
        y[i] = 1/wls_abs[NUMENTRIES_water-1-i];
        // y[i] = RAYLEIGH_water[NUMENTRIES_water-1-i];
    }
    
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    TGraph *g1 = new TGraph(NUMENTRIES_water, x, y);
    g1->Draw("AL");
    // g1->GetYaxis()->SetRangeUser(0,0.01);
    g1->SetLineWidth(2);
    g1->GetXaxis()->SetTitle("Wavelength (nm)");
    g1->GetYaxis()->SetTitle("Arbitratry Unit");
    g1->SetTitle("PPO Absorption & Re-emission");
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

{
    // Double_t wls_abs_wl[12]={250,275,290,300,305,310,315,320,325,330,335,340};
    // Double_t wls_abs[12]={0.2,0.8,1.5,1.8,2.0,1.95,1.9,1.9,1.2,0.9,0.7,0.};
    // Double_t wls_emi_wl[16]={325,332.5,337.5,340,345,350,355,360,365,370,375,385,400,425,450,475};
    // Double_t wls_emi[16]={0,0.4,1.2,0.9,0.9,1.5,2.1,1.5,1.5,1.6,1.5,1.0,0.5,0.25,0.1,0.0};
}
