/**
 * \class BuildFromJson
 *
 * \brief Detector construction driven by configuration files in JSON format.
 *
 * Configuration is provided by one or more files in JSON format.
 * 
 * bv@bnl.gov Fri Oct 19 10:31:29 2012
 *
 */

#ifndef BUILDFROMJSON_H
#define BUILDFROMJSON_H

#include <G4VUserDetectorConstruction.hh>
#include <G4VSensitiveDetector.hh>
#include <G4VPhysicalVolume.hh>

#include <json/json.h>

#include <string>
#include <vector>
#include <map>

namespace Cowbells {

    class BuildFromJson : public G4VUserDetectorConstruction {
    public:

        BuildFromJson();
        virtual ~BuildFromJson();

        // Add a JSON config file
        void addfile(std::string filename);

        // G4VU.D.C. interface
        virtual G4VPhysicalVolume* Construct();
        
    private:
        
        Json::Value gjov(std::string path);
        double asDistance(std::string path);
        std::string asString(std::string path);
        bool asBool(std::string path);

        void MakeElements(Json::Value eles);
        void MakeMaterials(Json::Value mats);
        G4VPhysicalVolume* MakeGeometry();
        G4LogicalVolume* MakeTubDet(std::string mat_sample_name,
                                    std::string mat_tub_name);

        std::vector<std::string> m_config_files;
        std::vector<Json::Value> m_roots;
    };
}
#endif  // BUILDFROMJSON_H
