/**
 * \class DataRecorder
 *
 * \brief Record results from simulation running.
 *
 * This is given to the run and event actions to save simulation
 * results to output.
 *
 * bv@bnl.gov Tue Aug  7 09:07:52 2012
 *
 */



#ifndef DATARECORDER_H
#define DATARECORDER_H

#include "Cowbells/Event.h"
#include "G4Event.hh"

#include <TFile.h>
#include <TTree.h>

#include <json/json.h>

#include <vector>
#include <string>

namespace Cowbells {

    class DataRecorder
    {
    public:
        DataRecorder(const char* filename=0, Json::Value cfg = Json::Value());
        virtual ~DataRecorder();

        void apply_json_cfg(Json::Value cfg);

        // Tell data to record hit collection 
        void add_hc(const std::string& hcname);

        static DataRecorder* Get();

        // call once an event
        void fill(const G4Event* event);

        // call at end of run
        void close();

        // Add a step to the current event
        void add_step(Cowbells::Step* step);

        // Set true to actually save steps to output file.  If not
        // called, default is false
        void save_steps(bool save = true) { m_save_steps = save; }

    private:
        TFile* m_file;
        TTree* m_tree;
        Cowbells::Event* m_event;
        bool m_save_steps;
        std::vector<std::string> m_hcnames;

        void clear();
        void set_output_file(std::string filename);
    };
}
#endif  // DATARECORDER_H
