// Based on Root test Event.cxx
////////////////////////////////////////////////////////////////////////

//#include "G4ios.hh"
#include "TObject.h"
#include "TDirectory.h"
#include "TProcessID.h"
#include "TClonesArray.h"

#include "WCSimRootGeom.hh"

#ifndef REFLEX_DICTIONARY
ClassImp(WCSimRootGeom)
ClassImp(WCSimRootPMT)
#endif

//______________________________________________________________________________
WCSimRootGeom::WCSimRootGeom()
{
  // Create a WCSimRootGeom object.
  fWCNumPMT = 0;
  fPMTArray = 0;
  fPMTArray = new TClonesArray("WCSimRootPMT", 500);

}

//______________________________________________________________________________
WCSimRootGeom::~WCSimRootGeom()
{
  fPMTArray->Delete();
  delete fPMTArray;
}

//______________________________________________________________________________
WCSimRootPMT::WCSimRootPMT()
{
  // Create a WCSimRootPMT object.
}

//______________________________________________________________________________
WCSimRootPMT::WCSimRootPMT(Int_t tubeNo, Int_t cylLoc, Float_t orientation[3], Float_t position[3])
{
	fTubeNo = tubeNo;
	fCylLoc = cylLoc;
	int j = 0;
	for(j = 0; j < 3; j++) {
		fOrientation[j] = orientation[j];
		fPosition[j] = position[j];
	}
	// fOrientation = *(orientation);
	// fPositoin = *(position);
  // Create a WCSimRootPMT object.
}

//______________________________________________________________________________
void WCSimRootGeom::SetPMT(Int_t i, Int_t tubeno, Int_t cyl_loc, 
			    Float_t rot[3], Float_t pos[3], bool expand)
{
   if(expand) (*(fPMTArray)).ExpandCreate(i+2);

  // Set PMT values
TClonesArray &pmtArray = *fPMTArray;
    WCSimRootPMT *jPMT = new(pmtArray[i]) WCSimRootPMT(tubeno, cyl_loc, rot, pos);
    //WCSimRootPMT jPMT = *(WCSimRootPMT*)(*fPMTArray)[i];
    // jPMT.SetTubeNo(tubeno);
    // jPMT.SetCylLoc(cyl_loc);
    // int j;
    // for (j=0;j<3;j++){
    //   jPMT.SetOrientation(j,rot[j]);
    //   jPMT.SetPosition(j,pos[j]);
    // }

}

//______________________________________________________________________________
WCSimRootPMT::~WCSimRootPMT()
{
}
