#include "Cowbells/strutil.h"
#include <cassert>
#include <algorithm>

using namespace std;

std::string Cowbells::lower(const std::string str)
{
    std::string ret;
    std::transform(str.begin(), str.end(), std::back_inserter(ret), ::tolower);
    return ret;
}


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

std::string Cowbells::get_startswith_rest(const std::string& str, 
                                          const std::string& startswith,
                                          const std::string& delim,
                                          const std::string& def)
{
    std::string ret = Cowbells::get_startswith(str,startswith,delim,def);
    if (ret == def) {
        return ret;
    }
    return ret.substr(startswith.size());
}

std::string Cowbells::get_startswith(const std::string& str, 
                                     const std::string& startswith,
                                     const std::string& delim,
                                     const std::string& def)
{
    vector<string> parts = split(str,delim);
    for (size_t ind=0; ind<parts.size(); ++ind) {
        if (startswith.size() > parts[ind].size()) { 
            continue; 
        }
        if (startswith != parts[ind].substr(0,startswith.size())) { 
            continue; 
        }
        return parts[ind];
    }
    return def;
}

bool Cowbells::in(const string& str, const string& thing)
{
    return str.find(thing) != string::npos;
}

G4ThreeVector Cowbells::str2threevector(const std::string& str)
{
    vector<string> xyz = split(str,",");
    assert (xyz.size() == 3);
    return G4ThreeVector(atof(xyz[0].c_str()),atof(xyz[1].c_str()),atof(xyz[2].c_str()));
}

/// Split scheme://path?query as vector of scheme,path,query
std::vector<std::string> Cowbells::uri_split(const std::string& uri)
{
    vector<string> ret;
    vector<string> scheme_rest = split(uri,"://");
    if (scheme_rest.size() != 2) {
        return ret;
    }
    ret.push_back(scheme_rest[0]);
    vector<string> path_rest = split(scheme_rest[1],"?");
    for (size_t ind=0; ind<path_rest.size(); ++ind) {
        ret.push_back(path_rest[ind]);
    }
    return ret;
}

/// Return named URI query argument as a three vector
G4ThreeVector Cowbells::uri_threevector(const std::string& argstr, const std::string& name, G4ThreeVector def)
{
    string find = name + "=";
    string found = Cowbells::get_startswith_rest(argstr, find, "&");
    if (!found.size()) return def;
    return str2threevector(found);
}

/// Return named URI query argument as an integer
int Cowbells::uri_integer(const std::string& argstr, const std::string& name, int def)
{
    string find = name + "=";
    string found = Cowbells::get_startswith_rest(argstr, find, "&");
    if (!found.size()) { return def; }
    return atol(found.c_str());
}

/// Return named URI query argument as a double
double Cowbells::uri_double(const std::string& argstr, const std::string& name, double def)
{
    string find = name + "=";
    string found = Cowbells::get_startswith_rest(argstr, find, "&");
    if (!found.size()) { return def; }
    return atof(found.c_str());
}
