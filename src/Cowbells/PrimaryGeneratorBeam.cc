#include "Cowbells/PrimaryGeneratorBeam.h"
#include "Cowbells/PrimaryGeneratorUtil.h"
#include "Cowbells/strutil.h"
#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>

#include <cassert>
#include <string>
using namespace std;

Cowbells::PrimaryGeneratorBeam::PrimaryGeneratorBeam(const char* kindesc)
    : G4VUserPrimaryGeneratorAction()
    , m_timer(new Cowbells::Timerator)
    , m_gun(0)
{
    if (kindesc) this->SetKinDesc(kindesc);
}

Cowbells::PrimaryGeneratorBeam::~PrimaryGeneratorBeam()
{
}

/** Set the kinematics via a description holding key/value pairs
 * separated by the delimeter.  Keys include:
 *
 * pdgcode INT - set the particle type by PDG code
 * name STRING - set the particle by canonical name (pdgcode overrides)
 * vertex FLOAT,FLOAT,FLAOT - set the gun's vertex 
 * direction FLOAT,FLOAT,FLAOT - set the gun's direction 
 * energy FLOAT - set the particle's kinetic energy (in the s.o.u.: MeV)
 * pol FLOAT,FLOAT,FLOAT - set the particle's polarization
 * count INT - set the per-event particle multiplicity 
 * timedist STRING - set timerator (default exponential)
 * period FLOAT - set time period in seconds (default 1 second)
 * starting TIME - set starting time in seconds (default 0 seconds)
 */
void Cowbells::PrimaryGeneratorBeam::SetKinDesc(const char* kindesc)
{
    cout << "Setting kinematics to: \"" << kindesc << "\"" << endl;

    const string delim = "&";
    string name = "";

    G4ParticleDefinition* particle = 0;

    name = get_startswith_rest(kindesc,"name=",delim);
    if (name != "") {
        particle = G4ParticleTable::GetParticleTable()->FindParticle(name.c_str());
    }
    name = get_startswith_rest(kindesc,"pdgcode=",delim);
    if (name != "") {
        int pdgcode = atol(name.c_str());
        particle = G4ParticleTable::GetParticleTable()->FindParticle(pdgcode);
    }
    if (!particle) {
        cerr << "Failed to get particle named: \"" << name << "\"" << endl;
        assert (particle);
    }
    else {
        cout << "Using particle: \"" << particle->GetParticleName() << "\"" << endl;
    }
        
    int count = uri_integer(kindesc, "count", 1);

    G4ThreeVector vertex = uri_threevector(kindesc,"vertex");
    G4ThreeVector direction = uri_threevector(kindesc,"direction", G4ThreeVector(0,0,1));
    G4ThreeVector pol = uri_threevector(kindesc,"pol");
    double energy = uri_double(kindesc,"energy");

    m_timer->set_uri(kindesc);

    if (m_gun) delete m_gun;
    m_gun = new G4ParticleGun(count);
    m_gun->SetParticlePosition(vertex);
    m_gun->SetParticleDefinition(particle);
    m_gun->SetParticleEnergy(energy);
    m_gun->SetParticleMomentumDirection(direction);
    m_gun->SetParticlePolarization(pol);

    cout << "Beam generator with KinE=" << energy << ", " << count << " particles:" << particle->GetParticleName() << " vertex=" << vertex << " direction=" << direction << " polarization=" << pol << endl;
}


void Cowbells::PrimaryGeneratorBeam::GeneratePrimaries(G4Event* gevt)
{
    m_gun->SetParticleTime(m_timer->gen());
    m_gun->GeneratePrimaryVertex(gevt);
}

