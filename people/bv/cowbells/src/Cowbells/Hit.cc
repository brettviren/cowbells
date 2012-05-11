#include "Cowbells/Hit.h"

Cowbells::Hit::Hit()
{
}

Cowbells::Hit::~Hit()
{
}

std::vector<double> Cowbells::Hit::pos()
{
    std::vector<double> ret;
    ret.push_back(m_x);
    ret.push_back(m_y);     
    ret.push_back(m_z); 
    return ret;
}
void Cowbells::Hit::setPos(double x, double y, double z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}
