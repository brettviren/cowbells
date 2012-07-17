#ifndef TESTCB_H
#define TESTCB_H

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include <string>

namespace Cowbells {

    class TestCB {

    public:
        TestCB();
        ~TestCB();

        G4RunManager* main(std::string geofile);

    private:
        G4RunManager* m_runMgr;

    };

}

#endif  // TESTCB_H
