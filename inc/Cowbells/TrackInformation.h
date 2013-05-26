#ifndef COWBELLS_TRACKINFORMATION_H
#define COWBELLS_TRACKINFORMATION_H

#include "G4VUserTrackInformation.hh"
#include "G4Allocator.hh"

namespace Cowbells {

    class TrackInformation : public G4VUserTrackInformation {
	int parentTrackID;
	int parentPDGID;
	int processType, processSubType;
    public:
	TrackInformation();
	TrackInformation(const TrackInformation* other);
	virtual ~TrackInformation();
	
	void set(int parent_tid=-1, int parent_pdg=0, 
		 int process_type=0, int process_subtype=0);
	int parent_tid() const { return parentTrackID; }
	int parent_pdg() const { return parentPDGID; }
	int process_type() const { return processType; }
	int process_subtype() const { return processSubType; }

	void set_parent_tid(int tid) { parentTrackID = tid; }
	void set_parent_pdg(int pdg) { parentPDGID = pdg; }
	void set_process_type(int t) { processType = t; }
	void set_process_subtype(int st) { processSubType = st; }

	inline void *operator new(size_t);
	inline void operator delete(void *info);

	int operator ==(const Cowbells::TrackInformation& right) const {
	    return (this==&right);
	}
    };

    extern G4Allocator<Cowbells::TrackInformation> gsTrackInformationAllocator;
}

inline void* Cowbells::TrackInformation::operator new(size_t) 
{
    return Cowbells::gsTrackInformationAllocator.MallocSingle();
}
inline void Cowbells::TrackInformation::operator delete(void *info) 
{
    Cowbells::gsTrackInformationAllocator.FreeSingle((Cowbells::TrackInformation*)info);
}


#endif /* COWBELLS_TRACKINFORMATION_H */
