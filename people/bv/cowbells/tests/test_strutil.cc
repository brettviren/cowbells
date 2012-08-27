#include "Cowbells/strutil.h"

#include <iostream>
#include <cassert>

using namespace std;
using namespace Cowbells;

void dump_uri(const string& uri)
{
    cout << uri << " --> ";
    vector<string> parts = uri_split(uri);
    for (size_t ind=0; ind<parts.size(); ++ind) {
        cout << parts[ind] << " ";
    }
    cout << endl;
}

void try_arg(const string& uri, const string& argname)
{
    vector<string> spq = uri_split(uri);
    if (spq.size() != 3) { return; }

    string find = argname+"=";
    string arg = get_startswith(spq[2],find,"&");
    cout << "Arg in URI " << uri << " argname " << argname << " found: \"" << arg << "\"" << endl;
}

void try_int(const string& uri, const string& argname)
{
    vector<string> spq = uri_split(uri);
    if (spq.size() != 3) { 
        cout << "URI has no query part: " << uri << endl;
        return; 
    }

    string find = argname+"=";
    int val = uri_integer(spq[2], find, 0);
    cout << "Int in URI " << uri << " argname " << argname << " found integer: " << val << endl;
}

void test_special()
{
    string argstr = "energy=3e-6&pdgcode=2212&pol=0,0,1&vertex=0,0,0&momentum=0,0,3e-6&count=100";
    cout 
        << "energy=" << uri_double(argstr,"energy") << ", "
        << "pdgcode=" << uri_integer(argstr,"pdgcode") << ", "
        << "count=" << uri_integer(argstr,"count") << ", "
        << "vertex=" << uri_threevector(argstr,"vertex") << ", "
        << "momentum=" << uri_threevector(argstr,"momentum") << ", "
        << "pol=" << uri_threevector(argstr,"pol") << " "
        << "from argstr:\"" << argstr << "\"" << endl;
}

int main(int argc, char *argv[])
{
    const char* strings[] = {
        "file:///absolute/path/to/file.txt",
        "file://relative/path/to/file.txt",
        "kin://gun",
        "kin://gun?pdgcode=2212&vertex=1.0,2.0,3.0&momentum0,0,1500",
        "kin://beam?pol=0,0,1&count=42",
        "kin://beam?pdgcode=22",
        0
    };

    const char* argnames[] = {
        "momentum","vertex","pol","count","pdgcode", 0
    };
    const char* intargs[] = {
        "count","pdgcode",0
    };

    for (int ind=0; strings[ind]; ++ind) {
        string uri = strings[ind];

        dump_uri(uri);
        for (int aind=0; argnames[aind]; ++aind) {
            try_arg(uri, argnames[aind]);
        }
        for (int iind=0; intargs[iind]; ++iind) {
            try_int(uri, intargs[iind]);
        }
    }

    test_special();
    return 0;
}
