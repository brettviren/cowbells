#include "Cowbells/Util.h"

#include <iostream>

#include <G4VPhysicalVolume.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4MaterialPropertiesTable.hh>
#include <G4MaterialTable.hh>
#include <G4Material.hh>

using namespace std;



string Cowbells::pv2str(G4VPhysicalVolume& pv)
{
    G4LogicalVolume* lv_mother = pv.GetMotherLogical();
    string mother = "(none)";
    if (lv_mother) {
        mother = lv_mother->GetName();
    }

    G4LogicalVolume* lv_daughter = pv.GetLogicalVolume();
    string daughter = "(none)";
    if (lv_daughter) {
        daughter = lv_daughter->GetName();
    }


    stringstream ss;
    ss << "PV(@0x" << (void*)&pv << "):" << pv.GetName() << " "
       << "mul:" << pv.GetMultiplicity() << " "
       << "copy:" << pv.GetCopyNo()
       << "lv:[\"" << mother << "\"|<-|\"" << daughter << "]\"";
    return ss.str();
}

string Cowbells::lv2str(G4LogicalVolume& lv)
{
    G4Material* mat = lv.GetMaterial();
    if (!mat) {
        cerr << "No material for " << lv.GetName() << endl;
        return lv.GetName();
    }
    stringstream ss;
    ss << "LV(@0x" << (void*)&lv << "):" << lv.GetName() << " "
       << "(" << mat->GetName() << ") "
       << "#children:" << lv.GetNoDaughters();
    return ss.str();
}


void Cowbells::dump(G4VPhysicalVolume* top, int depth) 
{
    std::string tab(depth,' ');

    G4LogicalVolume* lv = top->GetLogicalVolume();
    int nchilds = lv->GetNoDaughters();
    cout << tab << pv2str(*top) << "\n"
         << tab << lv2str(*lv)  << ":" 
         << endl;
    for (int ind=0; ind<nchilds; ++ind) {
        dump(lv->GetDaughter(ind), depth+1);
    }
    
}

void Cowbells::dump_pvs()
{
    G4PhysicalVolumeStore& store = *G4PhysicalVolumeStore::GetInstance();
    cout << "Physical volumes [" << store.size() << "]:" << endl;
    for (size_t ind=0; ind<store.size(); ++ind) {
        G4VPhysicalVolume* pv = store[ind];
        cout << ind << ": " << pv2str(*pv) << endl;
    }
}

void Cowbells::dump_lvs()
{
    G4LogicalVolumeStore& store = *G4LogicalVolumeStore::GetInstance();
    cout << "Logical volumes [" << store.size() << "]:" << endl;
    for (size_t ind=0; ind<store.size(); ++ind) {
        G4LogicalVolume* lv = store[ind];
        cout << ind << ": " << lv2str(*lv) << endl;
    }
}



