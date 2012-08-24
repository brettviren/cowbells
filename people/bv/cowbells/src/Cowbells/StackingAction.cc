#include "Cowbells/StackingAction.h"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Cowbells::StackingAction::StackingAction()
  : gammaCounter(0)
{
    G4cout << "Constructing Cowbells::StackingAction" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Cowbells::StackingAction::~StackingAction()
{
    G4cout << "Destructing Cowbells::StackingAction" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
Cowbells::StackingAction::ClassifyNewTrack(const G4Track * aTrack)
{
    if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
        { // particle is optical photon
            if(aTrack->GetParentID()>0)
                { // particle is secondary
                    gammaCounter++;
                }
        }
    return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Cowbells::StackingAction::NewStage()
{
    G4cout << "Number of optical photons produced in this event : "
           << gammaCounter << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Cowbells::StackingAction::PrepareNewEvent()
{ gammaCounter = 0; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
