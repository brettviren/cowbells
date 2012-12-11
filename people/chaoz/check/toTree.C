const int MAXTUBES = 12000;
Int_t nHit;
Float_t digi_npeSum;
Float_t digi_time[MAXTUBES], digi_npe[MAXTUBES], digi_ctime[MAXTUBES], hit_theta[MAXTUBES], hit_disToTube[MAXTUBES]; 
Float_t tube_x[MAXTUBES], tube_y[MAXTUBES], tube_z[MAXTUBES];

// last track
Float_t vertex[3], direction[3];
Float_t totankdis;
Float_t momemtum;

void toTree(TString from="../gen/wcsim.root", TString saveTo="../gen/simTree.root")
{
    gROOT->ProcessLine(".x loadClasses.C");
    
    TFile *f = new TFile(saveTo, "RECREATE");
    TTree *T = new TTree("eventTree", "eventTree");
    T->SetDirectory(f);
    
    // Int_t neve;
    Double_t pos_x,pos_y,pos_z;
    Double_t dir_x,dir_y,dir_z;
    
    T->Branch("nHit", &nHit, "nHit/I");
    T->Branch("digi_npeSum", &digi_npeSum, "digi_npeSum/F");
    T->Branch("digi_time", &digi_time, "digi_time[nHit]/F");
    T->Branch("digi_ctime", &digi_ctime, "digi_ctime[nHit]/F");
    T->Branch("hit_theta", &hit_theta, "hit_theta[nHit]/F");
    T->Branch("hit_disToTube", &hit_disToTube, "hit_disToTube[nHit]/F");
    T->Branch("digi_npe", &digi_npe, "digi_npe[nHit]/F");
    T->Branch("tube_x", &tube_x, "tube_x[nHit]/F");
    T->Branch("tube_y", &tube_y, "tube_y[nHit]/F");
    T->Branch("tube_z", &tube_z, "tube_z[nHit]/F");

          
    T->Branch("P", &momemtum, "data/F");
    T->Branch("totankdis", &totankdis, "data/F");
    T->Branch("x", &vertex[0], "data/F");
    T->Branch("y", &vertex[1], "data/F");
    T->Branch("z", &vertex[2], "data/F");
    T->Branch("dx", &direction[0], "data/F");
    T->Branch("dy", &direction[1], "data/F");
    T->Branch("dz", &direction[2], "data/F");
    

    
    
    // WCSim Root Tree
    TFile *fromFile = new TFile(from);
    TTree *wcsimT = fromFile->Get("wcsimT");
    WCSimRootEvent *wcsimrootsuperevent = new WCSimRootEvent();
    wcsimT->SetBranchAddress("wcsimrootevent", &wcsimrootsuperevent);
    wcsimT->GetBranch("wcsimrootevent")->SetAutoDelete(kTRUE);
    
    // PMT Geometry Tree
    TTree *gtree = fromFile->Get("wcsimGeoT");
    WCSimRootGeom *wcsimrootgeom = new WCSimRootGeom();
    gbranch = gtree->GetBranch("wcsimrootgeom");
    gbranch->SetAddress(&wcsimrootgeom);
    gtree->GetEntry(0);
    WCSimRootPMT *pmt;
    Double_t pmt_pos[MAXTUBES][3];
    cout << "total PMTs: " << wcsimrootgeom->GetWCNumPMT() << endl;
    for (int i=0; i!=wcsimrootgeom->GetWCNumPMT(); i++){
        for (int j=0; j<3; j++) {
            pmt_pos[i][j] = (wcsimrootgeom->GetPMT(i)).GetPosition(j);
        }
    }
    
    for (int j=0; j!=wcsimT->GetEntries(); j++) {
        Reset();
        wcsimT->GetEvent(j);
        
        WCSimRootTrigger *wcsimrootevent = wcsimrootsuperevent->GetTrigger(0);
        
        // Track info
        int ntrack =  wcsimrootevent->GetNtrack();
        WCSimRootTrack* lastTrack = wcsimrootevent->GetTracks()->At(ntrack-1);
        momemtum = lastTrack->GetP();
        // cout << momemtum << endl;
        for (int i=0; i<3; i++) {
            vertex[i] = lastTrack->GetStart(i);
            direction[i] = lastTrack->GetDir(i);
            // cout << vertex[i] << ", " << direction[i] << endl;
        }
        totankdis = ToTankDistance(vertex, direction);
        TVector3 vertex3(vertex[0], vertex[1], vertex[2]);
        TVector3 direction3(direction[0], direction[1], direction[2]);
        
        // PrintTrigger(wcsimrootevent);
        
        cout << "=======" << endl; 
        cout << "Hit Tubes: " << wcsimrootevent->GetNumTubesHit() << endl;
        cout << "Digi Tubes: " << wcsimrootevent->GetNumDigiTubesHit() << endl;
        cout << "CK Hits: " << wcsimrootevent->GetNcherenkovhits() << endl;
        cout << "CK Hit Times: " << wcsimrootevent->GetNcherenkovhittimes() << endl;
        cout << "CK Digi Hits: " << wcsimrootevent->GetNcherenkovhits() << endl;
        cout << "=======\n" << endl;
                
        //Digi Hits
        TClonesArray* digiHits = wcsimrootevent->GetCherenkovDigiHits();
        nHit = wcsimrootevent->GetNcherenkovdigihits();
        // cout << nHit << endl;
        for (int i=0; i<nHit; i++){
            WCSimRootCherenkovDigiHit *cDigiHit = digiHits->At(i);
            
            digi_time[i] = cDigiHit->GetT();
            digi_npe[i] = cDigiHit->GetQ();
            digi_npeSum += digi_npe[i];
            
            int tubeIdx = cDigiHit->GetTubeId()-1;
            tube_x[i] = pmt_pos[tubeIdx][0];
            tube_y[i] = pmt_pos[tubeIdx][1];
            tube_z[i] = pmt_pos[tubeIdx][2];
            
            TVector3 hit3(tube_x[i], tube_y[i], tube_z[i]);
            TVector3 dis = hit3 - vertex3;             
            digi_ctime[i] = digi_time[i] - dis.Mag()/299792458.*1.333*1.e7;
            hit_theta[i] = dis.Angle(direction3)/TMath::Pi()*180.;
            hit_disToTube[i] = dis.Mag();

        }
        T->Fill();
        
        
    }
    
    f->Write();
    f->Close();
}

