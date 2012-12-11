
#ifndef PRIMARYGENERATORGUN_H
#define PRIMARYGENERATORGUN_H

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>

#include <json/json.h>

namespace Cowbells {
    class PrimaryGeneratorGun : public G4VUserPrimaryGeneratorAction {
    public:
        PrimaryGeneratorGun(Json::Value cfg);
        virtual ~PrimaryGeneratorGun();

        // Required interface
        void GeneratePrimaries(G4Event* gevt);

        void load_gun(Json::Value cfg);

    private:

        G4ParticleGun* m_gun;
    };
}

#endif  // PRIMARYGENERATORGUN_H
