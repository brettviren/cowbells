#include "CowMCapp.h"

#include <TVirtualMC.h>         // for gMC
#include <TGeoManager.h>

#include <string>
#include <iostream>
using namespace std;

CowMCapp::CowMCapp()
    : TVirtualMCApplication()
{
}
CowMCapp::CowMCapp(const char* name,  const char *title)
    : TVirtualMCApplication(name,title)
{
}
CowMCapp:: ~CowMCapp()
{
    delete gMC;
    gMC = 0;
}

void CowMCapp::InitGeometry()
{
    cout << "moo: InitGeometry" << endl;
}

void CowMCapp::ConstructGeometry()
{
    cout << "moo: ConstructGeometry" << endl;
    
    // close geometry
    gGeoManager->CloseGeometry();

    // notify VMC about Root geometry
    gMC->SetRootGeometry();

}

void CowMCapp::ConstructOpGeometry()
{
    cout << "moo: ConstructOpGeometry" << endl;
}

void CowMCapp::GeneratePrimaries()
{
    cout << "moo: GeneratePrimaries" << endl;
}

void CowMCapp::BeginEvent()
{
    cout << "moo: BeginEvent" << endl;
}

void CowMCapp::BeginPrimary()
{
    cout << "moo: BeginPrimary" << endl;
}

void CowMCapp::PreTrack()
{
    cout << "moo: PreTrack" << endl;
}

void CowMCapp::Stepping()
{
    cout << "moo: Stepping" << endl;
}

void CowMCapp::PostTrack()
{
    cout << "moo: PostTrack" << endl;
}

void CowMCapp::FinishPrimary()
{
    cout << "moo: FinishPrimary" << endl;
}

void CowMCapp::FinishEvent()
{
    cout << "moo: FinishEvent" << endl;
}

