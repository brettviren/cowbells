#include "Cowbells/EventAction.h"

#include "Randomize.hh"

#include <iostream>
using namespace std;

Cowbells::EventAction::EventAction()
    : G4UserEventAction()
    , m_dr(0)
{
    G4cout << "Constructing Cowbells::EventAction" << G4endl;
}

Cowbells::EventAction::~EventAction()
{
    G4cout << "Destructing Cowbells::EventAction" << G4endl;
}

            
void Cowbells::EventAction::BeginOfEventAction(const G4Event* event)
{
    //G4cout << "Cowbells::EventAction::BeginOfEventAction()" << G4endl;
}


void Cowbells::EventAction::EndOfEventAction(const G4Event* event) 
{
    //G4cout << "Cowbells::EventAction::EndOfEventAction()" << G4endl;
    if (!m_dr) return;
    m_dr->fill(event);
}


