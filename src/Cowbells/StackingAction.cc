#include "Cowbells/StackingAction.h"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"

Cowbells::StackingAction::StackingAction()
    : G4UserStackingAction()
    , m_dr(0)
{
    G4cout << "Constructing Cowbells::StackingAction" << G4endl;
}

Cowbells::StackingAction::~StackingAction()
{
    G4cout << "Destructing Cowbells::StackingAction" << G4endl;
}

G4ClassificationOfNewTrack
Cowbells::StackingAction::ClassifyNewTrack(const G4Track * aTrack)
{
    m_dr->add_stack(aTrack);

    // if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()
    //    && aTrack->GetParentID()>0) {
    //     gammaCounter++;
    // }
    return fUrgent;
}


void Cowbells::StackingAction::NewStage()
{
//    G4cout << "Number of optical photons produced in this event : "
//           << gammaCounter << G4endl;
}


void Cowbells::StackingAction::PrepareNewEvent()
{
}



