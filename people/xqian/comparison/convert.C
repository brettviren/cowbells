void convert(){
  Double_t wl[60],abs_l[60],scat_l[60];
  ifstream infile("new_abs_minfang.txt");
  for (Int_t i=0;i!=60;i++){
    infile >> wl[i] >> scat_l[i] >> abs_l[i] ;
  }

  for (Int_t i=0;i!=60;i++){
    if (i%5==0) cout << endl;
    cout << 100./scat_l[59-i] << "*cm*RAYFF, ";
    
  }

  cout << endl << endl;

  
}
