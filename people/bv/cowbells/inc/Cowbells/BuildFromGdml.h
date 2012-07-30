/**
 * \class BuildFromGdml
 *
 * \brief A G4VUserDetectorContruction that uses GDML geometry to build G4 geometry.
 *
 *
 * bv@bnl.gov Sat May  5 14:24:10 2012
 *
 */



#ifndef COWBELLS_BUILDFROMGDML_H
#define COWBELLS_BUILDFROMGDML_H

#include "Cowbells/DetConsBase.h"

#include <G4GDMLParser.hh>

#include <string>
#include <map>

namespace Cowbells {

    class BuildFromGdml : public Cowbells::DetConsBase {
    public:

        /// Build Geometry, materials and optical properties
        BuildFromGdml(std::string gdml_file, std::string prop_file);
        virtual ~BuildFromGdml();

    protected:

	// Subclass builds geometry, returns world
	G4VPhysicalVolume* ConstructGeometry();

    private:

	// For GDML
	std::string m_gdml_file;
	G4GDMLParser m_gdml;
    };

}

#endif  // COWBELLS_BUILDFROMGDML_H
