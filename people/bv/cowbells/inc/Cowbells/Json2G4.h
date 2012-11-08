/**
 * \class Json2G4
 *
 * \brief Produce Geant4 objects based on content of files in JSON format.
 *
 * The schema of the files should be as produced by the
 * cowbells.geom.dumps_json() function.  For each module under
 * cowbells.geom.* there is a corresponding function here.
 *
 * bv@bnl.gov Wed Nov  7 14:16:42 2012
 *
 */



#ifndef JSON2G4_H
#define JSON2G4_H

#include <vector>
#include <string>
#include <json/json.h>

#include <G4VPhysicalVolume.hh>
#include <G4RotationMatrix.hh>

namespace Cowbells {

    G4LogicalVolume* get_LogicalVolume(Json::Value val, G4LogicalVolume* def = 0);
    G4ThreeVector get_ThreeVector(Json::Value val, G4ThreeVector def = G4ThreeVector());
    G4RotationMatrix* get_RotationMatrix(Json::Value val, G4RotationMatrix* def = 0);

    class Json2G4 {
    public:
        typedef std::vector<std::string> FileList;

        Json2G4(FileList files = FileList());
        ~Json2G4();

        void add_file(std::string file) { m_files.push_back(file); }

        /// Read in and parse all added JSON files
        void read();

        /// Return the value at the given path in the data structure
        Json::Value get(std::string path);

        // Load individual configuration sections of the same name
        int elements(Json::Value v);
        int materials(Json::Value v);
        int optical(Json::Value v);
        int volumes(Json::Value v);
        int placements(Json::Value v);
        int surfaces(Json::Value v);
        int sensitive(Json::Value v);

        /// Walk JSON data structure and make the G4 objects related
        /// to the detector.  This calls each of the above methods in
        /// turn.
        G4VPhysicalVolume* construct_detector();

    private:
        FileList m_files;
        std::vector<Json::Value> m_roots;

        G4VPhysicalVolume* m_world;
    };

}

#endif  // JSON2G4_H
