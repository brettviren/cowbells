#include "Cowbells/SteppingAction.h"

#include <G4Step.hh>
#include <G4StepPoint.hh>
#include <G4ParticleDefinition.hh>

#include <string>
using namespace std;

Cowbells::SteppingAction::SteppingAction()
{
}

Cowbells::SteppingAction::~SteppingAction()
{
}
        
void Cowbells::SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    G4StepPoint* prepoint = step->GetPreStepPoint();
    G4StepPoint* pstpoint= step->GetPostStepPoint();

    if (prepoint->GetPhysicalVolume() == pstpoint->GetPhysicalVolume()) {
        return;
    }

    G4int trackId = track ->GetTrackID();
    G4ParticleDefinition* particle = track->GetDefinition();

    G4VPhysicalVolume* prephy = prepoint->GetPhysicalVolume();
    G4VPhysicalVolume* pstphy = pstpoint->GetPhysicalVolume();
    
    string prename = "NONE";
    if (prephy) prename = prephy->GetName();
    string pstname = "NONE";
    if (pstphy) pstname = pstphy->GetName();


    G4cout << "Step: from " 
           << prename
           << " to " 
           << pstname
           << " with " 
           << "#" << trackId << ": "
           << particle->GetParticleName()
           << G4endl;
}

