#include "single.C"
void compare_ele_noLC(Int_t nrun = 1){
  gROOT->Reset();
  gSystem->Load("./libWCSimRoot.so");
  Int_t neve = 0;
  TString filename,filename1;

  filename.Form("wcsim.root",nrun);
  filename1.Form("./electron_noLC1_%d.root",nrun);
  compare_wls1(filename,"noLC",&neve,filename1);

  
}

