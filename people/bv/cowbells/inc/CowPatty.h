/**
 * \class CowPatty
 *
 * \brief A cow stack
 *
 * An MC Stack copied from g4vmc's Ex03.
 *
 * bv@bnl.gov Tue May  1 11:33:32 2012
 *
 */


#ifndef COWPATTY_H
#define COWPATTY_H

#include <TVirtualMCStack.h>
#include <TParticle.h>
#include <TClonesArray.h>
#include <stack>

class CowPatty  : public TVirtualMCStack
{
public:
    CowPatty();
    CowPatty(int size);
    virtual ~CowPatty();

    void Print(Option_t* option) const;
    void Reset();
    TParticle* GetParticle(Int_t id) const;

    //
    // TVirtualMCStack interface methods
    //

    // Create a new particle and push into stack;
    // toBeDone   - 1 if particles should go to tracking, 0 otherwise
    // parent     - number of the parent track, -1 if track is primary
    // pdg        - PDG encoding
    // px, py, pz - particle momentum [GeV/c]
    // e          - total energy [GeV]
    // vx, vy, vz - position [cm]
    // tof        - time of flight [s]
    // polx, poly, polz - polarization
    // mech       - creator process VMC code
    // ntr        - track number (is filled by the stack
    // weight     - particle weight
    // is         - generation status code
    virtual void  PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
                            Double_t px, Double_t py, Double_t pz, Double_t e,
                            Double_t vx, Double_t vy, Double_t vz, Double_t tof,
                            Double_t polx, Double_t poly, Double_t polz,
                            TMCProcess mech, Int_t& ntr, Double_t weight,
                            Int_t is);

    // The stack has to provide two pop mechanisms:
    // The first pop mechanism required.
    // Pop all particles with toBeDone = 1, both primaries and seconadies
    virtual TParticle* PopNextTrack(Int_t& itrack);

    // The second pop mechanism required.
    // Pop only primary particles with toBeDone = 1, stacking of secondaries
    // is done by MC
    virtual TParticle* PopPrimaryForTracking(Int_t i);

    //
    // Set methods
    //

    // Set the current track number
    virtual void       SetCurrentTrack(Int_t trackNumber);

    //
    // Get methods
    //

    // Total number of tracks
    virtual Int_t      GetNtrack()    const;

    // Total number of primary tracks
    virtual Int_t      GetNprimary()  const;

    // Current track particle
    virtual TParticle* GetCurrentTrack() const;

    // Current track number
    virtual Int_t      GetCurrentTrackNumber() const;

    // Number of the parent of the current track
    virtual Int_t      GetCurrentParentTrackNumber() const;

private:

    std::stack<TParticle*>  fStack;       //!< The stack of particles (transient)
    TClonesArray*           fParticles;   ///< The array of particle (persistent)
    Int_t                   fCurrentTrack;///< The current track number
    Int_t                   fNPrimary;    ///< The number of primaries
    
    ClassDef(CowPatty,1) // Ex03MCStack

};

#endif  // COWPATTY_H
