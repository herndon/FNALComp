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
#include "DetectorGeometry.hh"
#include "TrackGenModule.hh"
#include "HitStripGenModule.hh"
#include "DataOutputModule.hh"
#include "Config.hh"
#include "Random.hh"
#include "Event.hh"
#include "EventProcessor.hh"
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


  // Generator data
  bool genData = 1;

  // Configure genData using general Config class
  std::ifstream configfile("configfile.txt");
  fc::Config myConfig(configfile,genData);

 // Intialize Objects and Modules that are persistant
 
 // Initialie random number generator with seed = 1
  fc::Random myRandom(myConfig.getSeed());

  // DetectorGeometry
  std::ifstream detectorgeometryfile("sensorgeometry.txt");
  fc::DetectorGeometry myDetectorGeometry(detectorgeometryfile);  
  // files are closed by the default destructor
  if (debugLevel >= 2) myDetectorGeometry.printDetectorGeometry();

  // Input and output files and modules
  std::ofstream genoutputeventdatafile("genoutputeventdatafile.bin",std::ios::binary);


  // Instantiate the class which handles the details of processing the events
  fc::EventProcessor processor(genData);
 
  // Instantiate and initialize Module classes
  //  the order the modules are passed to the EventProcessor
  //  is th eorder the modules will run
  processor.addModule( new fc::TrackGenModule(debugLevel,myConfig.getNumberTracks(),myDetectorGeometry,myRandom));
  processor.addModule( new fc::HitStripGenModule(debugLevel,myDetectorGeometry,myRandom) );
  processor.addModule( new fc::DataOutputModule(debugLevel,myDetectorGeometry,genoutputeventdatafile));

  // Event loop over module classes
  processor.processEvents(myConfig.getNumberEvents());

  return 0; 

}
