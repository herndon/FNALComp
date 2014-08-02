//============================================================================
// hitReco.cc
// Main program for driving raw data reading modules and hit reconstruction 
// Calls DataReadModule and HitRecoModule
// 
// Author Matt Herndon, University of Wisconsin,
//                      Fermi National Accelerator Laboratory
// 2014-06-12
//============================================================================
#include "Services/include/Config.hh"
#include "Geometry/include/BuildDetectorGeometry.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Framework/include/EventProcessor.hh"
#include "Modules/include/DataSource.hh"
#include "Modules/include/HitRecoModule.hh"
#include "Modules/include/HitCompareModule.hh"
#include <fstream>
#include <iostream>
#include <memory>
#include "TFile.h"

int main ()
{



  // Debugging

  // data objects created in this module are reconstruted, not generated
  bool genData = false;

  // Get configuration information.
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
                                                "genHits", //get these hits from file
                                                "genStrips", //get these strips
						   detectorGeometry),
				                  rootFile );

  // Instantiate and initialize Module classes
  processor.addModule( new fc::HitRecoModule(config.getDebugLevel(),"genStrips", "recoHits", detectorGeometry) );
  processor.addModule( new fc::HitCompareModule(config.getDebugLevel(),"genHits", "recoHits", detectorGeometry) );


  // Event loop over module classes
  processor.processEvents();

  // end job functions.  Only needed to write and close root files
  processor.endJob();

  return 0; 

}
