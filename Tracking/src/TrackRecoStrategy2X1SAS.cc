#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include <vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "Tracking/include/LayerTrackFinder.hh"
#include "Tracking/include/TrackRecoStrategy2X1SAS.hh"

fc::TrackRecoStrategy2X1SAS::TrackRecoStrategy2X1SAS(int debugLevel,const DetectorGeometry& detectorGeometry,double minPTCut,double maxChi2NDofCut):
  TrackRecoStrategy(debugLevel,detectorGeometry),
  _minPTCut(minPTCut),
  _maxChi2NDofCut(maxChi2NDofCut) {
}


void fc::TrackRecoStrategy2X1SAS::findTracks(fc::trackSet& trackCandidateSet,const HitSet & recoHitSet) const{

  std::vector<int> layers {8,2,1,0,7,6,5};
  int expNHit = 3;

  for (std::vector<int>::const_iterator layerIter = layers.begin();layerIter!=layers.end();++layerIter,++expNHit){
    LayerTrackFinder layerTrackFinder(getDebugLevel(),getDetectorGeometry(),*layerIter,expNHit,_minPTCut,_maxChi2NDofCut);
    layerTrackFinder.findCandidateTracks(trackCandidateSet,recoHitSet,expNHit);
  }

}
