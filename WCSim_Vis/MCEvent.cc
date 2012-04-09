#include "MCEvent.h"
#include <iostream>

ClassImp(MCEvent)



// *************************************************************
// Event Display for WCSIM
// Xin Qian mailto: xqian@caltech.edu
// Modefied from Krim Zbiri's updated version of root display
// http://nwg.phy.bnl.gov/DDRD/cgi-bin/private/ShowDocument?docid=462

// Modified from run_wcmap.cc from C. Walter of Duke
// http://nwg.phy.bnl.gov/DDRD/cgi-bin/private/ShowDocument?docid=245

// *************************************************************
// *************************************************************

// Main features: 
// 1. Add GUI
// 2. Give the status of current event
// 3. Can Select to only display one or multiple particles out
//    of all the particles.
// 4. Change the color scale (log vs linear)
// 5. Three mode display, timing, energy, and special display 
//    for pi0
// 6. With vertex/ring information or not
// 7. Can place a cut of NPE per pmt to clear plot
// 8. Can select set number for saving the plot
// 9  Can change index of refraction if needed (can used to 
//    project the ring)
// 10. Can select event no (other than next and prev)
// 11. Support both Mailbox and Cylinder shape. 
// 12. Can do both batch mode or interactive mode
//     see example in event_display.C
// **************************************************************


//_____________________________________________________________________________

  MCEvent::MCEvent(WCSimRootTrigger* evt, WCSimRootGeom* geom,Int_t* a, Int_t b)
{
  wcsimrootevent = evt;
  wcsimrootgeom = geom;
  hitid = -1;
  digihitid = -1;
  ntracks = b;
  
  TClonesArray* temp = (TClonesArray*)wcsimrootevent->GetTracks();  
  Int_t ntrack =  wcsimrootevent->GetNtrack();
  if (ntrack>=3){
    vertex_x = ((WCSimRootTrack*)temp->At(2))->GetStart(0);
    vertex_y = ((WCSimRootTrack*)temp->At(2))->GetStart(0);
    vertex_z = ((WCSimRootTrack*)temp->At(2))->GetStart(0);
  }else{
    vertex_x = 0.;
    vertex_y = 0.;
    vertex_z = 0.;
  }

  for (Int_t i=0;i<b;i++){
    tracks[i] = *(a+i);
  }

  for (Int_t i=0;i!=wcsimrootgeom->GetWCNumPMT();i++){
    pmt_pos[i][0] = (wcsimrootgeom->GetPMT(i)).GetPosition(0);
    pmt_pos[i][1] = (wcsimrootgeom->GetPMT(i)).GetPosition(1);
    pmt_pos[i][2] = (wcsimrootgeom->GetPMT(i)).GetPosition(2);
  }

  FindEnergyandTimeScales();
  SetPMTMap();
}



//_____________________________________________________________________________

