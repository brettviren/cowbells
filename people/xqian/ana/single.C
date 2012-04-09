void compare_wls1(TString filename="../p15m_nwls/wcsim.root",TString histoname="p15m_nwls", Int_t *flag, TString rootfile = "temp.root"){
  
  TFile *file1;
  if (*flag!=0){
    //file1 = new TFile(rootfile,"Update");
    file1 = new TFile(rootfile,"RECREATE");
  }else{
    file1 = new TFile(rootfile,"RECREATE");
  }
  TString filename1;
  filename1 = histoname + "_digi";
  TTree *T = new TTree(filename1,filename1);
  T->SetDirectory(file1);
  Double_t diginpe,digitime,cor_digitime,digitheta,dis_digihit;
  Int_t neve;

  Double_t mom;
  Double_t pos_x,pos_y,pos_z;
  Double_t dir_x,dir_y,dir_z;
  Double_t tube_x,tube_y,tube_z;
  Double_t totankdis;
  Double_t vertex[3],dir[3];

  Double_t tube_pos[3];
  
  T->Branch("digi_eve",&neve,"data/I");
  T->Branch("diginpe",&diginpe,"data/D");
  T->Branch("digitime",&digitime,"data/D");
  T->Branch("cor_digitime",&cor_digitime,"data/D");
  T->Branch("digitheta",&digitheta,"data/D");
  T->Branch("dis_dighit",&dis_digihit,"data/D");  

  T->Branch("mom",&mom,"data/D");
  T->Branch("totankdis",&totankdis,"data/D");
  T->Branch("pos_x",&vertex[0],"data/D");
  T->Branch("pos_y",&vertex[1],"data/D");
  T->Branch("pos_z",&vertex[2],"data/D");

  T->Branch("dir_x",&dir[0],"data/D");
  T->Branch("dir_y",&dir[1],"data/D");
  T->Branch("dir_z",&dir[2],"data/D");

  T->Branch("tube_x",&tube_pos[0],"data/D");
  T->Branch("tube_y",&tube_pos[1],"data/D");
  T->Branch("tube_z",&tube_pos[2],"data/D");

  filename1 = histoname + "_hit";
  TTree *t1 = new TTree(filename1,filename1);
  t1->SetDirectory(file1);
  
  Double_t wavelength, truetime, corr_time,theta,distance,index;
  Int_t qe_flag,parentid,tubeid,totalpe;
  
  Int_t ntracks;

  t1->Branch("ntracks",&ntracks,"data/I");
  t1->Branch("neve",&neve,"data/I");
  t1->Branch("wavelength",&wavelength,"data/D");
  t1->Branch("truetime",&truetime,"data/D");
  t1->Branch("corr_time",&corr_time,"data/D");
  t1->Branch("theta",&theta,"data/D");
  t1->Branch("distance",&distance,"data/D");
  t1->Branch("index",&index,"data/D");
  
  t1->Branch("mom",&mom,"data/D");
  t1->Branch("totankdis",&totankdis,"data/D");
  t1->Branch("pos_x",&vertex[0],"data/D");
  t1->Branch("pos_y",&vertex[1],"data/D");
  t1->Branch("pos_z",&vertex[2],"data/D");

  t1->Branch("dir_x",&dir[0],"data/D");
  t1->Branch("dir_y",&dir[1],"data/D");
  t1->Branch("dir_z",&dir[2],"data/D");

  t1->Branch("tube_x",&tube_pos[0],"data/D");
  t1->Branch("tube_y",&tube_pos[1],"data/D");
  t1->Branch("tube_z",&tube_pos[2],"data/D");
  
  // t1->Branch("pos_x",&pos_x,"data/D");
//   t1->Branch("pos_y",&pos_y,"data/D");
//   t1->Branch("pos_z",&pos_z,"data/D");

//   t1->Branch("dir_x",&dir_x,"data/D");
//   t1->Branch("dir_y",&dir_y,"data/D");
//   t1->Branch("dir_z",&dir_z,"data/D");
  
//   t1->Branch("tube_x",&tube_x,"data/D");
//   t1->Branch("tube_y",&tube_y,"data/D");
//   t1->Branch("tube_z",&tube_z,"data/D");
  
  t1->Branch("qe_flag",&qe_flag,"data/I");
  t1->Branch("parentid",&parentid,"data/I");
  t1->Branch("tubeid",&tubeid,"data/I");
  t1->Branch("totalpe",&totalpe,"data/I");


  TFile *file = new TFile(filename);
  TTree  *wcsimT = file->Get("wcsimT");
  WCSimRootEvent *wcsimrootsuperevent = new WCSimRootEvent();
  wcsimT->SetBranchAddress("wcsimrootevent",&wcsimrootsuperevent);
  wcsimT->GetBranch("wcsimrootevent")->SetAutoDelete(kTRUE);
  
  

  TTree *gtree = file->Get("wcsimGeoT");
  WCSimRootGeom *wcsimrootgeom = new WCSimRootGeom();
  gbranch = gtree->GetBranch("wcsimrootgeom");
  gbranch->SetAddress(&wcsimrootgeom);
  gtree->GetEntry(0);

  WCSimRootPMT *pmt;

  Double_t pmt_pos[500000][3];

  for (Int_t i=0;i!=wcsimrootgeom->GetWCNumPMT();i++){
    pmt_pos[i][0] = (wcsimrootgeom->GetPMT(i)).GetPosition(0);
    pmt_pos[i][1] = (wcsimrootgeom->GetPMT(i)).GetPosition(1);
    pmt_pos[i][2] = (wcsimrootgeom->GetPMT(i)).GetPosition(2);
  }
  
  //in terms of wavelength (total NPE) real hit
  filename1 = histoname + "_total_wl";
  TH1F *hqx = new TH1F(filename1,filename1,600,200,800);
  
  //NPE in each event sum over digi hit
  filename1 = histoname + "_total_npe";
  TH1F *hqx2 = new TH1F(filename1,filename1,1000,0.,10000);
  
  //digitized hit time
  filename1 = histoname + "_digitime";
  TH1F *hqx1 = new TH1F(filename1,filename1,500,900,1400);
  
  //corrected digitized hit time
  filename1 = histoname + "_cor_digitime";
  TH1F *hqx4 = new TH1F(filename1,filename1,1000,400,1400);
  
  //digitized hit angle
  filename1 = histoname + "_digitheta";
  TH1F *hqx5 = new TH1F(filename1,filename1,180,0,180);
  
  

  //TH2F *h1 = new TH2F("h1","h1",100,1000,20000,100,90000,140000);
  
  Double_t index = 1.333;

  

  neve = *flag;

  cout << histoname << "\t" << wcsimT->GetEntries() << endl;
  for (Int_t j=0;j!=wcsimT->GetEntries();j++){
    //for (Int_t j=0;j!=90;j++){
    // cout << j << endl;
    wcsimT->GetEvent(j);
    neve ++;
    
    WCSimRootTrigger *wcsimrootevent = wcsimrootsuperevent->GetTrigger(0);
    temp = (TClonesArray*)wcsimrootevent->GetTracks();  
    
    Int_t ntrack =  wcsimrootevent->GetNtrack();
    //cout << ntrack << endl;
    ntracks = ntrack;
    
    mom = ((WCSimRootTrack*)temp->At(ntrack-1))->GetP();
    //get the vertex information
    vertex[0] = ((WCSimRootTrack*)temp->At(ntrack-1))->GetStart(0);
    vertex[1] = ((WCSimRootTrack*)temp->At(ntrack-1))->GetStart(1);
    vertex[2] = ((WCSimRootTrack*)temp->At(ntrack-1))->GetStart(2);
    
    //get position information
    dir[0] = ((WCSimRootTrack*)temp->At(ntrack-1))->GetDir(0);
    dir[1] = ((WCSimRootTrack*)temp->At(ntrack-1))->GetDir(1);
    dir[2] = ((WCSimRootTrack*)temp->At(ntrack-1))->GetDir(2);
    
    totankdis=ToTankDistance(vertex,dir);
    
    TVector3 vertex3(vertex[0],vertex[1],vertex[2]);
    TVector3 dir3(dir[0],dir[1],dir[2]);
    

     //loop through digi hit
    int max = wcsimrootevent->GetNcherenkovdigihits();
    
    double sum = 0;
    
    for (int i=0;i<max;i++){
      // cout << max << "\t" << i << endl;
      WCSimRootCherenkovDigiHit *cDigiHit = ((WCSimRootCherenkovDigiHit*)wcsimrootevent->GetCherenkovDigiHits()->At(i));
      hqx1->Fill(cDigiHit->GetT());
      tube_pos[0] =  pmt_pos[(cDigiHit->GetTubeId()-1)][0];
      tube_pos[1] =  pmt_pos[(cDigiHit->GetTubeId()-1)][1];
      tube_pos[2] =  pmt_pos[(cDigiHit->GetTubeId()-1)][2];
       
      TVector3 hit3(tube_pos[0],tube_pos[1],tube_pos[2]);
      TVector3 dis = hit3-vertex3;

      diginpe = cDigiHit->GetQ();
      digitime = cDigiHit->GetT();
      cor_digitime = digitime-dis.Mag()/299792458.*1.333*1.e7;
      digitheta = dis.Angle(dir3)/3.1415926*180.;
      dis_digihit = dis.Mag();


      hqx4->Fill(cor_digitime,diginpe);
      hqx5->Fill(digitheta,diginpe);
      sum += diginpe;
            
      T->Fill();
    }
    hqx2->Fill(sum);
    
  
    //loop through real hit
    
  
    
    //loop through PMT hit first
    max = wcsimrootevent-> GetNcherenkovhits();
    //cout << max << endl;
    if (max ==0){
      t1->Fill();
    }
    for (int i=0;i<max;i++){
      WCSimRootCherenkovHit* wcsimrootcherenkovhit = 
	dynamic_cast<WCSimRootCherenkovHit*>((wcsimrootevent->GetCherenkovHits())->At(i));
      
      totalpe = wcsimrootcherenkovhit->GetTotalPe(1);
      tubeid = wcsimrootcherenkovhit->GetTubeID() ;

      //loop through hit time etc
      for (int k=0;k<totalpe;k++){
	TObject *element2 = (wcsimrootevent->GetCherenkovHitTimes())->
	  At(wcsimrootcherenkovhit->GetTotalPe(0)+k);
	WCSimRootCherenkovHitTime *wcsimrootcherenkovhittime 
	  = dynamic_cast<WCSimRootCherenkovHitTime*>(element2);
		
       	wavelength =wcsimrootcherenkovhittime->GetWavelength();
	qe_flag = wcsimrootcherenkovhittime->GetQe_flag();
	truetime = wcsimrootcherenkovhittime->GetTruetime();
	parentid = wcsimrootcherenkovhittime->GetParentID();
	
	pos_x = wcsimrootcherenkovhittime->GetPosX() ;
	pos_y = wcsimrootcherenkovhittime->GetPosY() ;
	pos_z = wcsimrootcherenkovhittime->GetPosZ() ;
	dir_x = wcsimrootcherenkovhittime->GetDirX() ;
	dir_y = wcsimrootcherenkovhittime->GetDirY() ;
	dir_z = wcsimrootcherenkovhittime->GetDirZ() ;

	tube_pos[0] =  pmt_pos[tubeid-1][0];
	tube_pos[1] =  pmt_pos[tubeid-1][1];
	tube_pos[2] =  pmt_pos[tubeid-1][2];

	tube_x = tube_pos[0];
	tube_y = tube_pos[1];
	tube_z = tube_pos[2];

	

	TVector3 hit3(tube_pos[0],tube_pos[1],tube_pos[2]);
	TVector3 dis = hit3-vertex3;
	
	distance = dis.Mag();
	theta = dis.Angle(dir3)/3.1415926*180.;
	//index = index(wavelength);
	index = 1.34;
	corr_time = truetime - distance/299792458.*1e7*index;

	if (qe_flag==1){
	  hqx->Fill(wavelength);
	}

	t1->Fill();
      } 
    }



  }
  
  if (flag==1){
    hqx->SetDirectory(file1);
    hqx2->SetDirectory(file1);
    hqx1->SetDirectory(file1);
    hqx4->SetDirectory(file1);
    hqx5->SetDirectory(file1);
    file1->Write();
    file1->Close();
  }else{
    hqx->SetDirectory(file1);
    hqx2->SetDirectory(file1);
    hqx1->SetDirectory(file1);
    hqx4->SetDirectory(file1);
    hqx5->SetDirectory(file1);
    file1->Write();
    file1->Close();
  }

  *flag = neve;
  
}

