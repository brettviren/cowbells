#ifndef COWBELLS_TRACKINGACTION_H
#define COWBELLS_TRACKINGACTION_H

#include <G4UserTrackingAction.hh>

namespace Cowbells {

    class TrackingAction : public G4UserTrackingAction {
    public:
	TrackingAction();
	virtual ~TrackingAction();

	virtual void PreUserTrackingAction (const G4Track* aTrack);
	virtual void PostUserTrackingAction(const G4Track*);

    private:

    };

}


#endif /* COWBELLS_TRACKINGACTION_H */
