#include "Cowbells/DetectorConstruction.h"

#include "Cowbells/Util.h"      // for the dumps

Cowbells::DetectorConstruction::DetectorConstruction(Cowbells::Json2G4& j2g4)
    : m_j2g4(j2g4)
{
}

Cowbells::DetectorConstruction::~DetectorConstruction()
{
}
    
G4VPhysicalVolume* Cowbells::DetectorConstruction::Construct()
{
    G4VPhysicalVolume* world = m_j2g4.construct_detector();

    Cowbells::dump(world, 0);
    Cowbells::dump_lvs();
    Cowbells::dump_pvs();
    return world;
}

