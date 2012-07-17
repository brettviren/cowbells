/**
 * \class TestDetectorConstruction
 *
 * \brief Build a simple test geometry and materials
 *
 *
 * bv@bnl.gov Mon Jul 16 13:34:56 2012
 *
 */


#ifndef TESTDETECTORCONSTRUCTION_H
#define TESTDETECTORCONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>

namespace Cowbells {

    class TestDetectorConstruction : public G4VUserDetectorConstruction {
    public:

        TestDetectorConstruction();
        virtual ~TestDetectorConstruction();

        // G4VU.D.C. interface
        virtual G4VPhysicalVolume* Construct();
        
    };

}



#endif  // TESTDETECTORCONSTRUCTION_H
