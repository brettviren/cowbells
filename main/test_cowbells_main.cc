#include "Cowbells/PhysicsList.h"
#include "Cowbells/PrimaryGenerator.h"
#include "Cowbells/TestDetectorConstruction.h"
#include "Cowbells/BuildFromRoot.h"
#include "Cowbells/DataRecorder.h"
#include "Cowbells/RunAction.h"
#include "Cowbells/EventAction.h"
#include "Cowbells/TestStackingAction.h"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "Cintex/Cintex.h"

int main(int argc, char *argv[])
{
    ROOT::Cintex::Cintex::Enable();


    G4RunManager *rm = new G4RunManager;
    
    Cowbells::PhysicsList* pl = new Cowbells::PhysicsList();
    rm->SetUserInitialization(pl);

    Cowbells::PrimaryGenerator* pg = new Cowbells::PrimaryGenerator();
    rm->SetUserAction(pg);

    std::string geofile = "tubdet.root";
    Cowbells::BuildFromRoot* detcon = new Cowbells::BuildFromRoot(geofile);

    std::vector<std::string> paths; // fake it until you make it
    paths.push_back("");
    paths.push_back("PC");
    // paths.push_back("&cowbells_1%endcap_1%PC");
    // paths.push_back("&cowbells_1%endcap_2%PC");
    detcon->add_sensdet("PC", paths);
    rm->SetUserInitialization(detcon);

    Cowbells::DataRecorder* dr = new Cowbells::DataRecorder("test_cowbells.root");

    Cowbells::RunAction* ura = new Cowbells::RunAction();
    if (dr) { ura->set_recorder(dr); }
    rm->SetUserAction(ura);

    Cowbells::EventAction* ea = new Cowbells::EventAction();
    if (dr) { ea->set_recorder(dr); }
    rm->SetUserAction(ea);

    Cowbells::TestStackingAction* usa = new Cowbells::TestStackingAction();
    rm->SetUserAction(usa);

    rm->Initialize();
    
    G4UImanager* UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/run/verbose 0");
    UI->ApplyCommand("/event/verbose 0");
    UI->ApplyCommand("/tracking/verbose 0");
    UI->ApplyCommand("/WLS/phys/verbose 0");

    rm->BeamOn(10);

    delete rm;

    return 0;
}