Double_t index(Double_t x){ // x is wave length
  return 1.31279+15.762/x-4382./x/x+1.1455e6/x/x/x;
}


 Double_t ToTankDistance(Double_t *abc1, Double_t *abc2){
    Double_t vtx[3],dir[3];
    vtx[0]=*(abc1);
    vtx[1]=*(abc1+1);
    vtx[2]=*(abc1+2);

    dir[0]= *(abc2);
    dir[1]= *(abc2+1);
    dir[2]= *(abc2+2);
    
    

    // projects a vector to the tank surface (taken from SuperScan)
    Double_t a,b,c,d=0;
    // Double_t det_r = 53/2.*100.;
//     Double_t det_z = 60./2.*100.;

    //SK geometry
    Double_t det_r = 62.21/2.*100.;
    Double_t det_z = 79.96/2.*100.;
    
    Double_t pos[3];
    /* Find intercept of this vector with walls of detector */
    a = (dir[0]*dir[0]) + (dir[1]*dir[1]);
    b = 2.0 * ( dir[0] * vtx[0] + dir[1] * vtx[1] );
    c = (vtx[0]*vtx[0]) + (vtx[1]*vtx[1]) - (det_r*det_r);
    
    if (a != 0.0)
      {
	/* Find intersection with walls */
	
	c = (b*b - 4.0*a*c);
	if (c>0.0)
	  {
	    c = sqrt(c);
	    d = (-b+c)/(2.0*a);
	    if (d<0.0) d = (-b-c)/(2.0*a);
	    
	    pos[2] = vtx[2] + d*dir[2];
	    
	    /* Check that the z coordinate is inside the detector */
	    
	    if (pos[2]>det_z) /* Intersects top */
	      {
		d = (det_z-vtx[2])/dir[2];
		pos[2] = det_z;
	      }
	    else
	      if (pos[2]<-det_z) /* Intersects bottom */
		{
		  d = (-det_z-vtx[2])/dir[2];
		  pos[2] = -det_z;
		}
	  }
	pos[0] = vtx[0] + d*dir[0];
	pos[1] = vtx[1] + d*dir[1];
      }
    else
      {
	/* Goes out through the top */
	
	pos[0] = vtx[0];
	pos[1] = vtx[1];
	if (dir[2]>0.0)
	  {
	    pos[2] = det_z;
	  }
	else
	  {
	    pos[2] = -det_z;
	  }
      }
    
    Double_t tmp;
    tmp = sqrt(pow(pos[2]-vtx[2],2)+pow(pos[1]-vtx[1],2)+pow(pos[0]-vtx[0],2));
    return tmp;
  }
 
