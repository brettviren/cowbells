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


    {
        std::vector<std::string> keys;
        keys.push_back("elements");
        keys.push_back("H");
        Json::Value val = Cowbells::json_get_keys(roots, keys);
        cerr << val.toStyledString() << endl;
    }
    {
        std::vector<std::string> keys;
        keys.push_back("elements");
        keys.push_back("U");
        Json::Value val = Cowbells::json_get_keys(roots, keys);
        cerr << val.toStyledString() << endl;
    }


    
    return 0;
}
