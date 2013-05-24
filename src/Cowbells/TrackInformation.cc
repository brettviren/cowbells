#include "Cowbells/TrackInformation.h"

#include "G4ios.hh"

G4Allocator<Cowbells::TrackInformation> Cowbells::gsTrackInformationAllocator;

Cowbells::TrackInformation::TrackInformation()
{
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
    set(other->parent_tid(),
	other->parent_pdg(),
	other->process_type(),
	other->process_subtype());
}

Cowbells::TrackInformation::~TrackInformation()
{
}

