#ifndef CBUTIL_H
#define CBUTIL_H

#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolume.hh>
#include <string>


namespace Cowbells {

    void dump(G4VPhysicalVolume* top, int depth) ;
    void dump_pvs();
    void dump_lvs();

    std::string pv2str(G4VPhysicalVolume& pv);
    std::string lv2str(G4LogicalVolume& lv);
}

#endif  // CBUTIL_H
