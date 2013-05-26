#include "Cowbells/TrackingAction.h"
#include "Cowbells/TrackInformation.h"

#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"


#include <iostream>		// debugging

Cowbells::TrackingAction::TrackingAction()
{
}
Cowbells::TrackingAction::~TrackingAction()
{
}

void Cowbells::TrackingAction::PreUserTrackingAction (const G4Track* /*track*/)
{
}
void Cowbells::TrackingAction::PostUserTrackingAction(const G4Track* track)
{
    Cowbells::TrackInformation* info 
	= static_cast<Cowbells::TrackInformation*>(track->GetUserInformation());
    if (!info) {
	info = new Cowbells::TrackInformation();
    }

    info->set_parent_tid(track->GetParentID());

    const G4VProcess* proc = track->GetCreatorProcess();
    G4String procname = "";
    if (proc) {
	info->set_process_type(proc->GetProcessType());
	info->set_process_subtype(proc->GetProcessSubType());
	procname = proc->GetProcessName();
    }

    if (false) {
	std::cout << "PUTA: " 
		  << "ptid=" << info->parent_tid() << ", " 
		  << "ppdg=" << info->parent_pdg() << ", " 
		  << "ptype=" << info->process_type() << ", "
		  << "psubtype=" << info->process_subtype() << ", "
		  << procname
		  << std::endl;
    }

    // http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch06s03.html
    // Within a concrete implementation of G4UserEventAction, the
    // SetUserEventInformation() method of G4EventManager may be used
    // to set a pointer of a concrete class object to G4Event, given
    // that the G4Event object is available only by "pointer to const"
    //
    // translation: we have permission to do the following
    G4Track* modifiable = const_cast<G4Track*>(track);
    modifiable->SetUserInformation(info);

    // pass on down to daughters
    G4TrackVector* daughters = fpTrackingManager->GimmeSecondaries();
    if (!daughters) {
	return;
    }

    size_t ndaughters = daughters->size();
    for (size_t ind=0; ind<ndaughters; ++ind) {
	Cowbells::TrackInformation* di = new Cowbells::TrackInformation(info);
	di->set_parent_pdg(track->GetDefinition()->GetPDGEncoding());
	(*daughters)[ind]->SetUserInformation(di);
	track->GetDefinition()->GetPDGEncoding(); 
    }


}

