#include "Cowbells/Json2G4.h"
#include "Cowbells/PhysicsList.h"
#include "Cowbells/PrimaryGeneratorGun.h"
#include "Cowbells/PrimaryGeneratorBeam.h"
#include "Cowbells/PrimaryGeneratorFile.h"
#include "Cowbells/DetectorConstruction.h"
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


G4VUserPrimaryGeneratorAction* make_generator(Json::Value cfg, const char* okin)
{
    if (okin) {                 // command line take precedence
        cerr << "Using command line option: \"" << okin << "\"" << endl;
        vector<string> kin = uri_split(opt(oKIN));
        if (!kin.size()) {
            cerr << "Bad kinematics URL: \"" << okin << "\"" << endl;
            return 0;
        }

        if (kin[0] == "file") {     // file://path/to/file.txt
            cout << "File based generator with " << kin[1] << endl;
            return new Cowbells::PrimaryGeneratorFile(kin[1].c_str());
        }
        if (kin[0] == "kin") { // kin://beam?vertex=.....
            assert (kin.size() > 1);
            if (kin[1] == "beam") {
                assert (kin.size() > 2);
                cout << "Beam generator with " << kin[2] << endl;
                return new Cowbells::PrimaryGeneratorBeam(kin[2].c_str());
            }
        }
        cerr << "Unknown kinematics option: " << okin << endl;
        return 0;
    }

    
    string kintype = cfg["type"].asString();

    if (kintype == "hepmcfile") {
        string fname = cfg["filename"].asString();
        cout << "HepMC file-based generator with file: " << fname << endl;
        return new Cowbells::PrimaryGeneratorFile(fname.c_str());
    }
    
    if (kintype == "url") {
        string url = cfg["url"].asString();
        cout << "URL-based generator with url: " << url << endl;
        return new Cowbells::PrimaryGeneratorBeam(url.c_str());
    }

    if (kintype == "gun") {
        cout << "Gun-based generator" << endl;
        return new Cowbells::PrimaryGeneratorGun(cfg);
    }

    return 0;
}


int main(int argc, char *argv[])
{
    ROOT::Cintex::Cintex::Enable();


    if (!parse_args(argc, argv)) {
        return 1;
    }

    Cowbells::Json2G4 j2g4;
    vector<string> macfiles;
    for (int ind = 0; ind < parser->nonOptionsCount(); ++ind) {
        string fname = parser->nonOption(ind);
        string::size_type dot = fname.rfind(".");
        string ext = fname.substr(dot+1,fname.size()-dot-1);

        if (ext == "mac") {
            macfiles.push_back(fname);
            continue;
        }

        j2g4.add_file(fname);
    }
    j2g4.read();

    G4RunManager rm;
    
    Cowbells::PhysicsList* pl = new Cowbells::PhysicsList(j2g4);
    rm.SetUserInitialization(pl);
    
    G4VUserPrimaryGeneratorAction* pg = make_generator(j2g4.get("kinematics"), opt(oKIN));
    assert(pg);
    rm.SetUserAction(pg);
    
    Cowbells::DataRecorder* dr = 0;

    std::string outputfile = opt(oOUTPUT);
    if ("none" != outputfile) {
        dr = new Cowbells::DataRecorder(outputfile.c_str(), j2g4.get("sensitive"));
        dr->save_steps();
    }

    Cowbells::DetectorConstruction* detcon = new Cowbells::DetectorConstruction(j2g4);
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

    for (size_t ind=0; ind<macfiles.size(); ++ind) {
        std::string cmd = "/control/execute ";
        cmd += macfiles[ind];
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
