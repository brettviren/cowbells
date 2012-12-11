#ifndef TESTSTACKINGACTION_H
#define TESTSTACKINGACTION_H

#include "globals.hh"
#include "G4UserStackingAction.hh"

namespace Cowbells {

    class TestStackingAction : public G4UserStackingAction
    {
    public:
        TestStackingAction();
        ~TestStackingAction();

    public:
        G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
        void NewStage();
        void PrepareNewEvent();

    private:
        G4int gammaCounter;
    };




}

#endif  // TESTSTACKINGACTION_H
