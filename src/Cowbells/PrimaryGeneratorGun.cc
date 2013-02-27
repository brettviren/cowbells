#include "Cowbells/PrimaryGeneratorGun.h"
#include "Cowbells/PrimaryGeneratorUtil.h"

#include "Cowbells/JsonUtil.h"
#include "Cowbells/Json2G4.h"

#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>

#include <cassert>
#include <iostream>
using namespace std;

using Cowbells::get_int;
using Cowbells::get_str;
using Cowbells::get_num;
using Cowbells::get_ThreeVector;

Cowbells::PrimaryGeneratorGun::PrimaryGeneratorGun(Json::Value cfg)
    : G4VUserPrimaryGeneratorAction()
    , m_timer(new Cowbells::Timerator)
    , m_gun(0)
{
    this->load_gun(cfg);
}
Cowbells::PrimaryGeneratorGun::~PrimaryGeneratorGun()
{
}

void Cowbells::PrimaryGeneratorGun::load_gun(Json::Value cfg)
{
    if (m_gun) {
        cerr << "Warning: replacing gun" << endl;
        delete m_gun;
        m_gun = 0;
    }

    m_gun = new G4ParticleGun(get_int(cfg["count"],1));
    
    G4ParticleDefinition* particle = 0;
    if (!cfg["particle"].isNull()) {
        string name = cfg["particle"].asString();
        cout << "Gun with particle: " << name << endl;
        particle = G4ParticleTable::GetParticleTable()->FindParticle(name.c_str());
    }
    else if (!cfg["pdgcode"].isNull()) {
        int pdgcode = get_int(cfg["pdgcode"]);
        particle = G4ParticleTable::GetParticleTable()->FindParticle(pdgcode);
    }
    if (!particle) {
        cerr << "No particle specified.  Use a \"particle\" or \"pdgcode\" in the configuration." << endl;
        cerr << "Configured with:" << endl;
        cerr << cfg.toStyledString() << endl;
        assert (particle);
    }
    m_timer->set_distribution(get_str(cfg["timedist"], "exponential"));
    m_timer->set_period(get_num(cfg["period"], 1.0));
    m_timer->set_starting(get_num(cfg["starting"], 0.0));

    cout << "Using particle: \"" << particle->GetParticleName() << "\"" << endl;

    G4ThreeVector vertex = get_ThreeVector(cfg["vertex"]);
    G4ThreeVector direction = get_ThreeVector(cfg["direction"], G4ThreeVector(0,0,1));
    G4ThreeVector pol = get_ThreeVector(cfg["pol"]);
    double energy = get_num(cfg["energy"]);

    m_gun->SetParticlePosition(vertex);
    m_gun->SetParticleDefinition(particle);
    m_gun->SetParticleEnergy(energy);
    m_gun->SetParticleMomentumDirection(direction);
    m_gun->SetParticlePolarization(pol);

    cout << "Configured gun with: \n" << cfg.toStyledString() << endl;
}


 
void Cowbells::PrimaryGeneratorGun::GeneratePrimaries(G4Event* gevt)
{
    m_gun->SetParticleTime(m_timer->gen());
    m_gun->GeneratePrimaryVertex(gevt);
}

 
