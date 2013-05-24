#include "Cowbells/TrackingAction.h"
#include "Cowbells/TrackInformation.h"

#include "G4Track.hh"
#include "G4VProcess.hh"

#include <iostream>		// debugging

Cowbells::TrackingAction::TrackingAction()
{
}
Cowbells::TrackingAction::~TrackingAction()
{
}

void Cowbells::TrackingAction::PreUserTrackingAction (const G4Track* track)
{
}
void Cowbells::TrackingAction::PostUserTrackingAction(const G4Track* track)
{
    Cowbells::TrackInformation* info 
	= static_cast<Cowbells::TrackInformation*>(track->GetUserInformation());
    if (!info) {
	info = new Cowbells::TrackInformation();
    }

    int ptid = track->GetParentID();
    int ppdg = track->GetDefinition()->GetPDGEncoding(); 

    const G4VProcess* proc = track->GetCreatorProcess();
    int ptype=0, psubtype=0;
    G4String procname = "";
    if (proc) {
	ptype = proc->GetProcessType();
	psubtype = proc->GetProcessSubType();
	procname = proc->GetProcessName();
    }

    std::cout << "PUTA: " << ptid << ", " << ppdg << ", " 
	      << ptype << ", " << psubtype << " "
	      << procname
	      << std::endl;
    info->set(ptid, ppdg, ptype, psubtype);

    // http://geant4.web.cern.ch/geant4/UserDocumentation/UsersGuides/ForApplicationDeveloper/html/ch06s03.html
    // Within a concrete implementation of G4UserEventAction, the
    // SetUserEventInformation() method of G4EventManager may be used
    // to set a pointer of a concrete class object to G4Event, given
    // that the G4Event object is available only by "pointer to const"
    //
    // translation: we have permission to do the following
    G4Track* modifiable = const_cast<G4Track*>(track);
    modifiable->SetUserInformation(info);
}

