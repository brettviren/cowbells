#include "Cowbells/BuildFromRoot.h"
#include "Cowbells/SensitiveDetector.h"

#include <RootGM/volumes/Factory.h>
#include <Geant4GM/volumes/Factory.h>

#include <TGeoManager.h>
#include <TKey.h>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TGraph.h>
#include <G4VPhysicalVolume.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4MaterialTable.hh>
#include <G4Material.hh>

// for registering sensitive detectors
#include <G4LogicalVolume.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SDManager.hh>

#include <vector>

#include <iostream>
using namespace std;

Cowbells::BuildFromRoot::BuildFromRoot(std::string filename)
  : DetConsBase(filename)
{
}

Cowbells::BuildFromRoot::~BuildFromRoot()
{
  cerr << "Destructing BuildFromRoot" << endl;
}

G4VPhysicalVolume* Cowbells::BuildFromRoot::ConstructGeometry()
{
    // propfile assumed to hold tgeo too
    TGeoManager* geo = TGeoManager::Import(m_prop_file.c_str());

    // Import geometry from Root to VGM
    RootGM::Factory rtFactory;
    rtFactory.SetDebug(0);
    rtFactory.Import(geo->GetTopNode());
    cerr << "Loaded ROOT geometry" << endl;

    // Export VGM geometry to Geant4
    Geant4GM::Factory g4Factory;
    g4Factory.SetDebug(0);
    rtFactory.Export(&g4Factory);
    G4VPhysicalVolume * world = g4Factory.World();
    cerr << "Converted to Geant4 geometry" << endl;

    return world;
}

