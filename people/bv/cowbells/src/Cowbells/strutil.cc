#include "Cowbells/strutil.h"
#include <cassert>

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

/// Split and return the remaining part of the element that starts with the given string
std::string Cowbells::get_startswith(const std::string& str, 
                                     const std::string& startswith,
                                     const std::string& delim,
                                     const std::string& def)
{
    vector<string> parts = split(str,delim);
    for (size_t ind=0; ind<parts.size(); ++ind) {
        //cerr << "parts[" << ind << "]=" << parts[ind] << endl;
        if (startswith.size() > parts[ind].size()) { 
            //cerr << "bad size: " << startswith.size() << " > " << parts[ind].size() << endl;
            continue; 
        }
        if (startswith != parts[ind].substr(0,startswith.size())) { 
            //cerr << "bad match: " << startswith << " != " 
            //     << parts[ind].substr(0,startswith.size()) << endl;
            continue; 
        }
        return parts[ind].substr(startswith.size());
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
    string found = Cowbells::get_startswith(argstr, find, "&");
    if (!found.size()) return def;
    return str2threevector(found);
}

/// Return named URI query argument as an integer
int Cowbells::uri_integer(const std::string& argstr, const std::string& name, int def)
{
    string find = name + "=";
    string found = Cowbells::get_startswith(argstr, find, "&");
    if (!found.size()) { return def; }
    return atol(found.c_str());
}

/// Return named URI query argument as a double
double Cowbells::uri_double(const std::string& argstr, const std::string& name, double def)
{
    string find = name + "=";
    string found = Cowbells::get_startswith(argstr, find, "&");
    if (!found.size()) { return def; }
    return atof(found.c_str());
}
