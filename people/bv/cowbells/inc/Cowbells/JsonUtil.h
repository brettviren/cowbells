#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <json/json.h>
#include <string>
#include <vector>

namespace Cowbells {

    float get_num(Json::Value val, float def=0.0);
    int get_int(Json::Value val, int def=0);
    std::string get_str(Json::Value val, std::string def="");

    Json::Value json_parse_file(std::string filename);

    Json::Value json_get_fitting(std::vector<Json::Value>& roots,
                                 std::string path);
}
#endif  // JSONUTIL_H
