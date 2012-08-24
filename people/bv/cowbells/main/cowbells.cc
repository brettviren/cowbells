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

using namespace std;

int main(int argc, char *argv[])
{
    if (!parse_args(argc, argv)) {
        return 1;
    }

    
    ROOT::Cintex::Cintex::Enable();

    G4RunManager rm;
    
    Cowbells::PhysicsList* pl = new Cowbells::PhysicsList();
    rm.SetUserInitialization(pl);

    Cowbells::PrimaryGenerator* pg = new Cowbells::PrimaryGenerator(opt(oKIN));
    rm.SetUserAction(pg);

    std::string geofile = opt(oGEOMETRY);
    Cowbells::BuildFromRoot* detcon = new Cowbells::BuildFromRoot(geofile);

    std::vector<std::string> paths; // fake it until you make it
    paths.push_back("");
    paths.push_back("PC");
    // paths.push_back("&cowbells_1%endcap_1%PC");
    // paths.push_back("&cowbells_1%endcap_2%PC");
    detcon->add_sensdet("PC", paths);
    rm.SetUserInitialization(detcon);

    Cowbells::DataRecorder* dr = 0;

    std::string outputfile = opt(oOUTPUT);
    if ("none" != outputfile) {
        dr = new Cowbells::DataRecorder(outputfile);
    }

    Cowbells::RunAction* ura = new Cowbells::RunAction();
    if (dr) { ura->set_recorder(dr); }
    rm.SetUserAction(ura);

    Cowbells::EventAction* ea = new Cowbells::EventAction();
    if (dr) { ea->set_recorder(dr); }
    rm.SetUserAction(ea);

    Cowbells::TestStackingAction* usa = new Cowbells::TestStackingAction();
    rm.SetUserAction(usa);

    rm.Initialize();
    
    G4VisExecutive *vm = new G4VisExecutive("all");
    vm->Initialize();

    G4UImanager* um = G4UImanager::GetUIpointer();

    // apply command line args.
    G4UIExecutive * ui = 0;
    if (opt(oUI)) {
        cerr << "User interface requested" << endl;
        ui = new G4UIExecutive(0,0);
    }
    else {
        cerr << "Batch mode." << endl;
    }

    for (int ind=0; ind<nargs(); ++ind) {
        std::string cmd = "/control/execute ";
        cmd += arg(ind);
        std::cerr << cmd << std::endl;
        um->ApplyCommand(cmd);
    }

    if (opt(oNEVENTS)) {
        int nevents = atol(opt(oNEVENTS));
        assert (nevents);
        rm.BeamOn(nevents);
    }


    if (ui) {
        ui->SessionStart();
        delete ui;
    }

    if (vm) {delete vm; vm = 0;}
    if (dr) {delete dr; dr = 0;}
    return 0;
}
