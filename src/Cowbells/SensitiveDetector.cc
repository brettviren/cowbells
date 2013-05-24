
#include "Cowbells/SensitiveDetector.h"
#include "Cowbells/TrackInformation.h"

#include <G4Step.hh>
#include <G4SDManager.hh>
#include <Randomize.hh>
#include <G4Material.hh>
#include <G4OpticalPhoton.hh>

#include <cassert>
#include <iostream>
using namespace std;

Cowbells::SensitiveDetector::SensitiveDetector(const std::string& name, 
                                               const std::string& hitsname)
    : G4VSensitiveDetector(name)
    , fHC(0)
{
    collectionName.insert(hitsname); // stupid interface....
    //cout << "SensitiveDetector(" <<name<< "," << hitsname << ")" << endl;
}

Cowbells::SensitiveDetector::SensitiveDetector(const std::string& name, 
                                               const std::string& hitsname,
                                               std::vector<std::string> touchables)
    : G4VSensitiveDetector(name)
    , fHC(0)
{
    collectionName.insert(hitsname); // stupid interface....
    cout << "SensitiveDetector(" <<name<< "," << hitsname
	 << ", " << touchables.size() << ")" << endl;

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
    // cout << "SensitiveDetector::Initialize() hcid=" << hcid
    //      << ", SD name=" << this->SensitiveDetectorName 
    //      << ", HC name=" << this->collectionName[0] << " with "
    //      <<  this->collectionName.size() << " collections:" << endl;
    // for (size_t ind=0; ind < this->collectionName.size(); ++ind) {
    //     cout << "\t#" << ind << ": " << this->collectionName[ind] << endl;
    // }

}

void Cowbells::SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
//    cerr << "End of event for \""
//         << this->SensitiveDetectorName << "\"/\"" << this->collectionName[0] << "\" with "
//         << fHC->entries() << " entries" << endl;
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
    if (it == m_touchNameId.end()) { return -1; }
    return it->second;
}

bool pass_qe(G4Track* track)
{
    G4Material* mat = track->GetMaterial();
    if (!mat) {
        cerr << "Fail QE due to no material" << endl;
        assert(mat);
        return false;
    }

    G4MaterialPropertiesTable* mattab = mat->GetMaterialPropertiesTable();
    if (!mattab) {
        cerr << "Fail QE due to no material property table for " << mat->GetName() << endl;
        assert(mattab);
        return false;
    }
    
    G4MaterialPropertyVector* qevec = mattab->GetProperty("QE");
    if (!qevec) {
        cerr << "Fail QE due to not QE table in " << mat->GetName() << endl;
        assert(qevec);
        return false;
    }

    double energy = track->GetTotalEnergy();
    double qe = qevec->Value(energy);
    double live_or_die =  G4UniformRand();

    // extreme debugging
    if (false) {
        cerr << "energy=" << energy << ", qe=" << qe << ", rand=" << live_or_die;
        if (live_or_die < qe) {
            cerr << " LIVE!" << endl;
        }
        else {
            cerr << " DIE!" << endl;
        }
    }

    return live_or_die < qe;
}

G4bool Cowbells::SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* /*nada*/)
{
    G4Track* track = aStep->GetTrack();

    // Reject all non-opticalphotons
    if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
	return true;
    }
    
    G4StepPoint* psp = aStep->GetPreStepPoint();
    CLHEP::Hep3Vector pos = psp->GetPosition();
    G4TouchableHandle touch = psp->GetTouchableHandle();

    if (!pass_qe(track)) { return true; }

    int depth = touch->GetHistoryDepth();
    G4VPhysicalVolume* pv = touch->GetVolume();
    G4LogicalVolume* lv = pv->GetLogicalVolume();

    if (!fHC) {
        cerr  << "No hit collection for PV: " << pv->GetName() << endl;
        return true;
    }
    int hcid = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    Cowbells::Hit* hit = new Cowbells::Hit();
    fHC->insert(new Cowbells::GHit(hit));

    string tname = make_touchable_name(touch);
    if (false) {
        cerr << "Hit in #"<<hcid<<"("<<collectionName[0]<<"): " 
             << tname << " PV:" << pv->GetName() 
             << " trackid: " << track->GetTrackID()
             << " from: " << track->GetParentID()
             << " energy: " << track->GetTotalEnergy()
             << " gt=" << track->GetGlobalTime()
             << " lt=" << track->GetLocalTime()
             << " pt=" << track->GetProperTime()
             << " v=[" << pos.x() << "," << pos.y() << "," << pos.z() << "]"
             << endl;
    }

    int id = divine_touchable_id(tname);
    if (id<0) {
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
    hit->setTime(track->GetGlobalTime());
//    cerr << "Hit on: [" << hcid << ":" << id << "] at " 
//	 << setiosflags(ios::fixed) << setprecision(2) << hit->time() << " ns (" << tname << ")" <<  endl;
    hit->setPos(pos.x(),pos.y(),pos.z());
    hit->setVolId(id);
    hit->setHcId(hcid);

    Cowbells::TrackInformation* info 
	= dynamic_cast<Cowbells::TrackInformation*>(track->GetUserInformation());
    if (info) {
	hit->setPdgId(info->parent_pdg());
	hit->setpType(info->process_type());
	hit->setpSubType(info->process_subtype());
    }
    else {
	cerr << "No user info for track " << track->GetTrackID() << endl;
    }

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
