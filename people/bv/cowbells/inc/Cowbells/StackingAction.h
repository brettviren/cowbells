#ifndef STACKINGACTION_H
#define STACKINGACTION_H

#include "Cowbells/DataRecorder.h"

#include "globals.hh"
#include "G4UserStackingAction.hh"

namespace Cowbells {

    class StackingAction : public G4UserStackingAction
    {
    public:
        StackingAction();
        ~StackingAction();

        // Set data recorder object
        void set_recorder(Cowbells::DataRecorder* dr) { m_dr = dr; }

    public:
        G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
        void NewStage();
        void PrepareNewEvent();

    private:
        G4int gammaCounter;
        Cowbells::DataRecorder* m_dr;
    };




}

#endif  // STACKINGACTION_H
