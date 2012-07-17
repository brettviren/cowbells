#include "Cowbells/TestMain.h"

#include <iostream>
using namespace std;

G4RunManager* Cowbells::test_run_manager(std::string physics)
{
    G4VSteppingVerbose* verbosity = new TestSteppingVerbose;
    G4VSteppingVerbose::SetInstance(verbosity);
  
    G4RunManager* runManager = new G4RunManager;
    
    G4cout << "fixme: forcing TestPhysicsList (ignoring \"" << physics << "\")" << G4endl;
    G4VUserPhysicsList* physics_list = new TestPhysicsList;

    runManager-> SetUserInitialization(physics_list);

    return runManager;
}

void Cowbells::test_main()
{
    G4VSteppingVerbose* verbosity = new TestSteppingVerbose;
    G4VSteppingVerbose::SetInstance(verbosity);
  
    // Run manager
    //
    G4RunManager* runManager = new G4RunManager;

    // UserInitialization classes - mandatory
    //
    G4VUserPhysicsList* physics = new TestPhysicsList;
    runManager-> SetUserInitialization(physics);
    //
    G4VUserPrimaryGeneratorAction* gen_action = new TestPrimaryGeneratorAction;
    runManager->SetUserAction(gen_action);
    //
    G4VUserDetectorConstruction* detector = new TestDetectorConstruction;
    runManager-> SetUserInitialization(detector);
    //
    G4UserRunAction* run_action = new TestRunAction;
    runManager->SetUserAction(run_action);
    //
    G4UserStackingAction* stacking_action = new TestStackingAction;
    runManager->SetUserAction(stacking_action);
  
    // Initialize G4 kernel
    //
    runManager->Initialize();

    G4UImanager* UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/run/verbose 1");
    UI->ApplyCommand("/event/verbose 1");
    UI->ApplyCommand("/tracking/verbose 1");

    runManager->BeamOn(3);

    delete runManager;
}

Cowbells::TestMain::TestMain()
    : m_runMgr(0)
    , m_physList(0)
    , m_primGenAct(0)
    , m_detCons(0)
    , m_runAct(0)
    , m_stackAct(0)    
    , m_uiMgr(0)
{
    cerr << "Constructing TestMain" << endl;
}

Cowbells::TestMain::~TestMain()
{
    cerr << "Constructing TestMain" << endl;
    if (m_runMgr) delete m_runMgr; m_runMgr = 0;
}
        
G4RunManager* Cowbells::TestMain::run_manager()
{
    if (!m_runMgr) m_runMgr = new G4RunManager();
    return m_runMgr;
}
G4VUserPhysicsList* Cowbells::TestMain::physics_list()
{
    if (!m_physList) {
        m_physList = new Cowbells::TestPhysicsList();
        this->run_manager()->SetUserInitialization(m_physList);
    }
    return m_physList;
}
G4VUserPrimaryGeneratorAction* Cowbells::TestMain::primary_generator_action()
{
    if (!m_primGenAct) {
        m_primGenAct = new Cowbells::TestPrimaryGeneratorAction();
        this->run_manager()->SetUserAction(m_primGenAct);
    }
    return m_primGenAct;
}
G4VUserDetectorConstruction* Cowbells::TestMain::detector_construction()
{
    if (!m_detCons) {
        m_detCons = new Cowbells::TestDetectorConstruction();
        //this->run_manager()->SetUserInitialization(m_detCons);
    }
    return m_detCons;
}
G4UserRunAction* Cowbells::TestMain::user_run_action()
{
    if (!m_runAct) {
        m_runAct = new Cowbells::TestRunAction();
        //this->run_manager()->SetUserAction(m_runAct);
    }
    return m_runAct;
}
G4UserStackingAction* Cowbells::TestMain::user_stack_action()
{
    if (!m_stackAct) {
        m_stackAct = new Cowbells::TestStackingAction();
        //this->run_manager()->SetUserAction(m_stackAct);
    }
    return m_stackAct;
}
G4UImanager* Cowbells::TestMain::ui_manager()
{
    if (!m_uiMgr) m_uiMgr = G4UImanager::GetUIpointer();
    return m_uiMgr;
}
