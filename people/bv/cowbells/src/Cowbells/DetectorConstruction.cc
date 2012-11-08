#include "Cowbells/DetectorConstruction.h"

Cowbells::DetectorConstruction::DetectorConstruction(Cowbells::Json2G4& j2g4)
    : m_j2g4(j2g4)
{
}

Cowbells::DetectorConstruction::~DetectorConstruction()
{
}
    
G4VPhysicalVolume* Cowbells::DetectorConstruction::Construct()
{
    return m_j2g4.construct_detector();
}

