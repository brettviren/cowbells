#ifndef SENSITIVEDETECTOR_H
#define SENSITIVEDETECTOR_H

#include "Cowbells/Hit.h"

#include <G4HCofThisEvent.hh>
#include <G4TouchableHistory.hh>
#include <G4VSensitiveDetector.hh>

#include <string>


namespace Cowbells {

class SensitiveDetector : public G4VSensitiveDetector 
{
public:
    SensitiveDetector(const std::string& name, 
                      const std::string& hitsname);
    virtual ~SensitiveDetector();


    // optional interface
    virtual void Initialize(G4HCofThisEvent* hce);
    virtual void EndOfEvent(G4HCofThisEvent* hce);
    virtual void clear();

    // required interface 
    virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);

private:
    Cowbells::HitCollection* fHC;
};


}

#endif  // SENSITIVEDETECTOR_H
