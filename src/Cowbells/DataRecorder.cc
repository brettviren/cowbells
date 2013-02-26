#include "Cowbells/DataRecorder.h"

#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4OpticalPhoton.hh"

#include <iostream>
using namespace std;

static Cowbells::DataRecorder* singleton = 0;

Cowbells::DataRecorder::DataRecorder()
    : m_file(0)
    , m_tree(0)
    , m_event(0)
    , m_save_kine(false)
    , m_save_hits(false)
    , m_save_steps(false)
    , m_save_stacks(false)
{
    if (!singleton) { singleton = this; } // first one wins
}

void Cowbells::DataRecorder::set_module(std::string module, Json::Value cfg)
{
    if (module == "kine") {
        m_save_kine = cfg.asBool();
        cerr << "DataRecorder: Saving Kinematics" << endl;
        return;
    }
    if (module == "hits") {     // expect "sensitive" section of configuration
        int nsens = cfg.size();
        for (int ind=0; ind<nsens; ++ind) {
            Json::Value sens = cfg[ind];
            Json::Value name = sens["hcname"];
            if (name.isNull()) {
                cerr << "Failed to get \"hcname\" from: " << sens.toStyledString() << endl;
                continue;
            }
            m_hcnames.push_back(name.asString());
        }
        if (m_hcnames.size() > 0) {
            m_save_hits = true;
        }
        cerr << "DataRecorder: Saving Hits" << endl;
        return;
    }
    if (module == "steps") {    // expect True/False
        m_save_steps = cfg.asBool();
        cerr << "DataRecorder: Saving Steps" << endl;
        return;
    }
    if (module == "stacks") {   // expect True/False
        m_save_stacks = cfg.asBool();
        cerr << "DataRecorder: Saving Stacks" << endl;
        return;
    }

    cerr << "Unknown data recorder module: \"" << module << "\" configured with: " 
         << cfg.toStyledString() << endl;
    assert (0);
}

void Cowbells::DataRecorder::set_output(std::string filename)
{
    this->close();

    m_file = TFile::Open(filename.c_str(),"recreate");
    m_file->cd();
    m_tree = new TTree("cowbells","Cowbells Simulation Truth Tree");
    //m_event = new Cowbells::Event();
    TBranch* branch = m_tree->Branch("event","Cowbells::Event",&m_event);
    cerr << "Opened \"" << filename << "\" for writing with branch at 0x" << (void*)branch << endl;
    assert (branch);
}


Cowbells::DataRecorder::~DataRecorder()
{
    cerr << "Destructing DataRecorder" << endl;
    this->close();
    cerr << "DataRecorder done." << endl;
}

Cowbells::DataRecorder* Cowbells::DataRecorder::Get()
{
    if (!singleton) {
        singleton = new Cowbells::DataRecorder();
    }
    return singleton;
}


void Cowbells::DataRecorder::close()
{
    if (!m_file) return;
    cerr << "Closing \"" << m_file->GetName() << endl;
    m_file->cd();
    m_tree->Write();            // redundant?
    //m_file->Close();
}


void Cowbells::DataRecorder::add_event(const G4Event* event)
{
    //cerr << "Filling tree" << endl;

    if (m_save_kine) {
        int nprim = event->GetNumberOfPrimaryVertex();
        //cerr << "Saving kinematics from " << nprim << " vertices" << endl;
        for (int iprim=0; iprim<nprim; ++iprim) {
            const G4PrimaryVertex* pv = event->GetPrimaryVertex(iprim);
            Cowbells::Vertex cbv;
            cbv.x = pv->GetX0();
            cbv.y = pv->GetY0();
            cbv.z = pv->GetZ0();
            cbv.t = pv->GetT0();
            int ivtx = m_event->vtx.size();
            m_event->vtx.push_back(cbv);

            int nparts = pv->GetNumberOfParticle();
            for (int ipart=0; ipart<nparts; ++ipart) {
                const G4PrimaryParticle* part = pv->GetPrimary(ipart);
                Cowbells::Particle cbp;
                cbp.vertex = ivtx;
                cbp.trackid = part->GetTrackID();
                cbp.pdg = part->GetPDGcode();
                cbp.ekin = part->GetKineticEnergy();
                G4ThreeVector dir = part->GetMomentumDirection();
                cbp.dx = dir.x();
                cbp.dy = dir.y();
                cbp.dz = dir.z();
                cbp.proptime = part->GetProperTime();
                m_event->part.push_back(cbp);
            }
        }
    }

    if (!m_hcnames.size() ) {
        cerr << "No hit collections requested for storage." << endl;
    }

    int nhits_total = 0;
    for (size_t hcind = 0; hcind < m_hcnames.size(); ++hcind) {
        std::string hcName = m_hcnames[hcind];
        int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(hcName);
    
        G4HCofThisEvent* hcof = event->GetHCofThisEvent();
        if (!hcof) {
            cerr << "No hit collection of this event named " << hcName << endl;
            continue;
        }

        G4VHitsCollection* gen_hc = hcof->GetHC(hcID);

        Cowbells::HitCollection* hc = 
            static_cast<Cowbells::HitCollection*>(gen_hc);

        if (!hc) {
            cerr << "No hit collection from HC ID:" << hcID << " \"" << hcName << "\"" << endl;
            return;
        }

        int nhits = hc->entries();
        for (int ind = 0; ind < nhits; ++ind) {
            GHit* ghit = dynamic_cast<Cowbells::GHit*>(hc->GetHit(ind));
            assert(ghit);
            m_event->hc.push_back(*ghit->get());
        }
        nhits_total += nhits;
    }

    m_tree->Fill();
    m_event->clear();
    m_track2stack_index.clear();

    if (true) {
        cerr << "Filled tree with " << nhits_total << " hits in "
             << m_hcnames.size() << " collections"
             << endl;
    }
}

