//============================================================================
// dataRead.cc
// Main program for driving raw data reading modules and hit reconstruction 
// Calls DataReadModule and HitRecoModule
// 
// Author Matt Herndon, University of Wisconsin,
//                      Fermi National Accelerator Laboratory
// 2014-06-12
//============================================================================
#include "DetectorGeometry.hh"
#include "TrackSet.hh"
#include "HitSet.hh"
#include "StripSet.hh"
#include "DataInputModule.hh"
#include "HitRecoModule.hh"
#include "HitCompareModule.hh"
#include "TrackRecoModule.hh"
#include <fstream>
#include <iostream>
#include <memory>
#include "TFile.h"

int main ()
{



  // Debugging
  int debugLevel = 2;
  // six levels of degug
  // 5 most verbose with all printouts
  // Data object printout
  // 1 basic information on progress
  // 0 none

  // data objects created in this module are reconstruted, not generated
  bool genData = false;

  // Intialize Objects and Modules that are persistant

  // DetectorGeomergy
  std::ifstream detectorgeometryfile("sensorgeometry.txt");
  fc::DetectorGeometry myDetectorGeometry(detectorgeometryfile);  
  // files are closed by the default destructor
  if (debugLevel >= 2) myDetectorGeometry.printDetectorGeometry();

  // Input and output files
  std::ifstream inputeventdatafile("genoutputeventdatafile.bin",std::ios::binary);

  TFile * outputrootfile = new TFile("outputfile.root", "RECREATE");


  // Instantiate and initialize Module classes
  fc::DataInputModule myDataInputModule(debugLevel,myDetectorGeometry,inputeventdatafile);
  fc::HitRecoModule myHitRecoModule(debugLevel,myDetectorGeometry);
  fc::HitCompareModule myHitCompareModule(debugLevel,myDetectorGeometry,outputrootfile);
  fc::TrackRecoModule myTrackRecoModule(debugLevel,myDetectorGeometry);


  // Event loop over module classes

  for (int ii_event = 0; ii_event < 128; ++ii_event) {
 
   // Initialize object persistent only for each event
    fc::TrackSet myGenTrackSet(myDetectorGeometry);
    fc::HitSet myGenHitSet;
    fc::StripSet myGenStripSet;

    fc::HitSet myRecoHitSet(genData);
    fc::TrackSet myRecoTrackSet(myDetectorGeometry);

    myDataInputModule.processEvent(myGenTrackSet,myGenHitSet,myGenStripSet);
    myHitRecoModule.processEvent(myRecoHitSet,myGenStripSet);
    myHitCompareModule.processEvent(myGenHitSet,myRecoHitSet);
    myTrackRecoModule.processEvent(myRecoTrackSet,myGenHitSet);



  }


  // end job functions.  Only needed to write and close root files

  myHitCompareModule.endjob();



  return 0; 

}
