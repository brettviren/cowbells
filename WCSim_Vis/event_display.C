void event_display(TString filename="wcsim.root"){
    
	gSystem->Load("libWCMap.so");
	// gSystem->Load("libWCMap.so");
  // filename = "temp.root";
  WCSim_Draw *test = new WCSim_Draw(filename,gClient->GetRoot(), 800, 600);
    
 //  //batch mode
//   for (Int_t i=0;i!=10;i++){
//     test->SetEventNo(i);
//     test->DoDraw();
//     test->DoSave();
//   }
//   test->DoExit();
}
