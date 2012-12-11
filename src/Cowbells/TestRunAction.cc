// Make this appear first!
#include "G4Timer.hh"

#include "Cowbells/TestRunAction.h"

#include "G4Run.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Cowbells::TestRunAction::TestRunAction()
{
    G4cout << "Constructing Cowbells::TestRunAction" << G4endl;
    timer = new G4Timer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Cowbells::TestRunAction::~TestRunAction()
{
    G4cout << "Destructing Cowbells::TestRunAction" << G4endl;
    delete timer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Cowbells::TestRunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl; 
    timer->Start();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Cowbells::TestRunAction::EndOfRunAction(const G4Run* aRun)
{   
    timer->Stop();
    G4cout << "number of event = " << aRun->GetNumberOfEvent() 
           << " " << *timer << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
