/**
 * \class TestDetectorConstruction
 *
 * \brief Build a simple test geometry and materials
 *
 *
 * bv@bnl.gov Mon Jul 16 13:34:56 2012
 *
 */


#ifndef TESTDETECTORCONSTRUCTION_H
#define TESTDETECTORCONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>
#include <G4VSensitiveDetector.hh>

#include <string>
#include <map>

namespace Cowbells {

    class TestDetectorConstruction : public G4VUserDetectorConstruction {
    public:

        TestDetectorConstruction();
        virtual ~TestDetectorConstruction();

        // G4VU.D.C. interface
        virtual G4VPhysicalVolume* Construct();
        
        void add_sensdet(std::string logical_volume_name,
                         std::string hit_collection_name = "",
                         std::string sensitive_detector_class = "");

    private:

        // Register any sensitive detectors
        virtual void RegisterSensDets();

        typedef std::map<std::string, G4VSensitiveDetector*> LVSDMap_t;
        LVSDMap_t m_lvsd;
    };

}



#endif  // TESTDETECTORCONSTRUCTION_H
