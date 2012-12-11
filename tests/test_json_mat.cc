/** test_json

Test out the JSON-CPP library (http://jsoncpp.cf.net) built into
libcowbells.

g++ -g -o test_json -Wall ../cowbells/tests/test_json.cc -I ../cowbells/inc -L ../build/lib -ljson

*/

#include "json/json.h"

#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;


void dump_elements(Json::Value elements)
{
    int nelements = elements.size();
    cerr << "Got " << nelements << " elements:" << endl;
    Json::ValueIterator it = elements.begin(); 
    for (int count = 0; count<nelements; ++count, ++it) {
        Json::Value symv = it.key();
        cerr << symv.asString() << ": " << (*it).toStyledString() << endl;
    }
}
void dump_materials(Json::Value materials)
{
    int nmats = materials.size();
    cerr << "Got " << nmats << " materials:" << endl;
    Json::ValueIterator it = materials.begin();
    for (int count = 0; count<nmats; ++count, ++it) {
        Json::Value mat = *it;
        string matname = it.key().asString();
        float density = mat["density"].asFloat();

        Json::Value elelist = mat["elements"];
        int neles = elelist.size();
        cerr << "Material: " << matname << " density="  << density
             << " with " << neles << " elements:" << endl;
        Json::ValueIterator eit = elelist.begin();
        for (int ind=0; ind<neles; ++ind, ++eit) {
            cerr << "\t" << eit.key().asString() << " ";
            Json::Value comp = *eit;
            if (comp.isInt()) {
                cerr << comp.asInt() << " atoms" << endl;
            }
            else {
                cerr << 100.*comp.asFloat() << " %" << endl;
            }
        }
        
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cerr << "usage: test_json somefile.json" << endl;
        return 1;
    }

    ifstream fstr(argv[1]);
    stringstream ss;
    ss << fstr.rdbuf();
    string data = ss.str();

    Json::Value root;
    Json::Reader reader;

    bool ok = reader.parse(data, root);
    if (!ok) {
        cerr << "Failed to read " << argv[1] << endl;
        cerr << reader.getFormattedErrorMessages() << endl;
        return 1;
    }

    dump_elements(root["elements"]);
    dump_materials(root["materials"]);

    return 0;
}
