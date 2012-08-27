#ifndef PRIMARYGENERATORFILE_H
#define PRIMARYGENERATORFILE_H

#include "G4VUserPrimaryGeneratorAction.hh"
#include <G4ParticleGun.hh>

namespace Cowbells {

class PrimaryGeneratorFile  : public G4VUserPrimaryGeneratorAction
{
public:

    PrimaryGeneratorFile(const char* kindesc);
    
    virtual ~PrimaryGeneratorFile();

    virtual void SetKinDesc(const char* kindesc);

    // Required interface
    virtual void GeneratePrimaries(G4Event*);
};

}
#endif  // PRIMARYGENERATORFILE_H
