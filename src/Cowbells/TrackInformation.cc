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
    set_parent_tid(other->parent_tid());
    set_parent_pdg(other->parent_pdg());
    set_process_type(other->process_type());
    set_process_subtype(other->process_subtype());
}

Cowbells::TrackInformation::~TrackInformation()
{
}

