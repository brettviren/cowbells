void plot_dyb_abs(){
  Double_t x[1500],y[1500];
  ifstream infile("dyb_absorp.dat");
  for (Int_t i=0;i!=1421;i++){
    infile >> x[i] >> y[i];
    x[i] = 1240./x[i];
    y[i] = 1./y[i];
  }
  TGraph *g1 = new TGraph(1421,x,y);
  g1->Draw("A*");
  
}
