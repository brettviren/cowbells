#include "Cowbells/DataRecorder.h"

#include "G4SDManager.hh"
#include "G4Event.hh"

#include <iostream>
using namespace std;

static Cowbells::DataRecorder* singleton = 0;

Cowbells::DataRecorder::DataRecorder(const char* filename, Json::Value cfg)
    : m_file(0)
    , m_tree(0)
    , m_event(0)
    , m_save_steps(false)
{
    if (!singleton) { singleton = this; } // first one wins
    if (filename) {
        this->set_output_file(filename);
    }
    this->apply_json_cfg(cfg);
}

void Cowbells::DataRecorder::apply_json_cfg(Json::Value cfg)
{
    int nsens = cfg.size();
    for (int ind=0; ind<nsens; ++ind) {
        Json::Value sens = cfg[ind];
        Json::Value name = sens["hcname"];
        if (name.isNull()) {
            cerr << "Failed to get \"hcname\" from: " << sens.toStyledString() << endl;
            continue;
        }

        this->add_hc(name.asString());
    }
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

void Cowbells::DataRecorder::set_output_file(std::string filename)
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


void Cowbells::DataRecorder::close()
{
    if (!m_file) return;
    cerr << "Closing \"" << m_file->GetName() << endl;
    m_file->cd();
    m_tree->Write();            // redundant?
    //m_file->Close();
}

void Cowbells::DataRecorder::add_hc(const std::string& hcname)
{
    m_hcnames.push_back(hcname);
}

void Cowbells::DataRecorder::fill(const G4Event* event)
{
    //cerr << "Filling tree" << endl;

    if (!m_hcnames.size() ) {
        cerr << "No hit collections requested for storage." << endl;
    }

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
            m_event->hc.push_back(ghit->get());
        }
        
        if (!m_save_steps) { 
            m_event->clear_steps();
        }
    }

    m_tree->Fill();
    m_event->clear();

    //cerr << "Filled tree with " << nhits << " hits" << endl;
}

void Cowbells::DataRecorder::add_step(Step* step)
{
    m_event->steps.push_back(step);
}
