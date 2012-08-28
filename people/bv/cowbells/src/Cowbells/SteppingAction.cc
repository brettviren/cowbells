#include "Cowbells/SteppingAction.h"

#include <G4Step.hh>
#include <G4StepPoint.hh>
#include <G4ParticleDefinition.hh>

#include <string>
using namespace std;

Cowbells::SteppingAction::SteppingAction()
    : G4UserSteppingAction()
    , m_dr(0)
{
}

Cowbells::SteppingAction::~SteppingAction()
{
}
        

static int get_mat_index(G4VPhysicalVolume* pv)
{
    if (!pv) return -1;
    G4LogicalVolume* lv = pv->GetLogicalVolume();
    if (!lv) return -1;
    G4Material* mat = lv->GetMaterial();
    if (!mat) return -1;
    return mat->GetIndex();
}

void Cowbells::SteppingAction::UserSteppingAction(const G4Step* step)
{
    assert (m_dr);

    G4Track* track = step->GetTrack();
    G4StepPoint* prepoint = step->GetPreStepPoint();
    G4StepPoint* pstpoint= step->GetPostStepPoint();

    G4ParticleDefinition* particle = track->GetDefinition();

    G4VPhysicalVolume* prephy = prepoint->GetPhysicalVolume();
    G4VPhysicalVolume* pstphy = pstpoint->GetPhysicalVolume();
    
    string prename = "NONE";
    if (prephy) prename = prephy->GetName();
    string pstname = "NONE";
    if (pstphy) pstname = pstphy->GetName();


    Cowbells::Step* cb_step = new Cowbells::Step();
    m_dr->add_step(cb_step);
    cb_step->trackid = track->GetTrackID();
    cb_step->parentid = track->GetParentID();
    cb_step->pdgid = particle->GetPDGEncoding();
    cb_step->mat1 = get_mat_index(prephy);
    cb_step->mat2 = get_mat_index(pstphy);
    cb_step->energy1 = prepoint->GetKineticEnergy();
    cb_step->energy2 = pstpoint->GetKineticEnergy();

    G4ThreeVector r1 = prepoint->GetPosition();
    G4ThreeVector r2 = pstpoint->GetPosition();
    G4ThreeVector diff = r2-r1;
    cb_step->dist = diff.mag();
    cb_step->x1 = r1.x();
    cb_step->y1 = r1.y();
    cb_step->z1 = r1.z();
    cb_step->x2 = r2.x();
    cb_step->y2 = r2.y();
    cb_step->z2 = r2.z();
}
