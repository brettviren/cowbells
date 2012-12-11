#ifndef WCSimWCHit_h
#define WCSimWCHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4ios.hh"
// for sort, find, count_if
#include <algorithm>
//for less_equal, bind2nd,...
#include <functional>

// compose2 is not part of the C++ standard
// use this kludgy technique to use it
#include <ext/functional>
using __gnu_cxx::identity_element;
using __gnu_cxx::unary_compose;
using __gnu_cxx::binary_compose;
using __gnu_cxx::compose1;
using __gnu_cxx::compose2;
using __gnu_cxx::identity;
using __gnu_cxx::select1st;
using __gnu_cxx::select2nd;
using __gnu_cxx::project1st;
using __gnu_cxx::project2nd;
using __gnu_cxx::constant_void_fun;
using __gnu_cxx::constant_unary_fun;
using __gnu_cxx::constant_binary_fun;
using __gnu_cxx::constant0;
using __gnu_cxx::constant1;
using __gnu_cxx::constant2;
using __gnu_cxx::subtractive_rng;
using __gnu_cxx::mem_fun1;
using __gnu_cxx::mem_fun1_ref;



class WCSimWCHit : public G4VHit
{
 public:
  
  WCSimWCHit();
  ~WCSimWCHit();
  WCSimWCHit(const WCSimWCHit&);
  const WCSimWCHit& operator=(const WCSimWCHit&);
  G4int operator==(const WCSimWCHit&) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  void Draw();
  void Print();
  
 public:
  
  void SetTubeID       (G4int tube)                 { tubeID = tube; };
  void SetTrackID      (G4int track)                { trackID = track; };
  void SetEdep         (G4double de)                { edep = de; };
  void SetPos          (G4ThreeVector xyz)          { pos = xyz; };
  
  void SetRot          (G4RotationMatrix rotMatrix) { rot = rotMatrix; };
  void SetLogicalVolume(G4LogicalVolume* logV)      { pLogV = logV;}
  void AddParentID     (G4int primParentID)
  { primaryParentID.push_back(primParentID); }

  // This is temporarily used for the drawing scale
  void SetMaxPe(G4int number = 0)  {maxPe   = number;};

  void AddPe(G4float hitTime, G4float hitTime_corr, G4float hitWL, G4float hitWLSflag, G4int qe_flag,
	     G4double lpos_x, G4double lpos_y, G4double lpos_z,
	     G4double ldir_x, G4double ldir_y, G4double ldir_z)  
  {
    // First increment the totalPe number
    totalPe++; 

    if (totalPe > maxPe) 
      maxPe = totalPe;

    time.push_back(hitTime);
    time_var.push_back(hitTime_corr);
    wave_length.push_back(hitWL);
    wls_flag.push_back(hitWLSflag);
    qeflag.push_back(qe_flag);
    local_pos_x.push_back(lpos_x);
    local_pos_y.push_back(lpos_y);
    local_pos_z.push_back(lpos_z);
    local_dir_x.push_back(ldir_x);
    local_dir_y.push_back(ldir_y);
    local_dir_z.push_back(ldir_z);
    
  }
 
  G4int         GetTubeID()     { return tubeID; };
  G4int         GetTrackID()    { return trackID; };
  G4ThreeVector GetPos()        { return pos; };


  
  

  G4int         GetTotalPe()    { return totalPe;};
  G4float       GetTime(int i)  { return time[i]+time_var[i];};
  G4float       GetRawTime(int i)  { return time[i];};
  G4float       GetWL(int i) {return wave_length[i];};
  G4float       GetWLSflag(int i) {return wls_flag[i];};

  G4double       GetLocalPosX(int i) {return local_pos_x[i];};
  G4double       GetLocalPosY(int i) {return local_pos_y[i];};
  G4double       GetLocalPosZ(int i) {return local_pos_z[i];};
  G4double       GetLocalDirX(int i) {return local_dir_x[i];};
  G4double       GetLocalDirY(int i) {return local_dir_y[i];};
  G4double       GetLocalDirZ(int i) {return local_dir_z[i];};

  G4int         GetQeflag(int i) {return qeflag[i];}
  G4int         GetParentID(int i) { return primaryParentID[i];};
  
  void SortHitTimes() {   sort(time.begin(),time.end()); }


  // low is the trigger time, up is trigger+950ns (end of event)
  G4float GetFirstHitTimeInGate(G4float low,G4float upevent)
  {
    G4float firsttime;
    std::vector<G4float>::iterator tfirst = time.begin();
    std::vector<G4float>::iterator tlast = time.end();
  
    std::vector<G4float>::iterator found = 
      std::find_if(tfirst,tlast,
		   compose2(std::logical_and<bool>(),
			    std::bind2nd(std::greater_equal<G4float>(),low),
			    std::bind2nd(std::less_equal<G4float>(),upevent)
			    )
		   );
    if ( found != tlast ) {
      firsttime = *found; // first hit time
    }
    else {
      firsttime = -10000.; //error code.
    }
    //G4cout << "firsthit time " << firsttime << "\n";
    return firsttime;
  }


  // pmtgate  and evgate are durations, ie not absolute times

  G4int GetPeInGate(double low, double pmtgate,double evgate) {
    // M Fechner; april 2005
    // assumes that time has already been sorted
    std::vector<G4float>::iterator tfirst = time.begin();
    std::vector<G4float>::iterator tlast = time.end();
    // select min time
    G4float mintime = (pmtgate < evgate) ? pmtgate : evgate;
    
    // return number of hits in the time window...
    
    G4int number = std::count_if(tfirst,tlast,
				 compose2(std::logical_and<bool>(),
					  std::bind2nd(std::greater_equal<G4float>(),low),
					  std::bind2nd(std::less_equal<G4float>(),mintime)
					  )
				 );
    
    totalPeInGate = number;
    //    G4cout << "numer = " <<  number <<"\n";
    return number;
  }


 private:
  
  G4int            tubeID;
  G4int            trackID;
  G4double         edep;
  G4ThreeVector    pos;
  G4RotationMatrix rot;
  G4LogicalVolume* pLogV;

 

  // This is temporarily used for the drawing scale
  // Since its static *every* WChit sees the same value for this.

  static G4int     maxPe;

  G4int                 totalPe;
  std::vector<G4float>  time;
  std::vector<G4float>  time_var;
  std::vector<G4float>  wave_length;
  std::vector<G4float>  wls_flag;
  
  std::vector<G4double>  local_pos_x;
  std::vector<G4double>  local_pos_y;
  std::vector<G4double>  local_pos_z;
  std::vector<G4double>  local_dir_x;
  std::vector<G4double>  local_dir_y;
  std::vector<G4double>  local_dir_z;

  std::vector<G4int>     qeflag;
  std::vector<G4int>    primaryParentID;
  G4int                 totalPeInGate;
};

typedef G4THitsCollection<WCSimWCHit> WCSimWCHitsCollection;

extern G4Allocator<WCSimWCHit> WCSimWCHitAllocator;

inline void* WCSimWCHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) WCSimWCHitAllocator.MallocSingle();
  return aHit;
}

inline void WCSimWCHit::operator delete(void *aHit)
{
  WCSimWCHitAllocator.FreeSingle((WCSimWCHit*) aHit);
}

#endif
