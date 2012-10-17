
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
    cout << "SensitiveDetector(" <<name<< "," << hitsname << ")" << endl;
}

Cowbells::SensitiveDetector::SensitiveDetector(const std::string& name, 
                                               const std::string& hitsname,
                                               std::vector<std::string> touchables)
    : G4VSensitiveDetector(name)
    , fHC(0)
{
    collectionName.insert(hitsname); // stupid interface....
    cout << "SensitiveDetector(" <<name<< "," << hitsname << ")" << endl;

    for (size_t ind=0; ind<touchables.size(); ++ind) {
        string tname = touchables[ind];
        m_touchNameId[tname] = ind;
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
    cout << "SensitiveDetector::Initialize() hcid=" << hcid
         << ", SD name=" << this->SensitiveDetectorName 
         << ", HC name=" << this->collectionName[0] << " with "
         <<  this->collectionName.size() << " collections:" << endl;
    for (size_t ind=0; ind < this->collectionName.size(); ++ind) {
        cout << "\t#" << ind << ": " << this->collectionName[ind] << endl;
    }

}

void Cowbells::SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    cerr << "End of event for \""
         << this->SensitiveDetectorName << "\"/\"" << this->collectionName[0] << "\" with "
         << fHC->entries() << " entries" << endl;
}

static std::string make_touchable_name(G4TouchableHandle& touch)
{
    stringstream ss;
    string comma = "";

    for (int ind = touch->GetHistoryDepth(); ind >= 0; --ind) {
        G4VPhysicalVolume* pv = touch->GetVolume(ind);
        // cerr << "touch: #" << ind << " " << pv->GetName() 
        //      <<  " " << pv->GetCopyNo() << " " << pv->GetMultiplicity()
        //      << ", " << touch->GetCopyNumber(ind) 
        //      << ", " << touch->GetReplicaNumber(ind)
        //      << endl;
        ss << comma << pv->GetName() << ":" << touch->GetCopyNumber(ind);
        comma = "/";
    }
   return ss.str();
}

int Cowbells::SensitiveDetector::divine_touchable_id(const std::string& tname)
{
    TouchableNameId_t::iterator it = m_touchNameId.find(tname);
    if (it == m_touchNameId.end()) { return 0; }
    return it->second;
}

G4bool Cowbells::SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* /*nada*/)
{
    // fixme: don't accept all particles, return false for the losers

    G4StepPoint* psp = aStep->GetPreStepPoint();
    CLHEP::Hep3Vector pos = psp->GetPosition();
    G4TouchableHandle touch = psp->GetTouchableHandle();
    G4Track* track = aStep->GetTrack();

    int depth = touch->GetHistoryDepth();
    G4VPhysicalVolume* pv = touch->GetVolume();
    G4LogicalVolume* lv = pv->GetLogicalVolume();

    if (!fHC) {
        cerr  << "No hit collection for PV:" << pv->GetName() << endl;
        return true;
    }

    Cowbells::Hit* hit = new Cowbells::Hit();
    fHC->insert(new Cowbells::GHit(hit));

    string tname = make_touchable_name(touch);
    int id = divine_touchable_id(tname);
    if (!id) {
        cerr << "Hit: hit in unknown volume: \"" << tname << "\"" << endl;
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

    hit->setEnergy(track->GetTotalEnergy());
    hit->setTime(psp->GetGlobalTime());
    hit->setPos(pos.x(),pos.y(),pos.z());
    hit->setVolId(id);
    const G4ParticleDefinition* pd = track->GetParticleDefinition();
    hit->setPdgId(pd->GetPDGEncoding());

    // cerr << "Hit: in "
    //      << " LV:" << lv->GetName() << " "
    //      << "#"<<id
    //      <<" \"" << tname << "\"" 
    //      <<" (#" << hit->pdgId()<<" "<< pd->GetParticleName() << ") "
    //      << " @ " << hit->time() 
    //      << ", " << pos.x() << ", " << pos.y() << ", " << pos.z() << endl;

    // for (int ind = touch->GetHistoryDepth(); ind >= 0; --ind) {
    //     pv = touch->GetVolume(ind);
    //     cerr << "touch: #" << ind << " " << pv->GetName() 
    //          <<  " " << pv->GetCopyNo() << " " << pv->GetMultiplicity()
    //          << ", " << touch->GetCopyNumber(ind) 
    //          << ", " << touch->GetReplicaNumber(ind)
    //          << endl;
    // }
    return true;
}
