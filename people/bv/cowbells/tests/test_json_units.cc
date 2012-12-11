/*

(master)bvlbne@lbne0002:run> g++ -g -o test_json_units -Wall ../cowbells/tests/test_json_units.cc -I/data3/lbne/bv/wbls/install/geant4/4.9.5.p01/include/Geant4 -I ../cowbells/inc ../cowbells/src/Cowbells/strutil.cc -L /data3/lbne/bv/wbls/install/geant4/4.9.5.p01/lib64 -lG4clhep  -L ../build/lib -lcowbells -ljson
(master)bvlbne@lbne0002:run> ./test_json_units ../cowbells/tests/units.json                                                                                                                                                                    Parsing file 1: ../cowbells/tests/units.json
0: value_in_cm = 4.2*cm = 42
1: value_in_mm = 42*mm = 42
2: value_in_ns = 69*ns = 69
3: value_in_sec = 69e-9*second = 69

*/


#include "Cowbells/JsonUtil.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{
    for (int ind = 1; ind < argc; ++ind) {
        cerr << "Parsing file " << ind << ": " << argv[ind] << endl;
        Json::Value val = Cowbells::json_parse_file(argv[ind]);

        Json::ValueIterator it = val.begin();
        int n = val.size();
        for (int ind=0; ind<n; ++ind, ++it) {
            cerr << ind << ": " << it.key().asString() << " = " 
                 << (*it).asString() << " = "
                 << Cowbells::get_num(*it) << endl;
        }
    }
    return 0;
}

