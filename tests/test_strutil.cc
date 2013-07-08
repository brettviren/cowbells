/** test strutil

Build like:

g++ -g -o test_strutil -Wall ../cowbells/tests/test_strutil.cc \
     -I $geant4_install_dir/include/Geant4 \
     -I ../cowbells/inc \
     ../cowbells/src/Cowbells/strutil.cc \
     -L $geant4_install_dir/lib \
     -L $geant4_install_dir/lib64 \
     -lG4clhep

*/

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
    string arg = get_startswith_rest(spq[2],find,"&");
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
    cerr << "energy:" << fabs(3e-6-uri_double(argstr,"energy")) << endl;
    assert(fabs(3e-6-uri_double(argstr,"energy")) < 1e-8);
    assert(2212 == uri_integer(argstr,"pdgcode"));
    assert(100 == uri_integer(argstr,"count"));

    cout 
        << "energy=" << uri_double(argstr,"energy") << ", "
        << "pdgcode=" << uri_integer(argstr,"pdgcode") << ", "
        << "count=" << uri_integer(argstr,"count") << ", "
        << "vertex=" << uri_threevector(argstr,"vertex") << ", "
        << "momentum=" << uri_threevector(argstr,"momentum") << ", "
        << "pol=" << uri_threevector(argstr,"pol") << " "
        << "from argstr:\"" << argstr << "\"" << endl;
}

void test_special2()
{
    const char* physics = "em,op,had";
    const char* phy[] = {"em","op","had",0};
    for (int ind=0; phy[ind]; ++ind) {
        string what = get_startswith(physics,phy[ind],",","notfound");
        cout << "get \"" << phy[ind] << "\" from \"" 
             << physics << "\" returns \"" << what << "\"" << endl;
        assert (phy[ind] == what);
    }
}

void test_startswith()
{
    const char* modules = "kine,hits,steps,stacks";
    const char* mod[] = {"kine","hits","steps","stacks",0};
    for (int ind=0; mod[ind]; ++ind) {
        string what = get_startswith(modules, mod[ind]);
        cout << "get \"" << mod[ind] << "\" from \"" 
             << modules << "\" returns \"" << what << "\"" << endl;
        assert (mod[ind] == what);
    }
}
void test_startswith2()
{
    const char* modules = "exponential,fixed";
    const char* mod[] = {"exponential","fixed",0};
    for (int ind=0; mod[ind]; ++ind) {
        string what = get_startswith(modules, mod[ind]);
        cout << "get \"" << mod[ind] << "\" from \"" 
             << modules << "\" returns \"" << what << "\"" << endl;
        assert (mod[ind] == what);
    }
}

void test_lower()
{
    const char* words[] = {"lower","UPPER","StudlyCaps","inTHEmiddle",0};
    for (int ind=0; words[ind]; ++ind) {
        string l = lower(words[ind]);
        cout << ind << ": " << words[ind] << " --> " << l << endl;
    }
}

void test_direction()
{
    const char* url[] = {
	"kin://beam?vertex=0,0,1&name=opticalphoton&direction=0,0,1&energy=0.000003&pol=0,1,0&spread=1.571&count=100",
	"kin://beam?vertex=0,0,1&name=opticalphoton&direction=0,0,1&energy=0.000003&pol=0,1,0&spread=1&count=100",
	"kin://beam?vertex=0,0,1&name=opticalphoton&direction=0,0,1&energy=0.000003&pol=0,1,0&spread=0.001&count=100",
	0
    };
    for (int idir = 0; url[idir]; ++idir) {
	cout << "Directions from " << idir << ": " << url[idir] << endl;
	for (int ind=0; ind < 10; ++ind) {
	    G4ThreeVector dir = uri_direction(url[idir]);
	    cout << "\t" << ind << ": " << dir << endl;
	}
    }
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
    test_special2();
    test_startswith();
    test_startswith2();
    test_lower();
    test_direction();
    return 0;
}
