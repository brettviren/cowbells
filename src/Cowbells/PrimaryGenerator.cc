#include "Cowbells/PrimaryGenerator.h"
#include "Cowbells/strutil.h"

#include <cassert>
#include <iostream>
using namespace std;

Cowbells::PrimaryGenerator::PrimaryGenerator(const char* kindesc)
    : G4VUserPrimaryGeneratorAction()
{
    cerr << "Creating Cowbells::PrimaryGenerator with " << kindesc << endl;
    if (kindesc) {
        this->SetKinDesc(kindesc);
    }
}

Cowbells::PrimaryGenerator::~PrimaryGenerator()
{
    cerr << "Destructing Cowbells::PrimaryGenerator" << endl;
}


void Cowbells::PrimaryGenerator::SetKinDesc(const char* kindesc)
{
    bool implemented = false;
    assert(implemented);
}
void Cowbells::PrimaryGenerator::GeneratePrimaries(G4Event* gevt)
{
    bool implemented = false;
    assert(implemented);
}

