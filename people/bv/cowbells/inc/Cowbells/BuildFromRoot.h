/**
 * \class BuildFromRoot
 *
 * \brief A G4VUserDetectorContruction that uses ROOT geometry to build G4 geometry.
 *
 *
 * bv@bnl.gov Sat May  5 14:24:10 2012
 *
 */



#ifndef COWBELLS_BUILDFROMROOT_H
#define COWBELLS_BUILDFROMROOT_H

#include "Cowbells/DetConsBase.h"

#include <string>
#include <map>

namespace Cowbells {

    class BuildFromRoot : public Cowbells::DetConsBase {
    public:

        /// Build Geometry, materials and optical properties
        BuildFromRoot(std::string filename);
        virtual ~BuildFromRoot();

    protected:

        // Import the geometry from the given file holding a
        // TGeoManger that was previously exported.
        virtual G4VPhysicalVolume* ConstructGeometry();
        
    };

}

#endif  // COWBELLS_BUILDFROMROOT_H
