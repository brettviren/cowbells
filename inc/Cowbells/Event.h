/**
 * \class Event
 *
 * \brief An event described
 *
 * An event starts as some initial kinematics.  It can then be
 * processed through the simulation to produce hits and other "truth"
 * info.
 *
 * bv@bnl.gov Thu May 10 13:08:31 2012
 *
 */



#ifndef COWBELLS_EVENT_H
#define COWBELLS_EVENT_H


#include "Cowbells/Hit.h"

#include <vector>

namespace Cowbells {

    class Vertex {
    public:
        double x,y,z,t;         // four-position
        Vertex();
    };

    class Particle{
    public:
        int vertex;             // vertex number (index into vertex vector)
        int trackid;            // G4 track ID
        int pdg;                // PDG particle code
        double ekin;            // kinetic energy
        double dx,dy,dz;        // direction from momentum vector
        double proptime;        // proper time
        Particle();
    };

    // Record some truth info about steps
    class Step {
    public:
        int trackid;          // g4 track id number
        int parentid;         // track ID number of parent;
        int proctype;         // // g4 process type that produced the particle taking the step
        int procsubtype;      // // g4 process sub type
        int pdgid;            // pdg particle id
        int mat1, mat2;       // material index before/after the step
        int stepnum;          // step number in track
        float energy1;        // kinetic energy before the step
        float energy2;        // kinetic energy after the step
        float dist;           // the distance stepped
        float dt;             // time spanned in the step
        float edep;           // total energy deposted during the step
        float enoni;          // non-ionization energy deposited
        // positions before/after the step
        float x1,y1,z1,t1,x2,y2,z2,t2;
        Step();
    };

    // Record some truth at staking time.  Only record per-particle
    // info for non-opticalphotons.
    class Stack {
    public:
        // initial values at stacking
        int trackid;          // g4 track id number
        int parentid;         // track ID number of parent;
        int pdgid;            // pdg particle id
        int mat;              // material number where the stack happened
        float energy;           // kinetic energy at stacking

        // accumulated values.
        int nscint;           // number of scintilating photons
        int nceren;           // number of cernkov photons
        Stack();
    };

    class Event {
    public:
        Event();
        ~Event();

        void clear();
        void clear_kine();
        void clear_hits();
        void clear_steps();
        void clear_stacks();

        std::vector<Cowbells::Vertex> vtx;
        std::vector<Cowbells::Particle> part;

        std::vector<Cowbells::Hit> hc;
        std::vector<Cowbells::Step> steps;
        std::vector<Cowbells::Stack> stacks;
    };


}

#endif  // COWBELLS_EVENT_H
