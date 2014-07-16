#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include<cmath>
#include "DetectorGeometry.hh"
#include "Track.hh"
#include "TrackSet.hh"
#include "TrackGenModule.hh"
#include "TH1D.h"
#include "TVector3.h"
#include "UniqueRootDirectory.hh"

fc::TrackGenModule::TrackGenModule(int debugLevel, int numberOfTracks, const std::string& iTracksLabel, const DetectorGeometry & detectorGeometry, Random & random):
  _debugLevel(debugLevel),
  _numberOfTracks(numberOfTracks),
  _tracksLabel(iTracksLabel),
  _detectorGeometry(detectorGeometry),
  _random(random),
  _hPt(nullptr)
{

  // Intialize commonly used DetectorGeometry data
  _curvatureC = _detectorGeometry.getCurvatureC();

  UniqueRootDirectory tDir( "TrackGen" );
  _hPt = new TH1D ( "Pt", "Pt of Generated Track; GeV; N", 100, 0., 100.);

}

void fc::TrackGenModule::processEvent(fc::Event& event)
{
  std::unique_ptr<TrackSet> genTrackSet{ new TrackSet(event.eventNumber(),
						     *(event.get<bool>("genData")),
						     _detectorGeometry) };

  for (int ii_track = 0; ii_track < _numberOfTracks; ++ii_track) {

    Track track = generateTrack(); 
    genTrackSet->insertTrack(track);

  } // end track loop
  event.put(_tracksLabel,std::move(genTrackSet));
}



fc::Track fc::TrackGenModule::generateTrack(){

  // Generate track data
    
  // Track pT, phi0 and charge
  double trackPT = _random.getUniformDouble(20.0,40.0);
  _hPt->Fill(trackPT);
  int trackCharge = (_random.getUniformDouble(0.0,1.0) > 0.5) ? 1 : -1;
  double trackPhi0 = _random.getUniformDouble(-M_PI/24.0,M_PI/24.0) + M_PI/2.0;
 
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

 
  Track track(trackCharge/trackPT,0.0,0.0,trackPhi0,0.0,_detectorGeometry);

 
  return track;

}




