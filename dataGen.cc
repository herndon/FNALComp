//============================================================================
// dataGen.cc
// Main program for driving data gen Modules
// Calls DataGenModule
// 
// Planned development: adapt to framework when ready
//
// Author Matt Herndon, University of Wisconsin,
//                      Fermi National Accelerator Laboratory
// 2014-05-22
//============================================================================
#include "DataGenModule.hh"
#include "StripSet.hh"
#include "DataGenModule.hh"
#include "Random.hh"
#include <fstream>
#include <iostream>

int main ()
{

  // Initialie random number generator
  seedEngine();

  // Debugging
  int debugLevel = 2;
  // six levels of degug
  // 5 most verbose with all printouts
  // Data object printout
  // 1 basic information on progress
  // 0 none

  // Intialize DetectorGeomentry and StripSet
  // Eventually these will come from the framework
 
  std::ifstream detectorgeometryfile;
  detectorgeometryfile.open("sensorgeometry.txt");
  DetectorGeometry myDetectorGeometry(detectorgeometryfile);  
  detectorgeometryfile.close();
  if (debugLevel >= 2) myDetectorGeometry.printDetectorGeometry();

  StripSet myStripSet;

  // Open geneartor output event data file
  std::ofstream genoutputeventdatafile;
  genoutputeventdatafile.open("genoutputeventdatafile.bin",std::ios::binary);

  // Instantiate and initialize Module classes
  DataGenModule myDataGenModule(debugLevel,myDetectorGeometry,myStripSet,genoutputeventdatafile);


   // Event loop over module classes

  for (int ii_event = 0; ii_event < 128; ++ii_event) {

    myDataGenModule.event();

  }

  // Clean up

  genoutputeventdatafile.close();

  return 0; 

}
