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

namespace Cowbells {

    class Json2G4 {
    public:
        typedef std::vector<std::string> FileList;

        Json2G4(FileList files = FileList());
        ~Json2G4();

        void add_file(std::string file) { m_files.push_back(file); }

        void elements(Json::Value v);
        void materials(Json::Value v);
        void optical(Json::Value v);
        void volumes(Json::Value v);
        void placements(Json::Value v);
        void surfaces(Json::Value v);
        void sensitive(Json::Value v);

        /// Load all files
        void read();

        /// Make the G4 objects
        void make();

    private:
        FileList m_files;
        std::vector<Json::Value> m_roots;
    };

}

#endif  // JSON2G4_H
