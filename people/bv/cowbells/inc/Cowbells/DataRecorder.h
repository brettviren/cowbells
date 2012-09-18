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

namespace Cowbells {

    class DataRecorder
    {
    public:
        DataRecorder(const char* filename = 0);
        virtual ~DataRecorder();

        static DataRecorder* Get();

        // call once an event
        void fill(const G4Event* event);

        // call at end of run
        void close();

        // Add a step to the current event
        void add_step(Cowbells::Step* step);

    private:
        TFile* m_file;
        TTree* m_tree;
        Cowbells::Event* m_event;

        bool m_save_steps;

        void clear();
        void set_output_file(std::string filename);
    };
}
#endif  // DATARECORDER_H
