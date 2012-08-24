#include "Cowbells/strutil.h"

#include <iostream>
using namespace std;

void dump(vector<string>& strs)
{
    for (size_t ind=0; ind<strs.size(); ++ind) {
        cout << ind << ": " << strs[ind] << endl;
    }
}

int main(int argc, char *argv[])
{
    const char* strings[] = {
        "http://example.com/path/to/file",
        "http://example.com/path/to/dir/",
        "gun://2212/1.0,2.0,3.0/0,0,1500",
        "file:///absolute/path/to/file.txt",
        "file://relative/path/to/file.txt",
        0
    };
    for (int ind=0; strings[ind]; ++ind) {
        cout << strings[ind] << endl;
        vector<string> chunks = Cowbells::split(strings[ind],"://");
        dump(chunks);
        vector<string> parts = Cowbells::split(chunks[1],"/");
        dump(parts);
        for (size_t pind=0; pind<parts.size(); ++pind) {
            if (Cowbells::in(parts[pind],",")) {
                vector<string> v = Cowbells::split(parts[pind],",");
                cout << pind << ": part: " << parts[pind] << endl;
                dump(v);
            }
        }
    }
    return 0;
}