void MCEvent::FindEnergyandTimeScales()
{
  // this function is used to individually scale each event image according to
  // its maximum energy and time values; for batch scaling simply modify this
  // function

  energy = 0;

  std::vector<Float_t> time_list;
  std::vector<Float_t> digitime_list;
  for (int i=0;i<wcsimrootevent->GetNcherenkovhits();i++)
  {
    //std::cerr << "loop1 "<< i <<std::endl;
    SetCherenkovHit(i);

    if (wcsimrootcherenkovhit->GetTotalPe(1)>energy)
      energy =(wcsimrootcherenkovhit->GetTotalPe(1));

    
    Float_t timetemp=0;
    //std::cerr << "max j: "<< wcsimrootcherenkovhit->GetTotalPe(1) <<std::endl;
    for (int j=0; j<wcsimrootcherenkovhit->GetTotalPe(1); j++)
    {
      //std::cerr << "loop2 "<< j <<std::endl;
      TObject *element2 = (wcsimrootevent->GetCherenkovHitTimes())->
	At(wcsimrootcherenkovhit->GetTotalPe(0)+j);
	//std::cerr << "loop "<< i <<std::endl;
      WCSimRootCherenkovHitTime *wcsimrootcherenkovhittime 
	= dynamic_cast<WCSimRootCherenkovHitTime*>(element2);
	//std::cerr << "hit time true time "<< wcsimrootcherenkovhittime->GetTruetime() <<std::endl;
      timetemp += (wcsimrootcherenkovhittime->GetTruetime() /
		   wcsimrootcherenkovhit->GetTotalPe(1));
    }
    time_list.push_back(timetemp);
  }

  for (int i=0;i<wcsimrootevent->GetNcherenkovdigihits();i++)
  {
    SetCherenkovDigiHit(i);

    digitime_list.push_back(wcsimrootcherenkovdigihit->GetT());
  }

  for (int a=0;a<2;a++)
  {
    time[a]     = 0;
    digitime[a] = 0;
  }
  /////  Time

  // first find the mean...
  for (int c=0;c<time_list.size();c++)
    time[0]+=time_list.at(c)/time_list.size();

  // ...then determine the standard deviation from the mean
  for (int cc=0;cc<time_list.size();cc++)
    time[1]+=(time_list.at(cc)-time[0])*(time_list.at(cc)-time[0]);
  time[1] = sqrt(time[1]/time_list.size());  
  
  /////  Digitime
  
  // first find the mean...
  for (int d=0;d<digitime_list.size();d++)
    digitime[0]+=digitime_list.at(d)/digitime_list.size();
  
  // ...then determine the standard deviation from the mean
  for (int dd=0;dd<digitime_list.size();dd++)
    digitime[1]+=(digitime_list.at(dd)-digitime[0])*
      (digitime_list.at(dd)-digitime[0]);
  digitime[1] = sqrt(digitime[1]/digitime_list.size());  
}

//_____________________________________________________________________________

void MCEvent::SetCherenkovHit(Int_t i)
{
  // in order to speed up the program and to eliminate redundant code,
  // this simple function sets pointers to the WCSimRootCherenkovHit instance
  // being analyzed and locates the PMT in WCSimRootGeom

  TObject *element = (wcsimrootevent->GetCherenkovHits())->At(i);

  wcsimrootcherenkovhit 
    = dynamic_cast<WCSimRootCherenkovHit*>(element);

//Printf("spot 1\n");
//Printf("index: %d\n", (wcsimrootcherenkovhit->GetTubeID()-1));
  SetPMT(wcsimrootgeom->GetPMT(wcsimrootcherenkovhit->GetTubeID()-1));
//Printf("spot 2\n");
  hitid = i;
}

//_____________________________________________________________________________

void MCEvent::SetCherenkovDigiHit(Int_t i)
{
  // same as in SetCherenkovHit, but for the list of digitized hits

  TObject *element = (wcsimrootevent->GetCherenkovDigiHits())->At(i);

  wcsimrootcherenkovdigihit 
    = dynamic_cast<WCSimRootCherenkovDigiHit*>(element);
//Printf("spot 1a\n");
//Printf("index: %d\n", (wcsimrootcherenkovdigihit->GetTubeID()-1));
  SetPMT(wcsimrootgeom->GetPMT(wcsimrootcherenkovdigihit->GetTubeId()-1));
//Printf("spot 2a\n");
  digihitid = i;

  SetCherenkovHit(PMTmap[wcsimrootcherenkovdigihit->GetTubeId()]);
}

//_____________________________________________________________________________

Float_t MCEvent::GetEnergyRatio(Int_t i)
{
  // returns the ratio of PMT energy to the maximum PMT energy

  if (hitid != i)
    SetCherenkovHit(i);

  return ((float)wcsimrootcherenkovhit->GetTotalPe(1)/energy);  
}




Float_t MCEvent::GetEnergy(Int_t i)
{
  // returns the ratio of PMT energy to the maximum PMT energy

  if (hitid != i)
    SetCherenkovHit(i);

  return ((float)wcsimrootcherenkovhit->GetTotalPe(1));  
}




