/**
 * \class BuildByHand
 *
 * \brief A G4VUserDetectorContruction that hard codes the geometry building.
 *
 *
 * bv@bnl.gov Sat May  5 14:24:10 2012
 *
 */

#ifndef COWBELLS_BUILDBYHAND_H
#define COWBELLS_BUILDBYHAND_H

#include "Cowbells/DetConsBase.h"

#include <string>
#include <map>

class G4LogicalVolume;

namespace Cowbells {

    class BuildByHand : public DetConsBase {
    public:

        /// Build Geometry, materials and optical properties
        BuildByHand(std::string filename);
        virtual ~BuildByHand();

        // G4VU.D.C. interface
        virtual G4VPhysicalVolume* ConstructGeometry();

        
    private:
        void MakeMaterials();
        G4LogicalVolume* MakeTubDet(std::string mat_sample_name,
                                    std::string mat_tub_name);
    };

}

#endif  // COWBELLS_BUILDBYHAND_H
