#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H

#include "Cowbells/Json2G4.h"

#include <G4VUserDetectorConstruction.hh>

namespace Cowbells {

    class DetectorConstruction : public G4VUserDetectorConstruction {
    public:
    
        DetectorConstruction(Cowbells::Json2G4& j2g4);
        virtual ~DetectorConstruction();
    

        // G4VU.D.C. interface - subclass should not implement
        virtual G4VPhysicalVolume* Construct();

    private:
        Json2G4& m_j2g4;
    };
    

}

#endif  // DETECTORCONSTRUCTION_H