//_____________________________________________________________________________

Float_t MCEvent::GetDigiEnergyRatio(Int_t i)
{
  // returns the ratio of PMT energy to the maximum PMT energy using the 
  // digitized PMT list

  if (digihitid != i)
    SetCherenkovDigiHit(i);

  return ((float)wcsimrootcherenkovhit->GetTotalPe(1)/energy);  
}

Float_t MCEvent::GetTime(Int_t i)
{
  // returns the earliest time of this PMT

  if (hitid != i)
    SetCherenkovHit(i);

  Float_t timetemp=0;

  for (int j=0;j<=0;j++)
    {
      TObject *element2 = (wcsimrootevent->GetCherenkovHitTimes())->
	At(wcsimrootcherenkovhit->GetTotalPe(0)+j);
      
      WCSimRootCherenkovHitTime *wcsimrootcherenkovhittime 
	= dynamic_cast<WCSimRootCherenkovHitTime*>(element2);
      timetemp += wcsimrootcherenkovhittime->GetTruetime();
      Float_t index = 1.333;
      Int_t qtubeid = wcsimrootcherenkovhit->GetTubeID();
      TVector3 vertex3(vertex_x,vertex_y,vertex_z);
      Double_t tube_posx,tube_posy,tube_posz;
      tube_posx = pmt_pos[qtubeid-1][0];
      tube_posy = pmt_pos[qtubeid-1][1];
      tube_posz = pmt_pos[qtubeid-1][2];
      TVector3 hit3(tube_posx,tube_posy,tube_posz);
      TVector3 dis = hit3-vertex3;
      timetemp = timetemp - dis.Mag()/299792458.*index*1.e7;
    }
  return timetemp;
}


//_____________________________________________________________________________

Float_t MCEvent::GetTimeRatio(Int_t i)
{
	
  // returns the ratio of average hit times in one PMT to the maximum PMT time
  
  if (hitid != i)
    SetCherenkovHit(i);

  Float_t timetemp=0;

  
  for (int j=0;j<wcsimrootcherenkovhit->GetTotalPe(1);j++)
  {
    TObject *element2 = (wcsimrootevent->GetCherenkovHitTimes())->
      At(wcsimrootcherenkovhit->GetTotalPe(0)+j);

    WCSimRootCherenkovHitTime *wcsimrootcherenkovhittime 
      = dynamic_cast<WCSimRootCherenkovHitTime*>(element2);

    timetemp += (wcsimrootcherenkovhittime->GetTruetime() /
		 wcsimrootcherenkovhit->GetTotalPe(1));
  }

  if (timetemp>(time[0]+time[1]))
    return 0;

  else if (timetemp<(time[0]-time[1]))
    return 1;

  else
    return (1 - (timetemp-time[0]+time[1])/(2*time[1]));  

}



//_____________________________________________________________________________

Float_t MCEvent::GetDigiTimeRatio(Int_t i)
{
  // returns the ratio of digitized PMT time to the maximum digitized PMT time

  if (digihitid != i)
    SetCherenkovDigiHit(i);

  Float_t timetemp=wcsimrootcherenkovdigihit->GetT();

  if (timetemp>(digitime[0]+digitime[1]))
    return 0;

  else if (timetemp<(digitime[0]-digitime[1]))
    return 1;

  return (1 - (timetemp-digitime[0]+digitime[1])/(2*digitime[1]));
}

//_____________________________________________________________________________

