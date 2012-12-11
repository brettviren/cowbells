#include "Cowbells/Hit.h"

#include <iostream>
using namespace std;

Cowbells::Hit::Hit()
    : t(0), x(0), y(0), z(0), e(0)
    , volid(0), pdg(0), hcid(0)

{
}

Cowbells::Hit::~Hit()
{
    //cerr << "Deleting hit @ (" << x << "," << y << "," << z << "), " << e << "MeV" << endl;
}

std::vector<double> Cowbells::Hit::pos()
{
    std::vector<double> ret;
    ret.push_back(x);
    ret.push_back(y);     
    ret.push_back(z); 
    return ret;
}
void Cowbells::Hit::setPos(double _x, double _y, double _z)
{
    x = _x;
    y = _y;
    z = _z;
}

double Cowbells::Hit::time() const
{
    return t;
}

void Cowbells::Hit::setTime(double _t)
{
    t = _t;
}


int Cowbells::Hit::volId()
{
    return volid;
}
void Cowbells::Hit::setVolId(int _volid)
{
    volid = _volid;
}

int Cowbells::Hit::pdgId()
{
    return pdg;
}
void Cowbells::Hit::setPdgId(int _pdg)
{
    pdg = _pdg;
}
int Cowbells::Hit::hcId()
{
    return hcid;
}
void Cowbells::Hit::setHcId(int id)
{
    hcid = id;
}


double Cowbells::Hit::energy()
{
    return e;
}
void Cowbells::Hit::setEnergy(double _e)
{
    e = _e;
}
