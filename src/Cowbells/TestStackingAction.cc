#include "Cowbells/TestStackingAction.h"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Cowbells::TestStackingAction::TestStackingAction()
  : gammaCounter(0)
{
    G4cout << "Constructing Cowbells::TestStackingAction" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Cowbells::TestStackingAction::~TestStackingAction()
{
    G4cout << "Destructing Cowbells::TestStackingAction" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
Cowbells::TestStackingAction::ClassifyNewTrack(const G4Track * aTrack)
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

void Cowbells::TestStackingAction::NewStage()
{
    G4cout << "Number of optical photons produced in this event : "
           << gammaCounter << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Cowbells::TestStackingAction::PrepareNewEvent()
{ gammaCounter = 0; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