void Reset()
{
    digi_npeSum = 0;
    for (int i=0; i<MAXTUBES; i++) {
        digi_time[i] = 0;
        digi_npe[i] = 0;
        digi_ctime[i] = 0;
        hit_theta[i] = 0;
        hit_disToTube[i] = 0;
        tube_x[i] = 0;
        tube_y[i] = 0;
        tube_z[i] = 0;
        
    }
}

Float_t ToTankDistance(Float_t *abc1, Float_t *abc2)
{
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

    if (a != 0.0) {
        /* Find intersection with walls */
        c = (b*b - 4.0*a*c);
        if (c>0.0) {
            c = sqrt(c);
            d = (-b+c)/(2.0*a);
            if (d<0.0) d = (-b-c)/(2.0*a);
    
            pos[2] = vtx[2] + d*dir[2];
    
            /* Check that the z coordinate is inside the detector */
            if (pos[2]>det_z) { /* Intersects top */
                d = (det_z-vtx[2])/dir[2];
                pos[2] = det_z;
            }
            else {
                if (pos[2]<-det_z) { /* Intersects bottom */
                    d = (-det_z-vtx[2])/dir[2];
                    pos[2] = -det_z;
                }
            }
        }
        pos[0] = vtx[0] + d*dir[0];
        pos[1] = vtx[1] + d*dir[1];
    }
    else {
        /* Goes out through the top */
        pos[0] = vtx[0];
        pos[1] = vtx[1];
        if (dir[2]>0.0) {
            pos[2] = det_z;
        }
        else {
            pos[2] = -det_z;
        }
    }

    Double_t tmp;
    tmp = sqrt(pow(pos[2]-vtx[2],2)+pow(pos[1]-vtx[1],2)+pow(pos[0]-vtx[0],2));
    return tmp;
}


