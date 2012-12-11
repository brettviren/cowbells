/**
 * \class PrimaryGenerator
 *
 * \brief A G4 primary generator action
 *
 * This class generates the primary kinematics based on a given URI.
 * There are two URI schemes accepted: "file" and "kin".
 *
 * The "file" scheme specifies a file containing kinematics information:
 *
 *  file://relpath/to/kinematics/file.txt
 *  file:///abspath/to/kinematics/file.txt
 *
 *
 * The "kin" scheme specifies a supported kinematics generator
 * followed with any required or optional arguments specified as URI
 * query strings.  
 * 
 * Supported generators:
 * 
 *  kin://beam?<pdgcode>&<momentum>&<vertex>&<pol>&<count>
 *
 * Args:
 *
 *  pdgcode=INTEGER : set the PDG particle identification number, default=0
 *  
 *  momentum=FLOAT,FLOAT,FLOAT : set the momentum vector, default=0,0,0
 *
 *  vertex=FLOAT,FLOAT,FLOAT : set the vertex vector, default = 0,0,0
 *
 *  pol=FLOAT,FLOAT,FLAOT : set the polarization vector, default = 0,0,0
 *
 *  count=INTEGER : set the multiplicity, default=1
 *
 * All numerical quantites with units are implicitly taken to be in
 * the G4 system of units (mm,ns,MeV).
 *
 * bv@bnl.gov Thu May 10 12:58:31 2012
 *
 */



#ifndef PRIMARYGENERATOR_H
#define PRIMARYGENERATOR_H

#include "G4VUserPrimaryGeneratorAction.hh"

namespace Cowbells {

    class PrimaryGenerator : public G4VUserPrimaryGeneratorAction {
    public:
        PrimaryGenerator(const char* kindesc = 0);
        virtual ~PrimaryGenerator();

        virtual void SetKinDesc(const char* kindesc);
        void GeneratePrimaries(G4Event* gevt);

    };
}


#endif  // PRIMARYGENERATOR_H
