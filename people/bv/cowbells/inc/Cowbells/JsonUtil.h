#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <json/json.h>
#include <string>
#include <vector>

namespace Cowbells {

    Json::Value json_parse_file(std::string filename);

    Json::Value json_get_fitting(std::vector<Json::Value>& roots,
                                 std::string path);
}
#endif  // JSONUTIL_H
