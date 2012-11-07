#include "Cowbells/Json2G4.h"
#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Cintex/Cintex.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    Cowbells::Json2G4 j2g4;
    for (int ind=1; ind<argc; ++ind) {
        cerr << "Adding file: " << argv[ind] << endl;
        j2g4.add_file(argv[ind]);
    }

    G4RunManager rm;

    j2g4.read();

    j2g4.make();

    return 0;
}
