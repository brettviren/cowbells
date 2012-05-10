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

#include <G4VUserDetectorConstruction.hh>

namespace Cowbells {

    class BuildFromRoot : public G4VUserDetectorConstruction {
    public:

        BuildFromRoot(const char* root_geom_filename);
        virtual ~BuildFromRoot();

        // G4VU.D.C. interface
        virtual G4VPhysicalVolume* Construct();
        
    private:
        const char* m_geomfilename;
    };

}

#endif  // COWBELLS_BUILDFROMROOT_H
