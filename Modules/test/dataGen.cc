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
#include "Geometry/include/DetectorGeometry.hh"
#include "Geometry/include/BuildDetectorGeometry.hh"
#include "Modules/include/TrackGenModule.hh"
#include "Modules/include/HitStripGenModule.hh"
#include "Modules/include/DataOutputModule.hh"
#include "Modules/include/CountEventsSource.hh"
#include "Services/include/Config.hh"
#include "Services/include/Random.hh"
#include "Framework/include/EventProcessor.hh"
#include <fstream>
#include <iostream>

#include "TFile.h"

int main ()
{



    // Generator data
    bool genData = true;

    // Configure genData using general Config class
    std::ifstream configfile("configfile.txt");
    fc::Config config(configfile,genData,-1);

// Intialize Objects and Modules that are persistant

// Initialie random number generator with seed = 1
    fc::Random random(config.seed());

    // Open a root file to hold output histograms.
    auto rootFile = std::make_shared<TFile>( config.rootFileName().c_str(),
                    "RECREATE");

    // DetectorGeometry
    std::ifstream detectorgeometryfile("sensorgeometry.txt");
    fc::DetectorGeometry detectorGeometry(fc::buildDetectorGeometry(
            detectorgeometryfile));
    // files are closed by the default destructor
    if (config.debugLevel() >= 2) detectorGeometry.printDetectorGeometry(
            std::cout);

    // Input and output files and modules
    std::ofstream genoutputeventdatafile("genoutputeventdatafile.bin",
                                         std::ios::binary);


    // Instantiate the class which handles the details of processing the events
    fc::EventProcessor processor(new fc::CountEventsSource(config.debugLevel(),
                                 config.numberEvents(),genData),rootFile);

    // Instantiate and initialize Module classes
    //  the order the modules are passed to the EventProcessor
    //  is th eorder the modules will run
    processor.addModule( new fc::TrackGenModule(config.debugLevel(),
                         config.numberTracks(),
                         "genTracks", //label used for tracks put into the event
                         detectorGeometry,random));
    processor.addModule( new fc::HitStripGenModule(config.debugLevel(),
                         "genTracks",//get these tracks
                         "genHits", //create these hits
                         "strips", //create these strips
                         detectorGeometry,random) );
    processor.addModule( new fc::DataOutputModule(config.debugLevel(),
                         genoutputeventdatafile,
                         "genTracks", //get these tracks
                         "genHits", //get these hits
                         "strips", //get these strips
                         detectorGeometry));

    // Event loop over module classes
    processor.processEvents(config);

    processor.endJob();
    return 0;

}
