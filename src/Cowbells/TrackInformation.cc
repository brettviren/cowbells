#include "Cowbells/TrackInformation.h"

#include "G4ios.hh"

G4Allocator<Cowbells::TrackInformation> Cowbells::gsTrackInformationAllocator;

#include <iostream>
using namespace std;
static int nti = 0;

Cowbells::TrackInformation::TrackInformation()
{
    nti += 1;
    //cerr << "Constructing Trackinformation #" << nti << endl;
    set();
}

void Cowbells::TrackInformation::set(int parent_tid, int parent_pdg, 
				     int process_type, int process_subtype)
{
    parentTrackID = parent_tid;
    parentPDGID = parent_pdg;
    processType = process_type;
    processSubType = process_subtype;
}

Cowbells::TrackInformation::TrackInformation(const Cowbells::TrackInformation* other)
{
    nti += 1;
    //cerr << "Copy constructing Trackinformation #" << nti << endl;
    set_parent_tid(other->parent_tid());
    set_parent_pdg(other->parent_pdg());
    set_process_type(other->process_type());
    set_process_subtype(other->process_subtype());
}

Cowbells::TrackInformation::~TrackInformation()
{
    nti -= 1;
    if (false) {
	cerr << "Destructing Trackinformation with " << nti << " left: " 
	     << " ptid=" << parentTrackID
	     << " ppdg=" << parentPDGID
	     << " ptyp=" << processType
	     << " styp=" << processSubType
	     << endl;
    }
}

