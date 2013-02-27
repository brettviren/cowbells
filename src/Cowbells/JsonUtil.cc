#include "Cowbells/JsonUtil.h"
#include "Cowbells/strutil.h"

#include "CLHEP/Units/SystemOfUnits.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
using namespace std;

#include "TROOT.h"
#include "TGlobal.h"

void Cowbells::init_units()
{
    static bool been_here = false;
    if (been_here) { return ; }
    been_here = true;

    struct Units {
        const char* name;
        const char* value;
    };

    Units units[] = { 
        // length
        {"mm","1.0"},  {"cm","mm*10"}, {"m","100*cm"}, {"meter","m"},
        {"km", "1000*m"}, {"nm","1e-9*m"}, {"angstrom","1e-12*m"},
        {"inch", "2.54*cm"}, 
        {"cm2", "cm*cm"},
        {"cm3", "cm*cm*cm"},
        {"cc", "cm*cm*cm"},

        // angle
        {"radian","1.0"}, {"degree","(3.14159265358979323846/180.0)*radian"},
        {"rad","radian"}, {"deg","degree"},

        // time
        {"nanosecond","1.0"}, {"ns","nanosecond"},{"second","1.e+9*ns"},
        {"millisecond","1.e-3*second"},{"microsecond","1.e-6*second"},
        {"ms","millisecond"}, {"hertz","1.0/second"},
 
        // electric charge
        {"eplus","1"},{"e_SI","1.602176487e-19"},{"coulomb","eplus/e_SI"},

        // energy
        {"megaelectronvolt","1.0"},
        {"electronvolt", "1.e-6*megaelectronvolt"},
        {"kiloelectronvolt", "1.e-3*megaelectronvolt"},
        {"gigaelectronvolt", "1.e+3*megaelectronvolt"},
        {"teraelectronvolt", "1.e+6*megaelectronvolt"},
        {"petaelectronvolt", "1.e+9*megaelectronvolt"},
        {"joule", "electronvolt/e_SI"},
        {"MeV", "megaelectronvolt"},
        {"eV", "electronvolt"},
        {"keV", "kiloelectronvolt"},
        {"GeV", "gigaelectronvolt"},
        {"TeV", "teraelectronvolt"},
        {"PeV", "petaelectronvolt"},
          
        // Mass
        {"kilogram","joule*second*second/(meter*meter)"},
        {"gram","1.e-3*kilogram"},
        {"milligram", "1.e-3*gram"},
        {"kg", "kilogram"},
        {"g","gram"},
        {"mg", "milligram"},

        {"mole","1.0"},

        {0,0}
    };
    for (int ind=0; units[ind].name; ++ind) {
        gROOT->ProcessLine(Form("double %s = %s;", units[ind].name, units[ind].value));
    }

}


double Cowbells::get_num(Json::Value val, double def)
{
    if (val.isNull()) { return def; }

    if (val.isDouble()) { return val.asDouble(); }

    init_units();
    gROOT->ProcessLine(Form("double json_util_double_value = %s;", val.asString().c_str()));
    double ret = *((double*)((TGlobal*)gROOT->GetListOfGlobals()->FindObject("json_util_double_value"))->GetAddress());
    return ret;
}

int Cowbells::get_int(Json::Value val, int def)
{
    if (val.isNull()) { return def; }
    if (val.isInt()) { return val.asInt(); }

    init_units();
    gROOT->ProcessLine(Form("int json_util_int_value = %s;", val.asString().c_str()));
    int ret = *((int*)((TGlobal*)gROOT->GetListOfGlobals()->FindObject("json_util_int_value"))->GetAddress());
    return ret;

}
std::string Cowbells::get_str(Json::Value val, std::string def)
{
    if (val.isNull()) { return def; }
    return val.asString();
}


Json::Value Cowbells::json_parse_file(std::string filename)
{
    ifstream fstr(filename.c_str());
    stringstream ss;
    ss << fstr.rdbuf();
    string data = ss.str();

    Json::Value root;
    Json::Reader reader;

    bool ok = reader.parse(data, root);
    if (!ok) {
        cerr << "Failed to read " << filename << endl;
        cerr << reader.getFormattedErrorMessages() << endl;
        throw invalid_argument("Failed to parse config file");
    }
    return root;
}
    
// Json::Value Cowbells::json_get_keys(vector<Json::Value> roots, vector<string> keys)
// {
//     for (size_t iroot=0; iroot < roots.size(); ++iroot) {
//         Json::Value root = roots[iroot];

//         bool failed = false;
//         for (size_t ikey = 0; ikey < keys.size(); ++ikey) {
//             Json::Value val = root[keys[ikey]];
//             if (val.isNull()) { 
//                 failed = true; 
//                 break; 
//             }
//             root = val;
//         }
//         if (failed || root.isNull()) { 
//             continue; 
//         }
//         return root;
//     }
//     return Json::Value();
// }

Json::Value Cowbells::json_get_fitting(std::vector<Json::Value>& roots,
                                       std::string path)
{
    vector<string> keys = Cowbells::split(path,"/");

    for (size_t iroot=0; iroot < roots.size(); ++iroot) {
        Json::Value root = roots[iroot];

        bool failed = false;
        for (size_t ikey = 0; ikey<keys.size(); ++ikey) {
            Json::Value val = root[keys[ikey]];
            if (val.isNull()) { 
                failed = true; 
                break; 
            }
            root = val;
        }
        if (failed || root.isNull()) { 
            continue; 
        }
        return root;
    }
    cerr << "Unknown configuration item: \"" << path << "\"" << endl;
    return Json::Value();
}
