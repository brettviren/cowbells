#include "Cowbells/Event.h"

Cowbells::Event::Event(Cowbells::EventKinematics* kin)
    : m_kine(0)
{
    if (kin) set_kinematics(kin);
}
Cowbells::Event::~Event()
{
    if (m_kine) {
        delete m_kine;
        m_kine = 0;
    }
}

void Cowbells::Event::set_kinematics(Cowbells::EventKinematics* kin)
{
    if (m_kine) {
        delete m_kine;
    }
    m_kine = kin;
}
const Cowbells::EventKinematics* Cowbells::Event::get_kinematics() const
{
    return m_kine;
}

