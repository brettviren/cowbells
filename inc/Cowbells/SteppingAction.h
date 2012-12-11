#ifndef STEPPINGACTION_H
#define STEPPINGACTION_H

#include "Cowbells/DataRecorder.h"
#include <G4UserSteppingAction.hh>

namespace Cowbells {

    class SteppingAction : public G4UserSteppingAction 
    {
    public:
        SteppingAction();
        virtual ~SteppingAction();
        
        virtual void UserSteppingAction(const G4Step* step);

        // Set data recorder object
        void set_recorder(Cowbells::DataRecorder* dr) { m_dr = dr; }

    private:
        Cowbells::DataRecorder* m_dr;
    };

}


#endif  // STEPPINGACTION_H
