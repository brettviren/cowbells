void plot_dyb_ree(){
  Double_t x[25],y[25];
  ifstream infile("dyb_reemission.dat");
  for (Int_t i=0;i!=25;i++){
    infile >> x[i] >> y[i];
    x[i] = 1240./x[i];
  }
  TGraph *g1 = new TGraph(25,x,y);
  g1->Draw("A*");
  
}
