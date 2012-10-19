#include "Cowbells/JsonUtil.h"
#include "Cowbells/strutil.h"


#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
using namespace std;

Json::Value Cowbells::json_parse_file(std::string filename)
{
    ifstream fstr(filename.c_str());
    stringstream ss;
    ss << fstr.rdbuf();
    string data = ss.str();

    Json::Value root;
    Json::Reader reader;

    bool ok = reader.parse(data, root);
    if (!ok) {
        cerr << "Failed to read " << filename << endl;
        cerr << reader.getFormattedErrorMessages() << endl;
        throw invalid_argument("Failed to parse config file");
    }
    return root;
}
    
// Json::Value Cowbells::json_get_keys(vector<Json::Value> roots, vector<string> keys)
// {
//     for (size_t iroot=0; iroot < roots.size(); ++iroot) {
//         Json::Value root = roots[iroot];

//         bool failed = false;
//         for (size_t ikey = 0; ikey < keys.size(); ++ikey) {
//             Json::Value val = root[keys[ikey]];
//             if (val.isNull()) { 
//                 failed = true; 
//                 break; 
//             }
//             root = val;
//         }
//         if (failed || root.isNull()) { 
//             continue; 
//         }
//         return root;
//     }
//     return Json::Value();
// }

Json::Value Cowbells::json_get_fitting(std::vector<Json::Value>& roots,
                                       std::string path)
{
    vector<string> keys = Cowbells::split(path,"/");

    for (size_t iroot=0; iroot < roots.size(); ++iroot) {
        Json::Value root = roots[iroot];

        bool failed = false;
        for (size_t ikey = 0; ikey<keys.size(); ++ikey) {
            Json::Value val = root[keys[ikey]];
            if (val.isNull()) { 
                failed = true; 
                break; 
            }
            root = val;
        }
        if (failed || root.isNull()) { 
            continue; 
        }
        return root;
    }
    cerr << "Unknown configuration item: \"" << path << "\"" << endl;
    return Json::Value();
}
