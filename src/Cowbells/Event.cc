#include "Cowbells/Event.h"

#include <iostream>
using namespace std;

Cowbells::Vertex::Vertex()
    : x(0), y(0), z(0), t(0)
{}

Cowbells::Particle::Particle()
    : vertex(0), trackid(0), pdg(0) , ekin(0), dx(0), dy(0), dz(0), proptime(0)
{}

Cowbells::Step::Step()
    :  trackid(-1)
    , parentid(-1)
    , proctype(-1)
    , pdgid(-1)
    , mat1(-1)
    , mat2(-1)
    , stepnum(0)
    , energy1(0.0)
    , energy2(0.0)
    , dist(0.0)
    , x1(0), y1(0), z1(0)
    , x2(0), y2(0), z2(0)
{
}        

Cowbells::Stack::Stack()
    : trackid(-1)
    , parentid(-1)
    , pdgid(-1)
    , mat(-1)
    , energy(0.0)
    , nscint(0)
    , nceren(0)
{
}        


Cowbells::Event::Event()
{
}
Cowbells::Event::~Event()
{
    cerr << "Deleting Event" << endl;
}

void Cowbells::Event::clear()
{
    this->clear_kine();
    this->clear_hits();
    this->clear_steps();
    this->clear_stacks();
}
void Cowbells::Event::clear_kine()
{
    vtx.clear();
    part.clear();
}
void Cowbells::Event::clear_hits()
{
    hc.clear();
}
void Cowbells::Event::clear_steps()
{
    //cerr << "Deleting " << steps.size() << " steps" << endl;
    //for (size_t ind=0; ind<steps.size(); ++ind) {
    //    delete steps[ind];
    //}
    steps.clear();
}
void Cowbells::Event::clear_stacks()
{
    //cerr << "Deleting " << steps.size() << " steps" << endl;
    //for (size_t ind=0; ind<stacks.size(); ++ind) {
    //    delete stacks[ind];
    //}
    stacks.clear();
}

