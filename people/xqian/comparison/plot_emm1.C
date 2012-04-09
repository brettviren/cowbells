void plot_emm1(){
  
  ifstream infile("PPO_emm.dat");
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
  g1->SetLineWidth(3.5);
  g1->GetXaxis()->SetTitle("Wavelength (nm)");
  g1->GetYaxis()->SetTitle("Yield");
  g1->SetTitle("PPO Emission Spectrum");

  
  for (Int_t i=0;i!=181;i++){
    // cout << 1240./x[180-i] << "*eV,";
    cout << y[180-i] << ",";
  }
  cout << endl;
  //g1->S
}
