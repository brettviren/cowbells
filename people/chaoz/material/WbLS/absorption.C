void absorption()
{
    SetStyle();
    ifstream infile("absorption.txt");
    Double_t x[60],y[60],temp;
    for (Int_t i = 0;i!=60;i++){
        infile >> x[i] >> temp >> y[i];
    }

    for (Int_t i = 0;i!=60;i++){
    //cout << 1240./x[59-i] << "*eV,";
    if (i % 4 == 0) { cout << endl; }
        cout << 100./y[59-i] << "*cm*ABWFF, ";
    }
    cout << endl;

    TCanvas *c1  = new TCanvas("c1","c1",800,600);
    c1->SetFillColor(10);
    c1->SetLogy(1);
    TGraph *g1   = new TGraph(60,x,y);
    g1->Draw("AL");
    g1->SetLineWidth(2);
    g1->GetYaxis()->SetTitle("Absorption Coefficient [m^{-1}]");
    g1->GetXaxis()->SetTitle("Wavelength [nm]");
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
