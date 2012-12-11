/**
   usage: test_hepmcio [output.hepmc [input.hepmc] ]

   will write to output file if given using input file.  If no input
   then event is generated.


   bvlbne@lbne0002:run> g++ -I ../../../../../install/hepmc/2.06.08/include/ \
                            -L ../../../../../install/hepmc/2.06.08/lib -lHepMC \
                           -o test_hepmcio ../cowbells/tests/test_hepmcio.cc

   bvlbne@lbne0002:run> ./test_hepmcio test.hepmc
   bvlbne@lbne0002:run> ./test_hepmcio test2.hepmc test.hepmc
   bvlbne@lbne0002:run> diff test2.hepmc test.hepmc
   bvlbne@lbne0002:run> cat test.hepmc

HepMC::Version 2.06.08
HepMC::IO_GenEvent-START_EVENT_LISTING
E 1 -1 -1.0000000000000000e+00 -1.0000000000000000e+00 -1.0000000000000000e+00 20 -3 4 0 0 0 0
U GEV MM
V -1 0 0 0 0 0 1 1 0
P 10001 2212 0 0 7.0000000000000000e+03 7.0000000000000000e+03 0 3 0 0 -1 0
P 10003 1 7.5000000000000000e-01 -1.5690000000000000e+00 3.2191000000000003e+01 3.2238000000000000e+01 6.2465990744549081e-02 3 0 0 -3 0
V -2 0 0 0 0 0 1 1 0
P 10002 2212 0 0 -7.0000000000000000e+03 7.0000000000000000e+03 0 3 0 0 -2 0
P 10004 -2 -3.0470000000000002e+00 -1.9000000000000000e+01 -5.4628999999999998e+01 5.7920000000000002e+01 3.3845236001575724e-01 3 0 0 -3 0
V -3 0 0 0 0 0 0 2 0
P 10005 22 -3.8130000000000002e+00 1.1300000000000000e-01 -1.8330000000000000e+00 4.2329999999999997e+00 8.1621075709617186e-02 1 0 0 0 0
P 10006 -24 1.5169999999999999e+00 -2.0680000000000000e+01 -2.0605000000000000e+01 8.5924999999999997e+01 8.0799603408680156e+01 3 0 0 -4 0
V -4 0 1.2000000000000000e-01 -2.9999999999999999e-01 5.0000000000000003e-02 4.0000000000000001e-03 0 2 0
P 10007 1 -2.4449999999999998e+00 2.8815999999999999e+01 6.0819999999999999e+00 2.9552000000000000e+01 -9.9503768772913739e-02 1 0 0 0 0
P 10008 -2 3.9620000000000002e+00 -4.9497999999999998e+01 -2.6687000000000001e+01 5.6372999999999998e+01 -1.7403447934355551e-01 1 0 0 0 0
HepMC::IO_GenEvent-END_EVENT_LISTING

 */

#include <HepMC/GenEvent.h>
#include <HepMC/IO_GenEvent.h>
#include <HepMC/SimpleVector.h>

#include <iostream>
#include <fstream>


using HepMC::GenEvent;
using HepMC::GenParticle;
using HepMC::GenVertex;
using HepMC::FourVector;

void write_event(const char* filename, GenEvent* evt)
{
    HepMC::IO_GenEvent out(filename, std::ios::out);
    out << evt;
}

void dump_event(GenEvent* evt)
{
    evt->print();
}

HepMC::GenEvent* read_event(const char* filename)
{
    HepMC::IO_GenEvent in(filename, std::ios::in);
    return in.read_next_event();
}

HepMC::GenEvent* gen_event()
{
    // taken from example_BuildEventFromScratch
    GenEvent* evt = new GenEvent( 20, 1 );
    // define the units
    evt->use_units(HepMC::Units::GEV, HepMC::Units::MM);
    //
    // create vertex 1 and vertex 2, together with their inparticles
    GenVertex* v1 = new GenVertex();
    evt->add_vertex( v1 );
    v1->add_particle_in( new GenParticle( FourVector(0,0,7000,7000),
                                          2212, 3 ) );
    GenVertex* v2 = new GenVertex();
    evt->add_vertex( v2 );
    v2->add_particle_in( new GenParticle( FourVector(0,0,-7000,7000),
                                          2212, 3 ) );
    //
    // create the outgoing particles of v1 and v2
    GenParticle* p3 =
        new GenParticle( FourVector(.750,-1.569,32.191,32.238), 1, 3 );
    v1->add_particle_out( p3 );
    GenParticle* p4 =
        new GenParticle( FourVector(-3.047,-19.,-54.629,57.920), -2, 3 );
    v2->add_particle_out( p4 );
    //
    // create v3
    GenVertex* v3 = new GenVertex();
    evt->add_vertex( v3 );
    v3->add_particle_in( p3 );
    v3->add_particle_in( p4 );
    v3->add_particle_out(
        new GenParticle( FourVector(-3.813,0.113,-1.833,4.233 ), 22, 1 )
        );
    GenParticle* p5 =
        new GenParticle( FourVector(1.517,-20.68,-20.605,85.925), -24,3);
    v3->add_particle_out( p5 );
    //
    // create v4
    GenVertex* v4 = new GenVertex(FourVector(0.12,-0.3,0.05,0.004));
    evt->add_vertex( v4 );
    v4->add_particle_in( p5 );
    v4->add_particle_out(
        new GenParticle( FourVector(-2.445,28.816,6.082,29.552), 1,1 )
        );
    v4->add_particle_out(
        new GenParticle( FourVector(3.962,-49.498,-26.687,56.373), -2,1 )
        );
    //
    // tell the event which vertex is the signal process vertex
    evt->set_signal_process_vertex( v3 );

    return evt;
}


int main(int argc, char *argv[])
{
    const char* output = 0;
    if (argc >= 2) {
        output = argv[1];
    }
    const char* input = 0;
    if (argc >= 3) {
        input = argv[2];
    }

    GenEvent* evt = 0;

    if (input) {
        evt = read_event(input);
    }
    else {
        evt = gen_event();
    }

    dump_event(evt);

    if (output) {
        write_event(output, evt);
    }
    
    return 0;
}
