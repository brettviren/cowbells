/**
 * \class RunAction
 *
 * \brief Run stuff during the run.
 *
 * This drives the final closing of the DataRecorder
 *
 * bv@bnl.gov Tue Aug  7 09:20:03 2012
 *
 */



#ifndef RUNACTION_H
#define RUNACTION_H


#include "Cowbells/DataRecorder.h"
#include "G4UserRunAction.hh"

class G4Timer;
class G4Run;

namespace Cowbells {

    class RunAction : public G4UserRunAction
    {
    public:
        RunAction();
        ~RunAction();

    public:
        void BeginOfRunAction(const G4Run* aRun);
        void EndOfRunAction(const G4Run* aRun);

        // Set data recorder object
        void set_recorder(Cowbells::DataRecorder* dr) { m_dr = dr; }


    private:
        G4Timer* timer;

        Cowbells::DataRecorder* m_dr;
    };


}

#endif  // RUNACTION_H
