#include "Cowbells/Interface.h"
#include "Cowbells/BuildFromRoot.h"
#include "Cowbells/PhysicsList.h"


Cowbells::Interface::Interface()
    : m_runmgr(new G4RunManager())
{
}

Cowbells::Interface::~Interface()
{
    delete(m_runmgr);
}

void Cowbells::Interface::configure(const char* geofile)
{
    m_runmgr->SetUserInitialization(new Cowbells::BuildFromRoot(geofile));
    m_runmgr->SetUserInitialization(new Cowbells::PhysicsList());
}
        
