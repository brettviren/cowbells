#include "Cowbells/strutil.h"

using namespace std;

vector<string> Cowbells::split(const string& pass, const string& delim)
{
    string str(pass);
    vector<string> ret;

    while (str.size()) {
        string::size_type at = str.find(delim);

        ret.push_back( str.substr(0, at) );

        if (at == string::npos) { 
            break; 
        }

        str = str.substr( at + delim.size() );
    }
    return ret;                      
}

bool Cowbells::in(const string& str, const string& thing)
{
    return str.find(thing) != string::npos;
}

G4ThreeVector Cowbells::str2threevector(const std::string& str)
{
    vector<string> xyz = split(str,",");
    return G4ThreeVector(atof(xyz[0].c_str()),atof(xyz[1].c_str()),atof(xyz[2].c_str()));
}
