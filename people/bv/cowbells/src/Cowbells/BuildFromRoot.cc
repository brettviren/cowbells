#include "Cowbells/BuildFromRoot.h"

#include <RootGM/volumes/Factory.h>
#include <Geant4GM/volumes/Factory.h>

#include <TGeoManager.h>
#include <G4VPhysicalVolume.hh>

Cowbells::BuildFromRoot::BuildFromRoot(const char* root_geom_filename)
 : m_geomfilename(root_geom_filename)
{
}

Cowbells::BuildFromRoot::~BuildFromRoot()
{
}


G4VPhysicalVolume* Cowbells::BuildFromRoot::Construct()
{
    TGeoManager* geo = TGeoManager::Import(m_geomfilename);

    // Import geometry from Root to VGM
    RootGM::Factory rtFactory;
    rtFactory.SetDebug(1);
    rtFactory.Import(geo->GetTopNode());

    // Export VGM geometry to Geant4
    Geant4GM::Factory g4Factory;
    g4Factory.SetDebug(1);
    rtFactory.Export(&g4Factory);
    return g4Factory.World();
}
