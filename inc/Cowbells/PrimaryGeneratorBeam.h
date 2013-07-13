/**
 * \class PrimaryGeneratorBeam
 *
 * \brief A primary generator in the form of a beam
 *
 * Produce a beam of particles.
 *
 *
 *
 * bv@bnl.gov Mon Aug 27 13:03:49 2012
 *
 */



#ifndef PRIMARYGENERATORBEAM_H
#define PRIMARYGENERATORBEAM_H


#include <G4VUserPrimaryGeneratorAction.hh>

#include <G4ParticleGun.hh>

namespace Cowbells {

class Timerator;

class PrimaryGeneratorBeam  : public G4VUserPrimaryGeneratorAction
{
public:

    PrimaryGeneratorBeam(const char* kindesc);
    
    virtual ~PrimaryGeneratorBeam();

    /** Set the kinematics via a description holding key/value pairs
     * separated by the delimeter.  Keys include:
     *
     * pdgcode INT - set the particle type by PDG code
     * name STRING - set the particle by canonical name (pdgcode overrides)
     * vertex FLOAT,FLOAT,FLOAT - set the gun's vertex 
     * direction FLOAT,FLOAT,FLOAT - set the gun's direction 
     * energy FLOAT - set the particle's kinetic energy (in the s.o.u.: MeV)
     * pol FLOAT,FLOAT,FLOAT - set the particle's polarization
     * count INT - set the per-event particle multiplicity 
     * spread FLOAT - set the angular spread of the beam
     */
    virtual void SetKinDesc(const char* kindesc) {
	m_kindesc = kindesc;
    }


    // Required interface
    void GeneratePrimaries(G4Event*);

private:
    void ApplyKinDesc();

    Cowbells::Timerator* m_timer;
    G4ParticleGun* m_gun;
    int m_count;

    std::string m_kindesc;

};

}

#endif  // PRIMARYGENERATORBEAM_H
