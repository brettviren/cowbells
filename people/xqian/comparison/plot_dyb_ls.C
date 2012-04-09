void plot_dyb_ls(){
  ifstream infile1("dyb_ls_slow.dat");
  ifstream infile2("dyb_ls_fast.dat");
  Double_t x[2][300],y[2][300];
  for (Int_t i=0;i!=273;i++){
    infile1 >> x[0][i] >> y[0][i];
    infile2 >> x[1][i] >> y[1][i];
    
    x[0][i] = 1240./x[0][i];
    x[1][i] = 1240./x[1][i];
    
  }
  TGraph *g1 = new TGraph(273,x[0],y[0]);
  g1->Draw("A*");
  
  TGraph *g2 = new TGraph(273,x[1],y[1]);
  g1->Draw("*same");
  g1->SetMarkerColor(2);
}
