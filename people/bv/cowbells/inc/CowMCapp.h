/**
 * \class CowMCapp
 *
 * \brief Cowbells MC application
 *
 * bv@bnl.gov Tue Apr 24 15:29:17 2012
 *
 */

#ifndef COWMCAPP_H
#define COWMCAPP_H

#include "CowPatty.h"

#include <TVirtualMCApplication.h>
#include <TVirtualMC.h>
#include <TGraph.h>

class CowMCapp : public TVirtualMCApplication
{
public:
    CowMCapp();
    CowMCapp(const char* name,  const char *title);
    virtual ~CowMCapp();

    // My interface

    /// Set a file from which to get material property data
    void SetPropertiesFile(const char* propertiesfile);
    TVirtualMCStack* GetStack();

    // If a cintfile is given it is loaded and the function "Config()"
    // is called
    void InitMC(const char* cintfile = 0);
    void RunMC(Int_t nofEvents);

    // Required TVirtualMCApplication API
    virtual void ConstructGeometry();
    virtual void GeneratePrimaries();
    virtual void BeginEvent();
    virtual void BeginPrimary();
    virtual void PreTrack();
    virtual void Stepping();
    virtual void PostTrack();
    virtual void FinishPrimary();
    virtual void FinishEvent();

    // Optional TVirtualMCApplication API
    virtual void ConstructOpGeometry();
    virtual void InitGeometry();

    void DefineProperty(int matid, TGraph& prop);

private:
    void DefineProperties();

    std::string m_propertiesfile;
    CowPatty* m_stack;

    ClassDef(CowMCapp,1)  //Interface to MonteCarlo application
};
#endif  // COWMCAPP_H
