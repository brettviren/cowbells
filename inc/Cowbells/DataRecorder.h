/**
 * \class DataRecorder
 *
 * \brief Record results from simulation running.
 *
 * This is given to and used by the actions to save simulation
 * intermediate truth.
 *
 * bv@bnl.gov Tue Aug  7 09:07:52 2012
 *
 */



#ifndef DATARECORDER_H
#define DATARECORDER_H

#include "Cowbells/Event.h"
#include "G4Event.hh"
#include "G4Track.hh"

#include <TFile.h>
#include <TTree.h>

#include <json/json.h>

#include <vector>
#include <string>

namespace Cowbells {

    class DataRecorder
    {
    public:
        DataRecorder();
        virtual ~DataRecorder();

        static DataRecorder* Get();

        void set_output(std::string output_filename);
        void set_module(std::string module, Json::Value cfg = Json::Value());

        // call once an event
        void add_event(const G4Event* event);
        // call once per step
        void add_step(const G4Step* step);
        // call once per StackingAction::ClassifyNewStep()
        void add_stack(const G4Track* track);

        // call at end of run
        void close();

    private:
        TFile* m_file;
        TTree* m_tree;
        Cowbells::Event* m_event;

        std::vector<std::string> m_hcnames;

        bool m_save_hits;
        bool m_save_steps;
        bool m_save_stacks;

        typedef std::map<int, int> TrackStackMap_t;
        TrackStackMap_t m_track2stack_index;

        void clear();
    };
}
#endif  // DATARECORDER_H
