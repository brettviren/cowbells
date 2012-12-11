#include "Cowbells/PrimaryGeneratorFile.h"

Cowbells::PrimaryGeneratorFile::PrimaryGeneratorFile(const char* kindesc)
    : G4VUserPrimaryGeneratorAction()
{
    if (kindesc) this->SetKinDesc(kindesc);
}

Cowbells::PrimaryGeneratorFile::~PrimaryGeneratorFile()
{
}

void Cowbells::PrimaryGeneratorFile::SetKinDesc(const char* kindesc)
{
}

void Cowbells::PrimaryGeneratorFile::GeneratePrimaries(G4Event* gevt)
{
}
