#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include<cmath>
#include "DetectorGeometry.hh"
#include "Track.hh"
#include "TrackSet.hh"
#include "TrackGenModule.hh"

fc::TrackGenModule::TrackGenModule(int debugLevel, const DetectorGeometry & myDetectorGeometry, Random & myRandom):
  _debugLevel(debugLevel),
  _myDetectorGeometry(myDetectorGeometry),
  _myRandom(myRandom) {

  // Intialize commonly used DetectorGeometry data
  _curvatureC = _myDetectorGeometry.getCurvatureC();

}

void fc::TrackGenModule::processEvent(TrackSet& myTrackSet,int numberTracks)
{

  for (int ii_track = 0; ii_track < numberTracks; ++ii_track) {

    Track track = generateTrack(); 
    myTrackSet.insertTrack(ii_track,track);

  } // end track loop

}



fc::Track fc::TrackGenModule::generateTrack(){

  // Generate track data
    
  // Track pT, phi0 and charge
  double trackPT = _myRandom.getUniformDouble(10.0,20.0);
  int trackCharge = (_myRandom.getUniformDouble(0.0,1.0) > 0.5) ? 1 : -1;
  double trackPhi0 = _myRandom.getUniformDouble(-M_PI/6.0,M_PI/6.0) + M_PI/2.0;
 
  // Details of curvature calculation
  //using p = BqR, 
  //multiply by c to get energy in J  pc = BqRc, 
  //divide by e to get energy in eV pc = BqR3x108/1.6x10-19
  // q = Q1.6x10-19 where Q = 1, -1...
  //divide by 109 pc (in GeV) = BQR3x108/(1x10^9)

  double trackCurvature = _curvatureC/trackPT;


  if (_debugLevel >=5 ) {
    std::cout << "Track pT " << trackPT << std::endl;
    std::cout << "Track Charge " << trackCharge << std::endl;
    std::cout << "Track phi0 " << trackPhi0 << std::endl;
    std::cout << "Track radius curvature " << 1/trackCurvature << std::endl;
  }
 
  

  double vertex[3]{0.0,0.0,0.0};
  double bField[3]{_myDetectorGeometry.getBField()[0],_myDetectorGeometry.getBField()[1],_myDetectorGeometry.getBField()[2]};

  Track track(trackPT,0.0,trackCharge,trackPhi0,0.0,0.0,vertex,bField,_curvatureC);
 
  return track;

}




