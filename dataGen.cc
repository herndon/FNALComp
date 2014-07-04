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
#include "HitSet.hh"
#include "TrackSet.hh"
#include "StripSet.hh"
#include "TrackGenModule.hh"
#include "HitStripGenModule.hh"
#include "DataOutputModule.hh"
#include "Config.hh"
#include "Random.hh"
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

  // DetectorGeomergy
  std::ifstream detectorgeometryfile("sensorgeometry.txt");
  fc::DetectorGeometry myDetectorGeometry(detectorgeometryfile);  
  // files are closed by the default destructor
  if (debugLevel >= 2) myDetectorGeometry.printDetectorGeometry();

  // Input and output files and modules
  std::ofstream genoutputeventdatafile("genoutputeventdatafile.bin",std::ios::binary);


 
  // Instantiate and initialize Module classes
  fc::TrackGenModule myTrackGenModule(debugLevel,myDetectorGeometry,myRandom);
  fc::HitStripGenModule myHitStripGenModule(debugLevel,myDetectorGeometry,myRandom);
  fc::DataOutputModule myDataOutputModule(debugLevel,myDetectorGeometry,genoutputeventdatafile);


   // Event loop over module classes

  for (int ii_event = 0; ii_event < myConfig.getNumberEvents(); ++ii_event) {

    // Initialize object persistent only for each event
    fc::TrackSet myTrackSet(ii_event,genData,myDetectorGeometry);
    fc::HitSet myHitSet(ii_event,genData);
    fc::StripSet myStripSet(genData);

    myTrackGenModule.processEvent(myTrackSet,myConfig.getNumberTracks());
    myHitStripGenModule.processEvent(myTrackSet,myHitSet,myStripSet);
    myDataOutputModule.processEvent(ii_event,myTrackSet,myHitSet,myStripSet);

  }


  return 0; 

}
