// $Id: example06LinkDef.h 341 2008-05-26 11:04:57Z ivana $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file  example06LinkDef.h
/// \brief The CINT link definitions for example E06 classes

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
 
#pragma link C++ class  Ex06MCApplication+;
#pragma link C++ class  Ex03MCStack+;
#pragma link C++ class  Ex06DetectorConstruction+;
#pragma link C++ class  Ex06DetectorConstructionOld+;
#pragma link C++ class  Ex06PrimaryGenerator+;

#endif





