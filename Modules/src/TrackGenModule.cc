#define _USE_MATH_DEFINES
#include <string>
#include<stdlib.h>
#include<cmath>
#include "Services/include/Random.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/GenTrack.hh"
#include "DataObjects/include/GenTrackSet.hh"
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


  UniqueRootDirectory tDir( "TrackGen" );
  _hPt = new TH1D( "Pt", "Pt of Generated Track; GeV; N", 100, 0., 100.);

}

void fc::TrackGenModule::processEvent(fc::Event& event)
{

  std::unique_ptr<GenTrackSet> genTrackSet{ new GenTrackSet() };

  for (int ii_track = 0; ii_track < _numberOfTracks; ++ii_track) {

    GenTrack genTrack = generateTrack(); 
    genTrackSet->insertTrack(genTrack);

  } // end track loop
  event.put(_tracksLabel,std::move(genTrackSet));
}



fc::GenTrack fc::TrackGenModule::generateTrack(){

  // Generate track data
    
  // Track pT, phi0 and charge
  double trackPT = _random.getUniformDouble(20.0,40.0);
  _hPt->Fill(trackPT);
  int trackCharge = (_random.getUniformDouble(0.0,1.0) > 0.5) ? 1 : -1;
  double trackPhi0 = _random.getUniformDouble(-M_PI/24.0,M_PI/24.0) + M_PI/2.0;
  double trackTanL = _random.getUniformDouble(-0.1,0.1); 
  double trackD0 = _random.getUniformDouble(-0.01,0.01); 
  double trackZ0 = _random.getUniformDouble(-0.01,0.01); 


  if (_debugLevel >=5 ) {
    std::cout << "Track pT " << trackPT << std::endl;
    std::cout << "Track Charge " << trackCharge << std::endl;
    std::cout << "Track phi0 " << trackPhi0 << std::endl;
  }
 
  double phi0ToD0 = 0.0;
  if ( trackPhi0>=M_PI/2.0) phi0ToD0 = trackPhi0 - M_PI/2.0;
  if ( trackPhi0<M_PI/2.0) phi0ToD0  = trackPhi0 + M_PI/2.0;

  TLorentzVector lorentzVector(trackPT*std::cos(trackPhi0),trackPT*std::sin(trackPhi0),trackTanL*trackPT,trackPT*std::sqrt(1+trackTanL*trackTanL));
  TVector3 position(trackD0*std::cos(phi0ToD0),trackD0*std::sin(phi0ToD0),trackZ0);
  GenTrack genTrack(lorentzVector,trackCharge,position);

  return genTrack;

}




