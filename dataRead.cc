//============================================================================
// dataRead.cc
// Main program for driving raw data reading modules 
// Calls DataReadModule
// 
// Author Matt Herndon, University of Wisconsin,
//                      Fermi National Accelerator Laboratory
// 2014-06-03
//============================================================================
#include "DetectorGeometry.hh"
#include "TrackSet.hh"
#include "HitSet.hh"
#include "StripSet.hh"
#include "DataInputModule.hh"
#include <fstream>
#include <iostream>

int main ()
{



  // Debugging
  int debugLevel = 2;
  // six levels of degug
  // 5 most verbose with all printouts
  // Data object printout
  // 1 basic information on progress
  // 0 none

  // Intialize Objects and Modules that are persistant

  // DetectorGeomergy
  std::ifstream detectorgeometryfile("sensorgeometry.txt");
  fc::DetectorGeometry myDetectorGeometry(detectorgeometryfile);  
  // files are closed by the default destructor
  if (debugLevel >= 2) myDetectorGeometry.printDetectorGeometry();

  // Input and output files
  std::ifstream inputeventdatafile("genoutputeventdatafile.bin",std::ios::binary);


  // Instantiate and initialize Module classes
  fc::DataInputModule myDataInputModule(debugLevel,myDetectorGeometry,inputeventdatafile);


  // Event loop over module classes

  for (int ii_event = 0; ii_event < 128; ++ii_event) {
 
   // Initialize object persistent only for each event
    fc::TrackSet genTrackSet(myDetectorGeometry);
    fc::HitSet genHitSet;
    fc::StripSet genStripSet(true);


    myDataInputModule.processEvent(genTrackSet,genHitSet,genStripSet);

  }


  return 0; 

}