Float_t MCEvent::GetGammaRatio(Int_t i)
{
  // returns the ratio of the hits from one parent gamma in a PMT to the total
  // energy of the PMT (specifically for use with Pizero analysis)

  Double_t gamma[2]={0,0};
 
  if (hitid != i)
    SetCherenkovHit(i);
  
  //  cout << wcsimrootcherenkovhit->GetTotalPe(1) << endl;

  for (int i=0;i<wcsimrootcherenkovhit->GetTotalPe(1);i++)
    {
      TObject *element2 = (wcsimrootevent->GetCherenkovHitTimes())->
	At(wcsimrootcherenkovhit->GetTotalPe(0)+i);
      
      WCSimRootCherenkovHitTime *wcsimrootcherenkovhittime 
	= dynamic_cast<WCSimRootCherenkovHitTime*>(element2);
      
      for (int j=0;j<2;j++) {
	//	std::cerr << "gamma loop: "<< wcsimrootcherenkovhittime->GetParentID() << ", " << wcsimrootevent->GetPi0Info()->GetGammaID(j) << std::endl;
	if (wcsimrootcherenkovhittime->GetParentID()==wcsimrootevent->GetPi0Info()->GetGammaID(j))
	  gamma[j]++;
      }
    }
  //std::cerr << "gamma[] = " << gamma[0] <<"," << gamma[1] << std::endl;
  if(gamma[0] == 0 && gamma[1] == 0) return -1;
  return (gamma[0]/(gamma[0]+gamma[1]));
}



Float_t MCEvent::GetEnergyRatio_s(Int_t i)
{
  // returns the ratio of PMT energy to the maximum PMT energy

  if (hitid != i)
    SetCherenkovHit(i);
  
  float sum = 0;
  
  for (int i=0;i<wcsimrootcherenkovhit->GetTotalPe(1);i++){
    TObject *element2 = (wcsimrootevent->GetCherenkovHitTimes())->
      At(wcsimrootcherenkovhit->GetTotalPe(0)+i);
    
    WCSimRootCherenkovHitTime *wcsimrootcherenkovhittime 
      = dynamic_cast<WCSimRootCherenkovHitTime*>(element2);
        
    if (ntracks!=0){
      for (int j=0;j<ntracks;j++){
	if (wcsimrootcherenkovhittime->GetParentID()== tracks[j]){
	  // cout << wcsimrootcherenkovhittime->GetParentID() << "\t" << tracks[j] << endl;
	  sum++;
	  goto abc;
	}
      }
    }else{
      sum++;
    }
  abc:;
  }

  return ((float)sum/energy);  
}




//_____________________________________________________________________________

Float_t MCEvent::GetDigiGammaRatio(Int_t i)
{
  // same as SetGammaRatio, but for digitized hit list

  if (digihitid != i)
    SetCherenkovDigiHit(i);

  return GetGammaRatio(hitid);
}

//_____________________________________________________________________________

Float_t MCEvent::GetPosition(Int_t i, Int_t j)
{
  // returns the j-th Cartesian coordinate of the i-th PMT

  if (hitid != i)
    SetCherenkovHit(i);

  return pmt.GetPosition(j);
}

Int_t MCEvent::GetTubeID(Int_t i){
  return pmt.GetTubeNo();
}

//_____________________________________________________________________________

Float_t MCEvent::GetDigiPosition(Int_t i, Int_t j)
{
  // same as GetPosition, but for digitized hit list

  if (digihitid != i)
    SetCherenkovDigiHit(i);

  return pmt.GetPosition(j);
}

//_____________________________________________________________________________

void MCEvent::SetPMTMap()
{
  // fills in the PMTmap, which correlates PMT IDs to an index so they can be
  // easily referred to in a loop

  for (Int_t i=0;i<wcsimrootevent->GetNcherenkovhits();i++)
  {
    SetCherenkovHit(i);
    PMTmap[wcsimrootcherenkovhit->GetTubeID()]=i;
  }
}

//_____________________________________________________________________________

void MCEvent::SetPMT(WCSimRootPMT pmt_)
{
  // copies PMT geometry into local PMT class instance

  pmt.SetTubeNo(pmt_.GetTubeNo());
  pmt.SetCylLoc(pmt_.GetCylLoc());
  for (int i=0;i<3;i++)
  {
    pmt.SetOrientation(i,pmt_.GetOrientation(i));
    pmt.SetPosition(i,pmt_.GetPosition(i));
  }
}
//_____________________________________________________________________________
