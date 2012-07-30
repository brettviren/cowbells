#include "Cowbells/BuildFromGdml.h"
#include "Cowbells/SensitiveDetector.h"

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

Cowbells::BuildFromGdml::BuildFromGdml(std::string gdml_file, std::string prop_file)
    : DetConsBase(prop_file)
    , m_gdml_file(gdml_file)
{
}

Cowbells::BuildFromGdml::~BuildFromGdml()
{
}


G4VPhysicalVolume* Cowbells::BuildFromGdml::ConstructGeometry()
{
    m_gdml.Read(m_gdml_file.c_str());
    return m_gdml.GetWorldVolume();
}

