void PPO_emmission()
{
    SetStyle();
    ifstream infile("PPO_emmission.txt");
    Double_t x[200],y[200];
    for (Int_t i=0;i !=181;i++){
        infile >> x[i] >> y[i];
        //x[i] = 1240./ENERGY_water[i];
        //y[i] = wls_emi[i];
    }
    TCanvas *c1 = new TCanvas("c1","c1",800,600);
    c1->SetFillColor(10);
    TGraph *g1 = new TGraph(181,x,y);
    g1->Draw("AL");
    g1->SetLineWidth(2);
    g1->GetXaxis()->SetTitle("Wavelength (nm)");
    g1->GetYaxis()->SetTitle("Yield");
    g1->SetTitle("Light Emission Spectrum");


    for (Int_t i=0;i!=181;i++){
        // cout << 1240./x[180-i] << "*eV,";
        cout << y[180-i] << ",";
    }
    cout << endl;
  //g1->S
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
