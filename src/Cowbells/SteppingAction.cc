#include "Cowbells/SteppingAction.h"


#include <string>
using namespace std;

Cowbells::SteppingAction::SteppingAction()
    : G4UserSteppingAction()
    , m_dr(0)
{
}

Cowbells::SteppingAction::~SteppingAction()
{
}
        

void Cowbells::SteppingAction::UserSteppingAction(const G4Step* step)
{
    m_dr->add_step(step);
}
