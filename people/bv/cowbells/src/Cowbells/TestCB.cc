#include "Cowbells/TestCB.h"

#include "Cowbells/PhysicsList.h"
#include "Cowbells/PrimaryGenerator.h"
#include "Cowbells/BuildFromRoot.h"
#include "Cowbells/TestRunAction.h"
#include "Cowbells/TestStackingAction.h"

#include <iostream>
using namespace std;

Cowbells::TestCB::TestCB()
    : m_runMgr(0)
{
}
Cowbells::TestCB::~TestCB()
{
    if (m_runMgr) delete m_runMgr; m_runMgr = 0;
}


G4RunManager* Cowbells::TestCB::main(std::string geofile)
{
    if (m_runMgr) return m_runMgr;

    m_runMgr = new G4RunManager;


    G4VUserPhysicsList* physics = new Cowbells::PhysicsList;
    m_runMgr-> SetUserInitialization(physics);
    //
    G4VUserPrimaryGeneratorAction* gen_action = new Cowbells::PrimaryGenerator;
    m_runMgr->SetUserAction(gen_action);
    //
    G4VUserDetectorConstruction* detector = new Cowbells::BuildFromRoot(geofile.c_str());
    m_runMgr-> SetUserInitialization(detector);
    //
    G4UserRunAction* run_action = new TestRunAction;
    m_runMgr->SetUserAction(run_action);
    //
    G4UserStackingAction* stacking_action = new TestStackingAction;
    m_runMgr->SetUserAction(stacking_action);
  
    // Initialize G4 kernel
    //
    m_runMgr->Initialize();

    G4UImanager* UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/run/verbose 1");
    UI->ApplyCommand("/event/verbose 1");
    UI->ApplyCommand("/tracking/verbose 1");

    const G4MaterialTable& matv = *G4Material::GetMaterialTable();
    //cerr << "Materials:\n" << *(G4Material::GetMaterialTable()) << endl;
    size_t nmat = matv.size();
    for (size_t imat = 0; imat<nmat; ++imat) {
        G4Material* mat = matv[imat];
        cerr << mat->GetName() << " dens=" << mat->GetDensity() << " radlen=" << mat->GetRadlen() << " intlen=" << mat->GetNuclearInterLength() << endl;
        G4MaterialPropertiesTable* mattab = mat->GetMaterialPropertiesTable();
        if (mattab) {
            mattab->DumpTable();
        }
        else {
            cerr << "No MaterialPropertiesTable for " << mat->GetName() << endl;
        }
    }

    return m_runMgr;
}

