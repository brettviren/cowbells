#include "CowPatty.h"
#include <iostream>

using namespace std;

CowPatty::CowPatty(int size)
    : fParticles(new TClonesArray("TParticle", size)),
      fCurrentTrack(-1),
      fNPrimary(0)
{
}

CowPatty::CowPatty()
    : fParticles(0),
      fCurrentTrack(-1),
      fNPrimary(0)
{
}

CowPatty::~CowPatty() 
{
    if (fParticles) fParticles->Delete();
    delete fParticles;
}

void CowPatty::Print(Option_t* /*option*/) const 
{
    cout << "moo: Stack info  " << endl;
    cout << "moo:\tTotal number of particles:   " <<  GetNtrack() << endl;
    cout << "moo:\tNumber of primary particles: " <<  GetNprimary() << endl;

    for (Int_t i=0; i<GetNtrack(); i++) {
        GetParticle(i)->Print();
    }
}

void CowPatty::Reset()
{
    fCurrentTrack = -1;
    fNPrimary = 0;
    fParticles->Clear();
}       

TParticle*  CowPatty::GetParticle(Int_t id) const
{
    if (id < 0 || id >= fParticles->GetEntriesFast()) {
        Fatal("GetParticle", "Index out of range"); 
    }
   
    return (TParticle*)fParticles->At(id);
}




//
// TVirtualMCStack interface imp below
//

void CowPatty::PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
                         Double_t px, Double_t py, Double_t pz, Double_t e,
                         Double_t vx, Double_t vy, Double_t vz, Double_t tof,
                         Double_t polx, Double_t poly, Double_t polz,
                         TMCProcess mech, Int_t& ntr, Double_t weight,
                         Int_t is)
{
    const Int_t kFirstDaughter=-1;
    const Int_t kLastDaughter=-1;
  
    TClonesArray& particlesRef = *fParticles;
    Int_t trackId = GetNtrack();
    TParticle* particle
        = new(particlesRef[trackId]) 
        TParticle(pdg, is, parent, trackId, kFirstDaughter, kLastDaughter,
                  px, py, pz, e, vx, vy, vz, tof);
   
    particle->SetPolarisation(polx, poly, polz);
    particle->SetWeight(weight);
    particle->SetUniqueID(mech);

    if (parent<0) fNPrimary++;  
    
    if (toBeDone) fStack.push(particle);
  
    ntr = GetNtrack() - 1;  
}

TParticle* CowPatty::PopNextTrack(Int_t& itrack)
{
    itrack = -1;
    if  (fStack.empty()) return 0;
		      
    TParticle* particle = fStack.top();
    fStack.pop();

    if (!particle) return 0;  
  
    fCurrentTrack = particle->GetSecondMother();
    itrack = fCurrentTrack;
  
    return particle;
}

TParticle* CowPatty::PopPrimaryForTracking(Int_t i)
{
    if (i < 0 || i >= fNPrimary)
        Fatal("GetPrimaryForTracking", "Index out of range"); 
  
    return (TParticle*)fParticles->At(i);
}


void CowPatty::SetCurrentTrack(Int_t trackNumber)
{
    fCurrentTrack = trackNumber;
}

Int_t CowPatty::GetNtrack() const
{
    return fParticles->GetEntriesFast();
}

Int_t CowPatty::GetNprimary()  const
{
    return fNPrimary;
}

TParticle* CowPatty::GetCurrentTrack() const
{
    TParticle* current = GetParticle(fCurrentTrack);
    
    if (!current) {
        Warning("GetCurrentTrack", "Current track not found in the stack");
    }
    return current;
}

Int_t CowPatty::GetCurrentTrackNumber() const
{
    return fCurrentTrack;
}

Int_t CowPatty::GetCurrentParentTrackNumber() const
{
    TParticle* current = GetCurrentTrack();

    if (current) {
        return current->GetFirstMother();
    }
    return -1;
}










ClassImp(CowPatty)
