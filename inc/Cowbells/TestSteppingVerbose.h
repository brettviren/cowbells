#ifndef TESTSTEPINGVERBOSE_H
#define TESTSTEPINGVERBOSE_H

#include "G4SteppingVerbose.hh"

namespace Cowbells {

class TestSteppingVerbose : public G4SteppingVerbose
{
public:   

    TestSteppingVerbose();
    ~TestSteppingVerbose();

    void StepInfo();
    void TrackingStarted();

};


}

#endif  // TESTSTEPINGVERBOSE_H
