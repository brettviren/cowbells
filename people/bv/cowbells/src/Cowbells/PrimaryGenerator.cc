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

    static int count = 0;
    ++count;

    // fixme: for now just make *something* to get the ball rolling
    G4ThreeVector zero;
    G4PrimaryVertex* origin = new G4PrimaryVertex(zero,0.0);
    //int pdg = 22;               // gamma
    // int pdg = 11;               // electron
    int pdg = 2212;               // proton
    G4PrimaryParticle* particle = new G4PrimaryParticle(pdg);
    //particle->SetMass(0.0);
    G4cout << "Using primarty particle of mass " << particle->GetMass() << G4endl;
    double momz = 3000.0*MeV;
    if (count/2) momz *= -1.0;
    particle->SetMomentum(0.0,0.0,momz);
    origin->SetPrimary(particle);
    gevt->AddPrimaryVertex(origin);
    return;
}

