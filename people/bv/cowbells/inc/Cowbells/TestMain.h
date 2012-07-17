#ifndef TESTMAIN_H
#define TESTMAIN_H

#include "Cowbells/TestDetectorConstruction.h"
#include "Cowbells/TestPhysicsList.h"
#include "Cowbells/TestPrimaryGeneratorAction.h"
#include "Cowbells/TestRunAction.h"
#include "Cowbells/TestStackingAction.h"
#include "Cowbells/TestSteppingVerbose.h"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include <string>

namespace Cowbells {

    G4RunManager* test_run_manager(std::string physics = "test");
    void test_main();

    class TestMain {
    public:
        TestMain();
        ~TestMain();
        
        G4RunManager* run_manager();
        G4VUserPhysicsList* physics_list();
        G4VUserPrimaryGeneratorAction* primary_generator_action();
        G4VUserDetectorConstruction* detector_construction();
        G4UserRunAction* user_run_action();
        G4UserStackingAction* user_stack_action();
        G4UImanager* ui_manager();

    private:
        G4RunManager* m_runMgr;
        G4VUserPhysicsList* m_physList;
        G4VUserPrimaryGeneratorAction* m_primGenAct;
        G4VUserDetectorConstruction* m_detCons;
        G4UserRunAction* m_runAct;
        G4UserStackingAction* m_stackAct;    
        G4UImanager* m_uiMgr;
    };
}

#endif  // TESTMAIN_H
