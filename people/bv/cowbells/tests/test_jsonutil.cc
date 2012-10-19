/**

   

 */

#include "Cowbells/JsonUtil.h"


#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{
    vector<Json::Value> roots;
    for (int ind = 1; ind < argc; ++ind) {
        cerr << "Parsing file " << ind << ": " << argv[ind] << endl;
        Json::Value val = Cowbells::json_parse_file(argv[ind]);
        roots.push_back(val);
    }


    const char* paths[] = {
        "elements/H",
        "elements/U",
        "detector/world/size",
        0
    };
    for (int ind=0; paths[ind]; ++ind) {
        cerr << ind << ": " << paths[ind] << " = ";
        Json::Value val = Cowbells::json_get_fitting(roots, paths[ind]);
        cerr << val.toStyledString() << endl;
    }


    
    return 0;
}
