#include "Cowbells/PrimaryGenerator.h"
#include "Cowbells/strutil.h"

#include "G4Types.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4Event.hh"

#include <iostream>
#include <cassert>
using namespace std;

using Cowbells::split;

class Gun : public Cowbells::Generatelet 
{
    int m_pdg;
    G4ThreeVector m_vertex;
    G4ThreeVector m_momentum;

public:
    Gun(int pdg, G4ThreeVector vertex, G4ThreeVector momentum)
        : m_pdg(pdg)
        , m_vertex(vertex)
        , m_momentum(momentum)
        {    }

    void generate(G4Event* gevt) {
        G4PrimaryParticle* particle = new G4PrimaryParticle(m_pdg);
        particle->SetMomentum(m_momentum.x(),m_momentum.y(),m_momentum.z());
        G4PrimaryVertex* vertex = new G4PrimaryVertex(m_vertex.x(),m_vertex.y(),m_vertex.z(),0.0);
        vertex->SetPrimary(particle);
        gevt->AddPrimaryVertex(vertex);
    }
};


Cowbells::PrimaryGenerator::PrimaryGenerator(const char* kindesc)
    : G4VUserPrimaryGeneratorAction()
    , m_gen(0)
{
    assert(kindesc);
    cerr << "Creating Cowbells::PrimaryGenerator with " << kindesc << endl;
    this->SetKinDesc(kindesc);
}

Cowbells::PrimaryGenerator::~PrimaryGenerator()
{
    cerr << "Destructing Cowbells::PrimaryGenerator" << endl;
}


void Cowbells::PrimaryGenerator::SetKinDesc(const char* kindesc)
{
    if (m_gen) { 
        delete m_gen;
        m_gen = 0;
    }

    vector<string> scheme_args = split(kindesc,"://");
    string scheme = scheme_args[0];
    vector<string> args = split(scheme_args[1],"/");

    // if (scheme == "file") {
    //     m_gen = new File(schema_args[1]);
    // }
    if (scheme == "gun") {
        m_gen = new Gun(atol(args[0].c_str()),
                        str2threevector(args[1]),
                        str2threevector(args[2]));
    }
    // if (scheme == "ball") {
    //     vector<string> vtxstr = split(args[1],",");
    //     m_gen = new Ball(atol(args[0].c_str()),
    //                      G4ThreeVector(atof(vtxstr[0]),atof(vtxstr[1]),atof(vtxstr[2])),
    //                      atof(args[2]));
    // }

    if (!m_gen) {
        cerr << "Failed to parse \"" << kindesc << "\"" << endl;
        assert(m_gen);
    }
}

void Cowbells::PrimaryGenerator::GeneratePrimaries(G4Event* gevt)
{
    m_gen->generate(gevt);
    return;
}

