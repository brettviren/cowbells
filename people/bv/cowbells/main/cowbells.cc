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

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Cintex/Cintex.h"

#include <iostream>
#include "cmdline.h"

int main(int argc, char *argv[])
{
    if (!parse_args(argc, argv)) {
        return 1;
    }

    
    ROOT::Cintex::Cintex::Enable();

    G4RunManager *rm = new G4RunManager;
    
    Cowbells::PhysicsList* pl = new Cowbells::PhysicsList();
    rm->SetUserInitialization(pl);

    Cowbells::PrimaryGenerator* pg = new Cowbells::PrimaryGenerator();
    rm->SetUserAction(pg);

    std::string geofile = arg(oGEOMETRY);
    Cowbells::BuildFromRoot* detcon = new Cowbells::BuildFromRoot(geofile);

    std::vector<std::string> paths; // fake it until you make it
    paths.push_back("");
    paths.push_back("PC");
    // paths.push_back("&cowbells_1%endcap_1%PC");
    // paths.push_back("&cowbells_1%endcap_2%PC");
    detcon->add_sensdet("PC", paths);
    rm->SetUserInitialization(detcon);

    
    std::string outfile = arg(oOUTPUT);
    Cowbells::DataRecorder* dr = new Cowbells::DataRecorder(outfile);

    Cowbells::RunAction* ura = new Cowbells::RunAction();
    if (dr) { ura->set_recorder(dr); }
    rm->SetUserAction(ura);

    Cowbells::EventAction* ea = new Cowbells::EventAction();
    if (dr) { ea->set_recorder(dr); }
    rm->SetUserAction(ea);

    Cowbells::TestStackingAction* usa = new Cowbells::TestStackingAction();
    rm->SetUserAction(usa);

    rm->Initialize();
    
    G4VisManager* vm = new G4VisExecutive;
    vm->Initialize();

    G4UImanager* um = G4UImanager::GetUIpointer();

    um->ApplyCommand("/run/verbose 0");
    um->ApplyCommand("/event/verbose 0");
    um->ApplyCommand("/tracking/verbose 0");
    um->ApplyCommand("/WLS/phys/verbose 0");

    //rm->BeamOn(10);

    // apply command line args.
    if (arg(oUI)) {
        G4UIExecutive ui(argc,argv);
        ui.SessionStart();
    }
    else {
        // batch
    }

    delete vm;
    delete rm;

    return 0;
}
