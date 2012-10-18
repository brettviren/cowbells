#include "Cowbells/PhysicsList.h"
#include "Cowbells/PrimaryGeneratorBeam.h"
#include "Cowbells/PrimaryGeneratorFile.h"
#include "Cowbells/TestDetectorConstruction.h"
#include "Cowbells/BuildFromRoot.h"
#include "Cowbells/BuildByHand.h"
#include "Cowbells/DataRecorder.h"
#include "Cowbells/RunAction.h"
#include "Cowbells/EventAction.h"
#include "Cowbells/StackingAction.h"
#include "Cowbells/SteppingAction.h"
#include "Cowbells/strutil.h"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Cintex/Cintex.h"

#include <iostream>
#include "cmdline.h"

using namespace std;
using Cowbells::uri_split;

int main(int argc, char *argv[])
{
    if (!parse_args(argc, argv)) {
        return 1;
    }

    
    ROOT::Cintex::Cintex::Enable();

    G4RunManager rm;
    
    float defcut = 1.0;
    if (opt(oDEFCUT)) {
        defcut = atof(opt(oDEFCUT));
    }
    Cowbells::PhysicsList* pl = new Cowbells::PhysicsList(opt(oPHYSICS), defcut);
    rm.SetUserInitialization(pl);

    G4VUserPrimaryGeneratorAction* pg = 0;
    vector<string> kin = uri_split(opt(oKIN));
    if (kin[0] == "file") {     // file://path/to/file.txt
        cout << "File based generator with " << kin[1] << endl;
        pg = new Cowbells::PrimaryGeneratorFile(kin[1].c_str());
    }
    else if (kin[0] == "kin") { // kin://beam?vertex=.....
        assert (kin.size() > 1);
        if (kin[1] == "beam") {
            assert (kin.size() > 2);
            cout << "Beam generator with " << kin[2] << endl;
            pg = new Cowbells::PrimaryGeneratorBeam(kin[2].c_str());
        }
    }
    assert(pg);

    rm.SetUserAction(pg);

    std::string geofile = opt(oGEOMETRY);
    
    Cowbells::DataRecorder* dr = 0;

    std::string outputfile = opt(oOUTPUT);
    if ("none" != outputfile) {
        dr = new Cowbells::DataRecorder(outputfile.c_str());
        dr->save_steps();
    }

    if (false) {
        Cowbells::BuildFromRoot* detcon = new Cowbells::BuildFromRoot(geofile);

        {                                       
            std::vector<std::string> tub_paths; // fake it until you make it
            tub_paths.push_back("");
            tub_paths.push_back("Top:1/TubTeflon:1/Window:1/TUB_PC:1");
            tub_paths.push_back("Top:1/TubAluminum:1/Window:1/TUB_PC:1");
            detcon->add_sensdet("TUB_PC", tub_paths, "TUB_PC_HC", "/cowbells/tub");
            if (dr) dr->add_hc("TUB_PC_HC");
        }

        {
            std::vector<std::string> tc_paths;
            tc_paths.push_back("");
            tc_paths.push_back("Top:1/TC_PC:1");
            tc_paths.push_back("Top:1/TC_PC:2");
            tc_paths.push_back("Top:1/TC_PC:3");
            detcon->add_sensdet("TC_PC", tc_paths, "TC_PC_HC", "/cowbells/tc");
            if (dr) dr->add_hc("TC_PC_HC");
        }

        rm.SetUserInitialization(detcon);
    }
    else {
        Cowbells::BuildByHand* detcon = new Cowbells::BuildByHand(geofile);

        {
            std::vector<std::string> paths; // fake it until you make it
            paths.push_back("");
            paths.push_back("World:0/TubTeflon:0/TubdetWindow:0/PC:0");
            paths.push_back("World:0/TubAluminum:0/TubdetWindow:0/PC:0");
            detcon->add_sensdet("lvTubPC", paths, "TUB_PC_HC", "/cowbells/tub");
            if (dr) dr->add_hc("TUB_PC_HC");
        }

        {
            std::vector<std::string> paths;
            paths.push_back("");
            paths.push_back("World:0/TriggerCounter:0");
            paths.push_back("World:0/TriggerCounter:1");
            paths.push_back("World:0/TriggerCounter:2");
            detcon->add_sensdet("lvTCPC", paths, "TC_PC_HC", "/cowbells/tc");
            if (dr) dr->add_hc("TC_PC_HC");
        }

        rm.SetUserInitialization(detcon);
    }

    Cowbells::RunAction* ura = new Cowbells::RunAction();
    if (dr) { ura->set_recorder(dr); }
    rm.SetUserAction(ura);

    Cowbells::EventAction* ea = new Cowbells::EventAction();
    if (dr) { ea->set_recorder(dr); }
    rm.SetUserAction(ea);

    Cowbells::StackingAction* sta = new Cowbells::StackingAction();
    rm.SetUserAction(sta);

    if (dr) {
        Cowbells::SteppingAction* ste = new Cowbells::SteppingAction();
        ste->set_recorder(dr);
        rm.SetUserAction(ste);
    }

    rm.Initialize();
    
    cout << *G4Material::GetMaterialTable() << endl;

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
