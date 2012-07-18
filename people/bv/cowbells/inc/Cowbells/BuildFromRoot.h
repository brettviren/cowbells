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
#include <G4VSensitiveDetector.hh>

#include <string>
#include <map>

namespace Cowbells {

    class BuildFromRoot : public G4VUserDetectorConstruction {
    public:

        /// Build Geometry, materials and optical properties
        BuildFromRoot(std::string filename);
        virtual ~BuildFromRoot();

        /// Associate a sensitive detector and hit collection to a
        /// logical volume.  By default Cowbells::SensitiveDetector is
        /// used.  If the hit collection name is not given it will be
        /// formed by appending "HC" to the logical volume name.
        void add_sensdet(std::string logical_volume_name,
                         std::string hit_collection_name = "",
                         std::string sensitive_detector_class = "");

        // G4VU.D.C. interface

        virtual G4VPhysicalVolume* Construct();
        

    private:

        // Import the geometry from the given file holding a
        // TGeoManger that was previously exported.
        virtual G4VPhysicalVolume* ConstructGeometry();
        
        // Tack on material optical properties
        virtual void AddMaterialProperties();

        // Register any sensitive detectors
        virtual void RegisterSensDets();

    private:
        std::string m_filename;
        typedef std::map<std::string, G4VSensitiveDetector*> LVSDMap_t;
        LVSDMap_t m_lvsd;
    };

}

#endif  // COWBELLS_BUILDFROMROOT_H
