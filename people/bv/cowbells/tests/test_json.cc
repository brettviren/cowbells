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

void dump_scalar(Json::Value& val)
{

    if (val.isString()) {
        cout << "string(" << val.asString() << ")";
    } 
    else if (val.isBool()) {
        cout << "bool(" << val.asBool() << ")";
    }
    else if (val.isInt()) {
        cout << "int(" << val.asInt() << ")";
    }
    else if (val.isUInt()) {
        cout << "uint(" << val.asUInt() << ")";
    } 
    else if (val.isDouble()) {
        cout << "double(" << val.asDouble() << ")";
    }
    else {
        cout << "unknown(" << val.type() << ")";
    }
}

void dump(Json::Value val, string tab = "");
void dump(Json::Value val, string tab)
{
    if (val.isObject()) {

        cout << tab << "{(" << val.size() << ") " <<  endl;
        Json::ValueIterator it = val.begin();
        for (size_t count=0; count< val.size(); ++it, ++count) {
            Json::Value k = it.key();
            cout << "\"";
            dump_scalar(k);
            cout << "\" : ";
            dump(*it, tab + "  ");
            cout << endl;
        }

        cout << tab << "}" << endl;
        return;
    }

    if (val.isArray()) {
        cout << tab << "[" << endl;
        Json::ValueIterator it = val.begin();
        
        for (size_t count=0; count < val.size(); ++it, ++count) {
            dump(*it, tab + "  ");
            cout << endl;
        }
        cout << tab << "]" << endl;
        return;
    }


    cout << tab;
    dump_scalar(val);
    cout << endl;
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

    Json::FastWriter writer;

    cout << "Got:" << endl;
    cout << writer.write(root) << endl;

    dump(root);

    Json::StyledWriter sw;
    cout << sw.write(root) << endl;

    return 0;
}
