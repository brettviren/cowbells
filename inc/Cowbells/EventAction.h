
#ifndef COWBELLS_EVENTACTION_H
#define COWBELLS_EVENTACTION_H

#include "Cowbells/DataRecorder.h"

#include <G4UserEventAction.hh>

#include <TFile.h>
#include <TTree.h>

namespace Cowbells {
    class EventAction : public G4UserEventAction {
    public:
        EventAction();
        virtual ~EventAction();
            
        virtual void  BeginOfEventAction(const G4Event* event);
        virtual void    EndOfEventAction(const G4Event* event);

        // Set data recorder object
        void set_recorder(Cowbells::DataRecorder* dr) { m_dr = dr; }

    private:

        Cowbells::DataRecorder* m_dr;
    };
}

#endif  // COWBELLS_EVENTACTION_H
