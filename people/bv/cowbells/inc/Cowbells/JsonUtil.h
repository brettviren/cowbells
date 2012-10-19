#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <json/json.h>
#include <string>
#include <vector>

namespace Cowbells {

    Json::Value json_parse_file(std::string filename);
    Json::Value json_get_keys(std::vector<Json::Value> roots, 
                              std::vector<std::string> keys);

}
#endif  // JSONUTIL_H
