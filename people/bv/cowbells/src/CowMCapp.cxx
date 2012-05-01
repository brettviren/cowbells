#include "CowMCapp.h"

#include <TDatabasePDG.h>
#include <TParticlePDG.h>
#include <TVirtualMC.h>         // for gMC
#include <TGeoManager.h>
#include <TFile.h>
#include <TKey.h>

#include <string>
#include <iostream>
using namespace std;

ClassImp(CowMCapp)

CowMCapp::CowMCapp()
    : TVirtualMCApplication()
    , m_propertiesfile("")
    , m_stack(0)
{
}
CowMCapp::CowMCapp(const char* name,  const char *title)
    : TVirtualMCApplication(name,title)
    , m_propertiesfile("")
    , m_stack(new CowPatty(1000))
{
}
CowMCapp:: ~CowMCapp()
{
    delete m_stack;

    delete gMC;
    gMC = 0;
}

void CowMCapp::InitMC(TVirtualMC* mc)
{    
    mc->SetStack(m_stack);
    //gMC->SetMagField(fMagField);
    //gMC->Init();
    //gMC->BuildPhysics(); 
}

void CowMCapp::RunMC(Int_t nofEvents)
{    
    gMC->ProcessRun(nofEvents);
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
    this->DefineProperties();
    
}

void CowMCapp::DefineProperty(int matid, TGraph& prop)
{
    std::string propname = prop.GetName();
    
    // scalar
    if (prop.GetN() == 1) {
        gMC->SetMaterialProperty(matid, prop.GetName(), prop.GetY()[0]);
        return;
    }

    gMC->SetMaterialProperty(matid, prop.GetName(), prop.GetN(), prop.GetX(), prop.GetY());
    return;
}

void CowMCapp::SetPropertiesFile(const char* propertiesfile)
{
    cout << "moo: Using properties from: " << propertiesfile << endl;
    m_propertiesfile = propertiesfile;
}

TVirtualMCStack* CowMCapp::GetStack()
{
    return m_stack;
}

void CowMCapp::DefineProperties()
{
    if (! m_propertiesfile.size()) { 
        cout << "moo: no properties file given, this is probably not what you want" << endl;
        return; 
    }

    TFile fp(m_propertiesfile.c_str());
    TDirectory *propdir = dynamic_cast<TDirectory*>(fp.Get("properties"));

    TList* mat_lok = propdir->GetListOfKeys();
    for (int mat_ind=0; mat_ind < mat_lok->GetSize(); ++mat_ind) {
        TKey* mat_key = dynamic_cast<TKey*>(mat_lok->At(mat_ind));
        TDirectory* mat_dir = dynamic_cast<TDirectory*>(mat_key->ReadObj());

        int mat_id = gMC->MediumId(mat_dir->GetName());
        if (!mat_id) {
            cerr << "Error: zero ID returned for \"" << mat_dir->GetName() 
                 << "\" skipping" << endl;
            continue;
        }

        cout << "moo: Loading properties for " << mat_dir->GetName() << endl;

        TList* prop_lok = mat_dir->GetListOfKeys();
        for (int prop_ind=0; prop_ind<prop_lok->GetSize(); ++prop_ind) {
            TKey* prop_key = dynamic_cast<TKey*>(prop_lok->At(prop_ind));
            TGraph* prop = dynamic_cast< TGraph*>(prop_key->ReadObj());
            cout << "moo:\tproperty " << prop->GetName() << endl;
            this->DefineProperty(mat_id,*prop);
        }
    }
}

void CowMCapp::GeneratePrimaries()
{
    cout << "moo: GeneratePrimaries" << endl;

    //  Fixme: Here we just slam a muon to get something

    int op_id = 50000050;

    double dir[3] = {1.0, 0.0, 0.0};
    double polang = 90.0;       // deg

    TVector3 polar;
    if ( op_id == 50000050 ) {
        TVector3 normal (1., 0., 0.);
        TVector3 kphoton = TVector3(dir[0], dir[1], dir[2]);
        TVector3 product = normal.Cross(kphoton); 
        Double_t modul2  = product*product;
 
        TVector3 e_perpend (0., 0., 1.);
        if (modul2 > 0.) e_perpend = (1./sqrt(modul2))*product; 
        TVector3 e_paralle = e_perpend.Cross(kphoton);
        
        polar =   TMath::Cos(polang*TMath::DegToRad())*e_paralle 
            + TMath::Sin(polang*TMath::DegToRad())*e_perpend;
    }	


    double energy = 3.0e-09;    // 3 eV
    int track_id = -1;
    m_stack->PushTrack(1, -1, op_id, 
                       0.0, 0.0, energy, energy,
                       0, 0, 0, 0, // x,y,z,t
                       polar.X(), polar.Y(), polar.Z(),                        
                       kPPrimary, track_id, 1., 0);
    cout << "moo: pushed track id " << track_id << endl;
}

void CowMCapp::BeginEvent()
{
    cout << "moo: BeginEvent" << endl;
}

void CowMCapp::BeginPrimary()
{
    //cout << "moo: BeginPrimary" << endl;
}

void CowMCapp::PreTrack()
{
    //cout << "moo: PreTrack" << endl;
}

void CowMCapp::Stepping()
{
    //cout << "moo: Stepping" << endl;
}

void CowMCapp::PostTrack()
{
    //cout << "moo: PostTrack" << endl;
}

void CowMCapp::FinishPrimary()
{
    //cout << "moo: FinishPrimary" << endl;
}

void CowMCapp::FinishEvent()
{
    cout << "moo: FinishEvent" << endl;
}

