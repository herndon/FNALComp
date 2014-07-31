#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include <vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "Tracking/include/LayerTrackFinder.hh"
#include "Tracking/include/TrackingFilters.hh"
#include "Tracking/include/TrackRecoStrategy2X1SAS.hh"

fc::TrackRecoStrategy2X1SAS::TrackRecoStrategy2X1SAS(int debugLevel,const DetectorGeometry& detectorGeometry,double minPTCut,double maxChi2NDofCut):
  _debugLevel(debugLevel),
  _detectorGeometry(detectorGeometry),
  _minPTCut(minPTCut),
  _maxChi2NDofCut(maxChi2NDofCut) {
}

void fc::TrackRecoStrategy2X1SAS::recoTracks(trackSet & trackCandidateSet, const HitSet& recoHitSet) const{


  // !!!!! trackCandidateSet type def to a track list since we are doing may insertions and deletions?
 
  findTracks(trackCandidateSet,recoHitSet);

  fcf::contentionTrackSetFilter(trackCandidateSet);


}


void fc::TrackRecoStrategy2X1SAS::findTracks(fc::trackSet& trackCandidateSet,const HitSet & recoHitSet) const{

  std::vector<int> layers {8,2,1,0,7,6,5};
  int expNHit = 3;

  for (std::vector<int>::const_iterator layerIter = layers.begin();layerIter!=layers.end();++layerIter,++expNHit){
    LayerTrackFinder layerTrackFinder(_debugLevel,_detectorGeometry,*layerIter,expNHit,_minPTCut,_maxChi2NDofCut);
    layerTrackFinder.findCandidateTracks(trackCandidateSet,recoHitSet,expNHit);
  }

}
