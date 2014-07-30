//============================================================================
// dataRead.cc
// Main program for driving raw data reading modules 
// Calls DataReadModule
// 
// Author Matt Herndon, University of Wisconsin,
//                      Fermi National Accelerator Laboratory
// 2014-06-03
//============================================================================
#include "Services/include/Config.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Geometry/include/BuildDetectorGeometry.hh"
#include "Framework/include/EventProcessor.hh"
#include "Modules/include/DataSource.hh"
#include <fstream>
#include <iostream>

#include "TFile.h"

int main ()
{
 

  // Generator data
  bool genData = false;

  // Configure genData using general Config class
  std::ifstream configfile("configfilereco.txt");
  fc::Config config(configfile,genData);

  // Open a root file to hold output histograms.
  auto rootFile = std::make_shared<TFile>( config.getRootFileName().c_str(), "RECREATE");

  // Intialize Objects and Modules that are persistant

  // DetectorGeomergy
  std::ifstream detectorgeometryfile("sensorgeometry.txt");
  fc::DetectorGeometry detectorGeometry(fc::buildDetectorGeometry(detectorgeometryfile));  
  // files are closed by the default destructor
  if (config.getDebugLevel() >= 2) detectorGeometry.printDetectorGeometry(std::cout);

  // Input and output files
  std::ifstream inputeventdatafile("genoutputeventdatafile.bin",std::ios::binary);

 // Instantiate the class which handles the details of processing the events
  fc::EventProcessor processor( new fc::DataSource(config.getDebugLevel(),inputeventdatafile, genData,
                                               "genTracks", //get these tracks from file
                                                "hits", //get these hits from file
                                                "strips", //get these strips
						   detectorGeometry),
				                  rootFile );

  // Instantiate and initialize Module classes
  //  the order the modules are passed to the EventProcessor
  //  is the order the modules will run
 
  // Event loop over module classes
  processor.processEvents();

  processor.endJob();
  return 0; 

}
