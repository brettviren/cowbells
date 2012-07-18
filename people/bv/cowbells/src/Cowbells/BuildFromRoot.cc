#include "Cowbells/BuildFromRoot.h"

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

    int debug_level = 0;        // 0=shut up, 1=info, 2=verbose
    return 0;
}
