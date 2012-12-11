/**
 * \class TestPrimaryGeneratorAction
 *
 * \brief Ripped for N06
 *
 * bv@bnl.gov Tue Jul 17 10:45:15 2012
 *
 */


#ifndef TESTPRIMARYGENERATORACTION_H
#define TESTPRIMARYGENERATORACTION_H

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
//class ExN06PrimaryGeneratorMessenger;

namespace Cowbells {

class TestPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    TestPrimaryGeneratorAction();
   ~TestPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);

    void SetOptPhotonPolar();
    void SetOptPhotonPolar(G4double);

  private:
    G4ParticleGun* particleGun;
    //TestPrimaryGeneratorMessenger* gunMessenger;
};
}
#endif  // TESTPRIMARYGENERATORACTION_H
