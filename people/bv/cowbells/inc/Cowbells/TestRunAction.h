#ifndef TESTRUNACTION_H
#define TESTRUNACTION_H

#include "G4UserRunAction.hh"

class G4Timer;
class G4Run;

namespace Cowbells {

    class TestRunAction : public G4UserRunAction
    {
    public:
        TestRunAction();
        ~TestRunAction();

    public:
        void BeginOfRunAction(const G4Run* aRun);
        void EndOfRunAction(const G4Run* aRun);

    private:
        G4Timer* timer;
    };


}

#endif  // TESTRUNACTION_H
