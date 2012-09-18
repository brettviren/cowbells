
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

Cowbells::SensitiveDetector::SensitiveDetector(const std::string& name, 
                                               const std::string& hitsname,
                                               std::vector<std::string> touchables)
    : G4VSensitiveDetector(name)
    , fHC(0)
{
    collectionName.insert(hitsname); // stupid interface....


    for (size_t ind=0; ind<touchables.size(); ++ind) {
        string tname = touchables[ind];
        m_touchId[tname] = ind;
    }
}

Cowbells::SensitiveDetector::~SensitiveDetector()
{
}


void Cowbells::SensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
    fHC = new Cowbells::HitCollection(this->SensitiveDetectorName, // stupid interface....
                                      this->collectionName[0]);

    int hcid = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection(hcid, fHC);
    //cerr << "SensitiveDetector::Initialize() with " 
    //     << this->SensitiveDetectorName << ", " <<  this->collectionName[0] << endl;

}

void Cowbells::SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    cerr << "End of event for \""
         << this->SensitiveDetectorName << "\"/\"" << this->collectionName[0] << "\" with "
         << fHC->entries() << " entries" << endl;
}

G4bool Cowbells::SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* /*nada*/)
{
    // fixme: don't accept all particles, return false for the losers

    G4StepPoint* psp = aStep->GetPreStepPoint();
    CLHEP::Hep3Vector pos = psp->GetPosition();
    G4TouchableHandle touch = psp->GetTouchableHandle();
    G4Track* track = aStep->GetTrack();

    Cowbells::Hit* hit = new Cowbells::Hit();
    fHC->insert(new Cowbells::GHit(hit));

    int depth = touch->GetHistoryDepth();
    G4VPhysicalVolume* pv = touch->GetVolume();

    TouchableId_t::iterator it = m_touchId.find(pv->GetName());
    if (it == m_touchId.end()) {
        cerr << "Hit: hit in unknown volume: \"" << pv->GetName() << "\"" << endl;

        for (int ind = touch->GetHistoryDepth(); ind >= 0; --ind) {
            pv = touch->GetVolume(ind);
            cerr << "touch: #" << ind << " " << pv->GetName() 
                 <<  " " << pv->GetCopyNo() << " " << pv->GetMultiplicity()
                 << ", " << touch->GetCopyNumber(ind) 
                 << ", " << touch->GetReplicaNumber(ind)
                 << endl;
        }

        return true;
    }
    int id = it->second;

    hit->setEnergy(track->GetTotalEnergy());
    hit->setTime(psp->GetGlobalTime());
    hit->setPos(pos.x(),pos.y(),pos.z());
    hit->setVolId(id);
    const G4ParticleDefinition* pd = track->GetParticleDefinition();
    hit->setPdgId(pd->GetPDGEncoding());

    // cerr << "Hit: in #"<<id
    //      <<" \"" << pv->GetName() << "\"" 
    //      <<" (#" << hit->pdgId()<<" "<< pd->GetParticleName() << ") "
    //      << " @ " << hit->time() 
    //      << ", " << pos.x() << ", " << pos.y() << ", " << pos.z() << endl;

    return true;
}
