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
#include "Framework/include/EventProcessor.hh"
#include "Modules/include/DataSource.hh"
#include <fstream>
#include <iostream>

#include "TFile.h"

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
  bool genData = false;

  // Configure genData using general Config class
  std::ifstream configfile("configfilereco.txt");
  fc::Config config(configfile,genData);

  // Open a root file to hold output histograms.
  TFile* rootFile = new TFile( config.getRootFileName().c_str(), "RECREATE");

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
