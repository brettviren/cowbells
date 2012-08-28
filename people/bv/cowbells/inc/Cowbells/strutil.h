#ifndef STRUTIL_H
#define STRUTIL_H

#include <G4ThreeVector.hh>
#include <CLHEP/Vector/ThreeVector.h>

#include <string>
#include <vector>

namespace Cowbells {
    /// Split str into substrings based delimiter.
    std::vector<std::string> split(const std::string& str, const std::string& delim);

    /// Split and return the remaining part of the element that starts with the given string
    std::string get_startswith(const std::string& str, 
                               const std::string& startswith,
                               const std::string& delim = ",",
                               const std::string& def = "");

    /// Return true if thing is in given str.
    bool in(const std::string& str, const std::string& thing);

    /// Convert string holding comma-separated numbers as a three vector
    G4ThreeVector str2threevector(const std::string& str);

    /// Split scheme://path[?query] as vector of (scheme,path,[query])
    std::vector<std::string> uri_split(const std::string& uri);

    /// Return named URI query argument as a three vector
    G4ThreeVector uri_threevector(const std::string& argstr, const std::string& name, 
                                  G4ThreeVector def = G4ThreeVector());

    /// Return named URI query argument as an integer
    int uri_integer(const std::string& argstr, const std::string& name, int def = 0);

    /// Return named URI query argument as a double
    double uri_double(const std::string& argstr, const std::string& name, double def = 0.0);
}


#endif  // STRUTIL_H
