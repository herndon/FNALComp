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
#include "DataSource.hh"
#include "Config.hh"
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
  // 2 Data object printout
  // 1 basic information on progress
  // 0 none

  // Generator data
  bool genData = 0;

  // Configure genData using general Config class
  std::ifstream configfile("configfile.txt");
  fc::Config config(configfile,genData);


  // Intialize Objects and Modules that are persistant

  // DetectorGeomergy
  std::ifstream detectorgeometryfile("sensorgeometry.txt");
  fc::DetectorGeometry detectorGeometry(detectorgeometryfile);  
  // files are closed by the default destructor
  if (debugLevel >= 2) detectorGeometry.printDetectorGeometry();

  // Input and output files
  std::ifstream inputeventdatafile("genoutputeventdatafile.bin",std::ios::binary);

 // Instantiate the class which handles the details of processing the events
  fc::EventProcessor processor( new fc::DataSource(debugLevel,inputeventdatafile, genData,
                                               "tracksWithHits", //get these tracks from file
                                                "hits", //get these hits from file
                                                "strips", //get these strips
                                                detectorGeometry));

  // Instantiate and initialize Module classes
  //  the order the modules are passed to the EventProcessor
  //  is the order the modules will run
 
  // Event loop over module classes
  processor.processEvents();

  processor.endJob();
  return 0; 

}
