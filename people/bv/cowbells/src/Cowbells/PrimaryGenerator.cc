#include "Cowbells/PrimaryGenerator.h"

#include "G4Types.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4Event.hh"

#include <iostream>
using namespace std;


Cowbells::PrimaryGenerator::PrimaryGenerator()
    : G4VUserPrimaryGeneratorAction()
    , m_kine(0)
{
    cerr << "Creating Cowbells::PrimaryGenerator" << endl;
}

Cowbells::PrimaryGenerator::~PrimaryGenerator()
{
    cerr << "Destructing Cowbells::PrimaryGenerator" << endl;
}


void Cowbells::PrimaryGenerator::GeneratePrimaries(G4Event* gevt)
{
    cerr << "Cowbells::PrimaryGenerator::GeneratePrimaries("<<(void*)this<<") with G4Event*(" << (void*)gevt << ")" << endl;

    // fixme: for now just make *something* to get the ball rolling
    G4ThreeVector zero;
    G4PrimaryVertex* origin = new G4PrimaryVertex(zero,0.0);
    //int pdg = 22;               // gamma
    int pdg = 11;               // electron
    G4PrimaryParticle* particle = new G4PrimaryParticle(pdg);
    particle->SetMass(0.0);
    particle->SetMomentum(0.0,0.0,10.0*MeV);
    origin->SetPrimary(particle);
    gevt->AddPrimaryVertex(origin);
    return;
}

