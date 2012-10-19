#include "Cowbells/PhysicsList.h"
#include "Cowbells/PrimaryGeneratorBeam.h"
#include "Cowbells/PrimaryGeneratorFile.h"
#include "Cowbells/TestDetectorConstruction.h"
#include "Cowbells/BuildFromJson.h"
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

    Cowbells::BuildFromJson* detcon = new Cowbells::BuildFromJson();
    {
        const char* cf = opt(oCONFIG);
        if (!cf) {
            cerr << "No config files!" << endl;
            return 1;
        }
        vector<std::string> cfgfiles = Cowbells::split(cf,",");
        for (size_t ind=0; ind<cfgfiles.size(); ++ind) {
            cout << "Using configuration file: " << cfgfiles[ind] << endl;
            detcon->addfile(cfgfiles[ind]);
        }
    }
    rm.SetUserInitialization(detcon);

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
