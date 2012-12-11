#include "Cowbells/Event.h"

#include <iostream>
using namespace std;

Cowbells::Step::Step()
    :  trackid(-1)
    , parentid(-1)
    , pdgid(-1)
    , mat1(-1)
    , mat2(-1)
    , energy1(0.0)
    , energy2(0.0)
    , dist(0.0)
{
}        


Cowbells::Event::Event(Cowbells::EventKinematics* /*kin*/)
//    : m_kine(0)
{
//    if (kin) set_kinematics(kin);
}
Cowbells::Event::~Event()
{
    cerr << "Deleting Event" << endl;
    // if (m_kine) {
    //     delete m_kine;
    //     m_kine = 0;
    // }
}

void Cowbells::Event::clear()
{
    hc.clear();
    this->clear_steps();
}
void Cowbells::Event::clear_steps()
{
    //cerr << "Deleting " << steps.size() << " steps" << endl;
    for (size_t ind=0; ind<steps.size(); ++ind) {
        delete steps[ind];
    }
    steps.clear();
}

// void Cowbells::Event::set_kinematics(Cowbells::EventKinematics* kin)
// {
//     if (m_kine) {
//         delete m_kine;
//     }
//     m_kine = kin;
// }
// const Cowbells::EventKinematics* Cowbells::Event::get_kinematics() const
// {
//     return m_kine;
// }

