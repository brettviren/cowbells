/**
 * \class DetConsBase.h
 *
 * \brief A base G4VUserDetectorContruction 
 *
 *
 * bv@bnl.gov Sat May  5 14:24:10 2012
 *
 */



#ifndef COWBELLS_DETCONSBASE_H
#define COWBELLS_DETCONSBASE_H

#include <G4VUserDetectorConstruction.hh>
#include <G4VSensitiveDetector.hh>
#include <G4VPhysicalVolume.hh>

#include <string>
#include <vector>
#include <map>

namespace Cowbells {

    class DetConsBase : public G4VUserDetectorConstruction {
    public:

        
        DetConsBase(std::string properties_filename);
        virtual ~DetConsBase();

        /// Associate a sensitive detector and hit collection to a
        /// logical volume.  The touchables vector should contain an
        /// ordered list of touchable names and is used to translate
        /// name into an index.  If the vector is empty it is assumed
        /// to use the logical volume's physical volume's copy number.
	///
	/// By default a Cowbells::SensitiveDetector object is used.
        /// If the hit collection name is not given it will be formed
        /// by appending "HC" to the logical volume name.
        void add_sensdet(std::string logical_volume_name,
			 std::vector<std::string> touchables,
                         std::string hit_collection_name = "",
                         std::string sensitive_detector_class = "");

        // G4VU.D.C. interface - subclass should not implement
        virtual G4VPhysicalVolume* Construct();

    protected:

	// Subclass builds geometry, returns world
	virtual G4VPhysicalVolume* ConstructGeometry() = 0;

    private:


        // Register any sensitive detectors
        virtual void RegisterSensDets();

    protected:
        std::string m_prop_file;

    private:
        typedef std::map<std::string, G4VSensitiveDetector*> LVSDMap_t;
        LVSDMap_t m_lvsd;
    };

}

#endif  // COWBELLS_BUILDFROMROOT_H
