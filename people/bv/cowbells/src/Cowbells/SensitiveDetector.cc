
#include "Cowbells/SensitiveDetector.h"

#include <G4Step.hh>
#include <G4SDManager.hh>

#include <iostream>
using namespace std;

Cowbells::SensitiveDetector::SensitiveDetector(const std::string& name, 
                                               const std::string& hitsname)
    : G4VSensitiveDetector(name)
    , fHC(0)
{
    collectionName.insert(hitsname); // stupid interface....
}

Cowbells::SensitiveDetector::~SensitiveDetector()
{
}


//  These two methods are invoked at the begining and at the end of each
// event. The hits collection(s) created by this sensitive detector must
// be set to the G4HCofThisEvent object at one of these two methods.
void Cowbells::SensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
    fHC = new Cowbells::HitCollection(this->SensitiveDetectorName, // stupid interface....
                                      this->collectionName[0]);
    int hcid = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcid, fHC);
    cerr << "SensitiveDetector::Initialize() with " 
         << this->SensitiveDetectorName << ", " <<  this->collectionName[0] << endl;

}

void Cowbells::SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    cerr << "End of event for \""
         << this->SensitiveDetectorName << "\"/\"" << this->collectionName[0] << "\" with "
         << fHC->entries() << " entries" << endl;
}

//  This method is invoked if the event abortion is occured. Hits collections
// created but not beibg set to G4HCofThisEvent at the event should be deleted.
// Collection(s) which have already set to G4HCofThisEvent will be deleted 
// automatically.
void Cowbells::SensitiveDetector::clear()
{
}


//  The user MUST implement this method for generating hit(s) using the 
// information of G4Step object. Note that the volume and the position
// information is kept in PreStepPoint of G4Step.
//  Be aware that this method is a protected method and it sill be invoked 
// by Hit() method of Base class after Readout geometry associated to the
// sensitive detector is handled.
//  "ROhist" will be given only is a Readout geometry is defined to this
// sensitive detector. The G4TouchableHistory object of the tracking geometry
// is stored in the PreStepPoint object of G4Step.
G4bool Cowbells::SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* /*nada*/)
{
    // fixme: don't accept all particles, return false for the losers

    G4StepPoint* psp = aStep->GetPreStepPoint();
    CLHEP::Hep3Vector pos = psp->GetPosition();
    G4TouchableHandle touch = psp->GetTouchableHandle();
    G4Track* track = aStep->GetTrack();

    Cowbells::Hit* hit = new Cowbells::Hit();
    hit->setEnergy(track->GetTotalEnergy());
    hit->setTime(psp->GetGlobalTime());
    hit->setPos(pos.x(),pos.y(),pos.z());
    hit->setVolId(touch->GetCopyNumber());
    fHC->insert(hit);


    int depth = touch->GetHistoryDepth();
    G4VPhysicalVolume* pv = touch->GetVolume();

    cerr << "Hit: in \"" << pv->GetName() << "\"" << " @ " << hit->time() << ", " << pos.x() << ", " << pos.y() << ", " << pos.z() << endl;

    for (int ind = touch->GetHistoryDepth(); ind >= 0; --ind) {
      pv = touch->GetVolume(ind);
      cerr << "touch: #" << ind << " " << pv->GetName() <<  " " << pv->GetCopyNo() << " " << pv->GetMultiplicity()
	   << ", " << touch->GetCopyNumber(ind) 
	   << ", " << touch->GetReplicaNumber(ind)
	   << endl;
    }

    // do {
    //     cerr << "\t" << pv->GetName() 
    //          << " depth:" << depth
    //          << " copy:" << touch->GetCopyNumber() 
    //          << " repl:" << touch->GetReplicaNumber() 
    //          << " hist:" << touch->GetHistoryDepth()
    //          << endl;
    //     touch->MoveUpHistory(1);
    //     pv = touch->GetVolume();
    //     --depth;
    // } while (depth >= 0);

    return true;
}
