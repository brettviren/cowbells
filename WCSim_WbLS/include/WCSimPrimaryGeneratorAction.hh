#ifndef WCSimPrimaryGeneratorAction_h
#define WCSimPrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

#include <fstream>

class WCSimDetectorConstruction;
class G4ParticleGun;
class G4Event;
class WCSimPrimaryGeneratorMessenger;

class WCSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  WCSimPrimaryGeneratorAction(WCSimDetectorConstruction*);
  ~WCSimPrimaryGeneratorAction();
  
public:
  void GeneratePrimaries(G4Event* anEvent);

 

  
  // Normal gun setting calls these functions to fill jhfNtuple and Root tree
  void SetVtx(G4ThreeVector i)     { vtx = i; };
  void SetBeamEnergy(G4double i)   { beamenergy = i; };
  void SetBeamDir(G4ThreeVector i) { beamdir = i; };
  void SetBeamPDG(G4int i)         { beampdg = i; };

  // These go with jhfNtuple
  G4int GetVecRecNumber(){return vecRecNumber;}
  G4int GetMode() {return mode;};
  G4int GetVtxVol() {return vtxvol;};
  G4ThreeVector GetVtx() {return vtx;}
  G4int GetNpar() {return npar;};
  G4int GetBeamPDG() {return beampdg;};
  G4double GetBeamEnergy() {return beamenergy;};
  G4ThreeVector GetBeamDir() {return beamdir;};
  G4int GetTargetPDG() {return targetpdg;};
  G4double GetTargetEnergy() {return targetenergy;};
  G4ThreeVector GetTargetDir() {return targetdir;};

  // older ...
  G4double GetNuEnergy() {return nuEnergy;};
  G4double GetEnergy() {return energy;};
  G4double GetXPos() {return xPos;};
  G4double GetYPos() {return yPos;};
  G4double GetZPos() {return zPos;};
  G4double GetXDir() {return xDir;};
  G4double GetYDir() {return yDir;};
  G4double GetZDir() {return zDir;};

private:
  WCSimDetectorConstruction*      myDetector;
  G4ParticleGun*                   particleGun;
  WCSimPrimaryGeneratorMessenger* messenger;

  // Variables set by the messenger
  G4bool   useMulineEvt;
  G4bool   useNormalEvt;
  
  std::fstream inputFile;
  G4String vectorFileName;
  G4bool   GenerateVertexInRock;

  // These go with jhfNtuple
  G4int mode;
  G4int vtxvol;
  G4ThreeVector vtx;
  G4int npar;
  G4int beampdg, targetpdg;
  G4ThreeVector beamdir, targetdir;
  G4double beamenergy, targetenergy;
  G4int vecRecNumber;

  G4double nuEnergy;
  G4double energy;
  G4double xPos, yPos, zPos;
  G4double xDir, yDir, zDir;

  G4int    _counterRock; 
  G4int    _counterCublic; 
public:

  inline void SetMulineEvtGenerator(G4bool choice) { useMulineEvt = choice; }
  inline G4bool IsUsingMulineEvtGenerator() { return useMulineEvt; }

  inline void SetNormalEvtGenerator(G4bool choice) { useNormalEvt = choice; }
  inline G4bool IsUsingNormalEvtGenerator()  { return useNormalEvt; }

  
 
  
  
  inline void OpenVectorFile(G4String fileName) 
  {
    if ( inputFile.is_open() ) 
      inputFile.close();

    vectorFileName = fileName;
    inputFile.open(vectorFileName, std::fstream::in);
  }
  inline G4bool IsGeneratingVertexInRock() { return GenerateVertexInRock; }
  inline void SetGenerateVertexInRock(G4bool choice) { GenerateVertexInRock = choice; }

};

#endif


