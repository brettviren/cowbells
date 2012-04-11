void rayleigh() 
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
    double RAYFF = 1.;
    double cm = 0.01.;
    double RAYLEIGH_water[NUMENTRIES_water] = {
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
    
    double x[NUMENTRIES_water];
    double y[NUMENTRIES_water];
    for (int i=0; i<NUMENTRIES_water; i++) {
        x[i] = 1240/ENERGY_water[NUMENTRIES_water-1-i]; // nm
        y[i] = 1/RAYLEIGH_water[NUMENTRIES_water-1-i];
        // y[i] = RAYLEIGH_water[NUMENTRIES_water-1-i];
    }
    
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    TGraph *g1 = new TGraph(NUMENTRIES_water, x, y);
    g1->Draw("AL");
    g1->SetLineWidth(2);
    g1->GetXaxis()->SetTitle("Wavelength (nm)");
    g1->GetYaxis()->SetTitle("Rayleigh Scattering Coefficient [m^{-1}]");
    g1->SetTitle("");
    gPad->SetLogy();
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
