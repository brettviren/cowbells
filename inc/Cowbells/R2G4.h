// Root -> Geant4 conversion routines

#ifndef R2G4_H
#define R2G4_H

#include <string>

class TGraph;
class G4OpticalSurface;

namespace Cowbells {
    // Tack on material optical properties
    bool AddMaterialProperties(std::string filename);

    bool AddOpticalSurfaces(std::string filename);
    bool SetOpSurfParameter(G4OpticalSurface& opsurf,
                            std::string name, std::string value);
    bool SetOpSurfProperty(G4OpticalSurface& opsurf, TGraph& prop);
    bool SetLogicalSurface(G4OpticalSurface& opsurf, 
                           std::string first, std::string second);
}

#endif  // R2G4_H
