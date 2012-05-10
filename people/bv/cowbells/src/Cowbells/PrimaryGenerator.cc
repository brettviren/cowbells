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
}

Cowbells::PrimaryGenerator::~PrimaryGenerator()
{
}


void Cowbells::PrimaryGenerator::GeneratePrimaries(G4Event* gevt)
{
    cerr << "Cowbells::PrimaryGenerator::GeneratePrimaries" << endl;

    // fixme: for now just make *something* to get the ball rolling
    G4ThreeVector zero;
    G4PrimaryVertex* origin = new G4PrimaryVertex(zero,0.0);
    int pdg = 22;               // gamma
    G4PrimaryParticle* particle = new G4PrimaryParticle(pdg);
    particle->SetMass(0.0);
    particle->SetMomentum(0.0,0.0,1.0*MeV);
    origin->SetPrimary(particle);
    gevt->AddPrimaryVertex(origin);
    return;
}

