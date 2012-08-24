#ifndef STRUTIL_H
#define STRUTIL_H

#include <G4ThreeVector.hh>

#include <string>
#include <vector>

namespace Cowbells {
    /// Split str into substrings based delimiter.
    std::vector<std::string> split(const std::string& str, const std::string& delim);

    /// Return true if thing is in given str.
    bool in(const std::string& str, const std::string& thing);

    G4ThreeVector str2threevector(const std::string& str);
}


#endif  // STRUTIL_H
