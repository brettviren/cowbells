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

#include <TVirtualMCApplication.h>

class CowMCapp : public TVirtualMCApplication
{
public:
    CowMCapp();
    CowMCapp(const char* name,  const char *title);
    virtual ~CowMCapp();

    // My interface

    /// Set a file from which to get material property data
    void SetPropertiesFile(const char* propertiesfile) {
        m_propertiesfile = propertiesfile;
    }

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

private:
    void DefineProperties();
    std::string m_propertiesfile;

};
#endif  // COWMCAPP_H
