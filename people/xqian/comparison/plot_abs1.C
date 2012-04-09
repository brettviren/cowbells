void plot_abs1(){
  ifstream infile("Wa_abs.dat");
  Double_t x[60],y[60],temp;
  for (Int_t i=0;i!=60;i++){
    infile >> x[i] >> temp >> y[i] >> temp;
  }
  
  for (Int_t i=0;i!=60;i++){
    //cout << 1240./x[59-i] << "*eV,";
    cout << 100./y[59-i] << "*cm*ABWFF,";
  }
  cout << endl;

  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  c1->SetFillColor(10);
  c1->SetLogy(1);
  TGraph *g1 = new TGraph(60,x,y);
  g1->Draw("AL");
  g1->SetLineWidth(3.5);
  g1->GetYaxis()->SetTitle("Absorption Coefficient (1/#lambda 1/m)");
  g1->GetXaxis()->SetTitle("Wavelength (nm)");
  g1->SetTitle("Absorption of Water-LS");
}
