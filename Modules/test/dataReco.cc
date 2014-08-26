//============================================================================
// trackReco.cc
// Main program for driving raw data reading modules and track reconstruction
// Calls DataReadModule and HitRecoModule
//
// Author Matt Herndon, University of Wisconsin,
//                      Fermi National Accelerator Laboratory
// 2014-06-12
//============================================================================
#include "Services/include/Config.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Geometry/include/BuildDetectorGeometry.hh"
#include "Framework/include/EventProcessor.hh"
#include "Modules/include/DataSource.hh"
#include "Modules/include/Day0HistogrammingModule.hh"
#include "Modules/include/HitRecoModule.hh"
#include "Modules/include/HitCompareModule.hh"
#include "Modules/include/PerfectTrackRecoModule.hh"
#include "Modules/include/TrackRecoModule.hh"
#include "Modules/include/TrackSeedModule.hh"
#include "Modules/include/TrackCompareWithPerfectModule.hh"
#include "Modules/include/TrackSeedCompareModule.hh"
#include "Modules/include/TrackCompareWithGenModule.hh"
#include "EventDisplay/include/EventDisplayModule.hh"
#include <fstream>
#include <iostream>
#include <memory>
#include "TFile.h"

int main (int argc,const char *argv[])
{

  int eventNumber = -1;
  if (argc>1) eventNumber = std::atoi( argv[1] );

    // data objects created in this module are reconstruted, not generated
    bool genData = false;

    // Get configuration information.
    std::ifstream configfile("configfilereco.txt");
    fc::Config config(configfile,genData,eventNumber);


    // iostream file for debug output
    std::ofstream debugfile(config.debugFileName().c_str());

    if (config.debugLevel() == 0 ) std::cout << "dataReco running " << std::endl;

    if (config.debugLevel() >=1 ) config.printConfig(debugfile);


    // Intialize Objects and Modules that are persistant

    // DetectorGeomergy
    std::ifstream detectorgeometryfile("sensorgeometry.txt");
    fc::DetectorGeometry detectorGeometry(fc::buildDetectorGeometry(
            detectorgeometryfile));
    // files are closed by the default destructor
    if (config.debugLevel() >= 2) detectorGeometry.printDetectorGeometry(debugfile);

    // Input and output files
    std::ifstream inputeventdatafile("genoutputeventdatafile.bin",std::ios::binary);

    // Open a root file to hold output histograms.
    auto rootFile = std::make_shared<TFile>( config.rootFileName().c_str(),
                    "RECREATE");


// Instantiate the class which handles the details of processing the events
    fc::EventProcessor processor( new fc::DataSource(config.debugLevel(),debugfile,
                                  inputeventdatafile, genData,
                                  "genTracks", //get these tracks from file
                                  "genHits", //get these hits from file
                                  "genStrips", //get these strips
                                  detectorGeometry),
                                  rootFile
                                );


  // Instantiate and initialize Module classes
  if (config.runGenHistogrammingModule()) 
    processor.addModule( new fc::Day0HistogrammingModule("genTracks", // tracks from file
						     "genHits", // hits from file
						     "genStrips", // strips from file
						     detectorGeometry)); //get these strips
 
  if (config.runHitRecoModule()) 
    processor.addModule( new fc::HitRecoModule(config.debugLevel(),debugfile,
					       "genStrips","recoHits", detectorGeometry));

  if (config.runHitCompareModule()) 
    processor.addModule( new fc::HitCompareModule(config.debugLevel(),"genHits",
						  "recoHits", detectorGeometry));

  if (config.runPerfectTrackRecoModule()) 
    processor.addModule( new fc::PerfectTrackRecoModule(config.debugLevel(),debugfile,
							"recoHits", "genHits", "perfectRecoTracks", detectorGeometry) );
  if (config.runPerfectTrackCompareWithGenModule()) 
    processor.addModule( new fc::TrackCompareWithGenModule(config.debugLevel(),debugfile,
							   "genTracks", "perfectRecoTracks", detectorGeometry) );

  if (config.runTrackSeedModule()) 
    processor.addModule( new fc::TrackSeedModule(config.debugLevel(),debugfile,
						      "recoHits", "seedTracks","trackSeedStrategy2X1SASML",config,detectorGeometry) );
  if (config.runTrackSeedCompareModule()) 
    processor.addModule( new fc::TrackSeedCompareModule(config.debugLevel(),debugfile,
							"perfectRecoTracks", "seedTracks", detectorGeometry) );


  if (config.runTrackRecoModule()) 
    processor.addModule( new fc::TrackRecoModule(config.debugLevel(), debugfile,"recoHits",
						 "seedTracks", "recoTracks",config,detectorGeometry) );
  if (config.runTrackCompareWithPerfectModule()) 
    processor.addModule( new fc::TrackCompareWithPerfectModule(config.debugLevel(),
							       "perfectRecoTracks", "recoTracks", detectorGeometry) );
  if (config.runRecoTrackCompareWithGenModule()) 
    processor.addModule( new fc::TrackCompareWithGenModule(config.debugLevel(), debugfile,
							   "genTracks", "recoTracks", detectorGeometry) );
 

  if (config.runEventDisplayModule()) 
    processor.addModule( new fc::EventDisplayModule(config.debugLevel(),"genHits","genTracks","recoHits","recoTracks",config,detectorGeometry) );
 



 
    // Event loop over module classes
    processor.processEvents(config);

    // end job functions.  Only needed to write and close root files
    processor.endJob();

    return 0;

}