#include <G4Step.hh>
#include <G4StepPoint.hh>
#include <G4ParticleDefinition.hh>
#include <G4VProcess.hh>

static int get_mat_index(G4VPhysicalVolume* pv)
{
    if (!pv) return -2;
    G4LogicalVolume* lv = pv->GetLogicalVolume();
    if (!lv) return -3;
    G4Material* mat = lv->GetMaterial();
    if (!mat) return -4;
    return mat->GetIndex();
}

int pdgid_optical_photon = 20;  // wedge in just before gluon and gamma
int get_pdgid(const G4Track* track) 
{

    if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
        return pdgid_optical_photon;
    }
    G4ParticleDefinition* particle = track->GetDefinition();
    return particle->GetPDGEncoding();
}

void Cowbells::DataRecorder::add_stack(const G4Track* track)
{
    if (!m_save_stacks) { 
        return;
    }
    int track_id = track->GetTrackID();

    // non-optical
    if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {

        TrackStackMap_t::iterator it = m_track2stack_index.find(track_id);
        if (it != m_track2stack_index.end()) { // got it already
            return;
        }
        Cowbells::Stack cb_stack;
        cb_stack.trackid = track_id;
        cb_stack.parentid = track->GetParentID();
        G4ParticleDefinition* particle = track->GetDefinition();
        cb_stack.pdgid = particle->GetPDGEncoding();

        cb_stack.mat = -1;
        cb_stack.energy = track->GetKineticEnergy();

        m_track2stack_index[track_id] = m_event->stacks.size();
        m_event->stacks.push_back(cb_stack);
        return;
    }

    // I'm an opticalphoton

    const G4VProcess* proc = track->GetCreatorProcess();
    if (!proc) {
        return;
    }

    int parent_id = track->GetParentID();
    TrackStackMap_t::iterator it = m_track2stack_index.find(parent_id);
    assert(it != m_track2stack_index.end());
    Cowbells::Stack& cb_stack = m_event->stacks[it->second];

    if (proc->GetProcessName() == "Cerenkov") {
        cb_stack.nceren += 1;
    }
    if (proc->GetProcessName() == "Scintillation") {
        cb_stack.nscint += 1;
    }
}

void Cowbells::DataRecorder::add_step(const G4Step* step)
{
    if (!m_save_steps) { 
        return;
    }

    G4Track* track = step->GetTrack();
    G4StepPoint* prepoint = step->GetPreStepPoint();
    G4StepPoint* pstpoint= step->GetPostStepPoint();

    //G4ParticleDefinition* particle = track->GetDefinition();

    G4VPhysicalVolume* prephy = prepoint->GetPhysicalVolume();
    G4VPhysicalVolume* pstphy = pstpoint->GetPhysicalVolume();
    
    // string prename = "NONE";
    // if (prephy) prename = prephy->GetName();
    // string pstname = "NONE";
    // if (pstphy) pstname = pstphy->GetName();

    Cowbells::Step cb_step;

    cb_step.trackid = track->GetTrackID();
    cb_step.parentid = track->GetParentID();
    const G4VProcess* proc = track->GetCreatorProcess();
    if (proc) {
        cb_step.proctype = proc->GetProcessType();
    }

    cb_step.stepnum = track->GetCurrentStepNumber();

    cb_step.pdgid = get_pdgid(track);
    cb_step.mat1 = get_mat_index(prephy);
    cb_step.mat2 = get_mat_index(pstphy);
    cb_step.energy1 = prepoint->GetKineticEnergy();
    cb_step.energy2 = pstpoint->GetKineticEnergy();

    cb_step.edep = step->GetTotalEnergyDeposit();
    cb_step.enoni = step->GetNonIonizingEnergyDeposit();
    cb_step.dist = step->GetStepLength();
    cb_step.dt = step->GetDeltaTime();

    G4ThreeVector r1 = prepoint->GetPosition();
    G4ThreeVector r2 = pstpoint->GetPosition();
    cb_step.x1 = r1.x();
    cb_step.y1 = r1.y();
    cb_step.z1 = r1.z();
    cb_step.x2 = r2.x();
    cb_step.y2 = r2.y();
    cb_step.z2 = r2.z();


    // patch up what could not be collected at stacking time:
    if (cb_step.stepnum == 1) {
        if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {

            TrackStackMap_t::iterator it = m_track2stack_index.find(cb_step.trackid);
            if (it == m_track2stack_index.end()) {
                cerr << "Stepping track #"<< cb_step.trackid
                     << " before it's been stacked?" << endl;
            }
            else {
                Cowbells::Stack& cb_stack = m_event->stacks[it->second];
                cb_stack.mat = cb_step.mat1;
            }
        }
    }

    m_event->steps.push_back(cb_step);
}
