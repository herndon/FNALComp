#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include<cmath>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Modules/include/TrackGenModule.hh"
#include "TH1D.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "Services/include/UniqueRootDirectory.hh"


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
  double trackTanL = _random.getUniformDouble(-0.1,0.1); 
  double trackD0 = _random.getUniformDouble(-0.01,0.01); 
  double trackZ0 = _random.getUniformDouble(-0.01,0.01); 

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

 
  Track track(trackCharge/trackPT,trackD0,trackZ0,trackPhi0,trackTanL,_detectorGeometry);
  TLorentzVector lorentzVector(trackPT*std::cos(trackPhi0),trackPT*std::sin(trackPhi0),trackTanL*trackPT,trackPT*std::sqrt(1+trackTanL*trackTanL));
  TVector3 position(trackD0*std::cos(trackPhi0-M_PI/2.0),trackD0*std::sin(trackPhi0-M_PI/2.0),trackZ0);
  GenTrack genTrack(lorentzVector,trackCharge,position);

  std::cout << "Compare track and genTrack" << std::endl;
  track.print();
  genTrack.print();

  return track;

}




