void plot_emm(){
  ifstream infile("emm.txt");
  Double_t x[471],y[471];
  for (Int_t i=0;i!=471;i++){
    infile >> x[i] >> y[i];
  }
  TGraph *g1 = new TGraph(471,x,y);
  g1->Draw("A*");
  
 //  for (Int_t i=0;i!=471;i++){
//     //cout << 1240/x[470-i] << "*eV,";
//     cout << y[470-i] << ",";
//   }
//   cout << endl;
  
  ifstream infile1("LAB.dat");
  Double_t x1[500],y1[500];
  for (Int_t i=0;i!=101;i++){
    infile1 >> x1[i] >> y1[i];
    y1[i]/=50;
  }
  TGraph *g2 = new TGraph(100,x1,y1);
  g2->Draw("*same");
  g2->SetMarkerColor(2);

  for (Int_t i=0;i!=101;i++){
    //cout << 1240/x[101-i] << "*eV,";
    cout << y[101-i] << ",";
  }
  cout << endl;
}
