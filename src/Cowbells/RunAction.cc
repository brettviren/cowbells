#include "G4Timer.hh"
#include "Randomize.hh"

#include "Cowbells/RunAction.h"

#include "G4Run.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Cowbells::RunAction::RunAction()
    : G4UserRunAction()
    , m_dr(0)
{
    G4cout << "Constructing Cowbells::RunAction" << G4endl;
    timer = new G4Timer;

    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Cowbells::RunAction::~RunAction()
{
    G4cout << "Destructing Cowbells::RunAction" << G4endl;
    delete timer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Cowbells::RunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl; 
    timer->Start();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Cowbells::RunAction::EndOfRunAction(const G4Run* aRun)
{   
    timer->Stop();
    G4cout << "number of event = " << aRun->GetNumberOfEvent() 
           << " " << *timer << G4endl;

    if (!m_dr) { return; }
    G4cout << "Cowbells::RunAction: closing DataRecorder" << G4endl;
    m_dr->close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
