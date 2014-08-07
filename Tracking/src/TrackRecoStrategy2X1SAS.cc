#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include <vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "Tracking/include/LayerTrackFinder.hh"
#include "Tracking/include/TrackingFilters.hh"
#include "Tracking/include/TrackRecoStrategy2X1SAS.hh"

fc::TrackRecoStrategy2X1SAS::TrackRecoStrategy2X1SAS(int debugLevel,
        const DetectorGeometry& detectorGeometry,double minPTCut,double maxChi2NDofCut):
    _debugLevel(debugLevel),
    _detectorGeometry(detectorGeometry),
    _minPTCut(minPTCut),
    _maxChi2NDofCut(maxChi2NDofCut) {
}

void fc::TrackRecoStrategy2X1SAS::recoTracks(const HitSet& recoHitSet,
        TrackSetContainer& trackCandidateSet) const {


    // !!!!! trackCandidateSet type def to a track list since we are doing may insertions and deletions?

    findTracks(recoHitSet,trackCandidateSet);

    fcf::contentionTrackSetFilter(trackCandidateSet,5);


}


void fc::TrackRecoStrategy2X1SAS::findTracks(const HitSet & recoHitSet,
					     TrackSetContainer& trackCandidateSet) const {

  std::vector<int> layers {4,9,3,8,2,7,1,6,0,5};
  unsigned int expNHit = 0;

  for (auto layer : layers) {

    //if (layer==3) --expNHit;
    //if(layer==7) ++expNHit;
    LayerTrackFinder layerTrackFinder(_debugLevel,_detectorGeometry,layer,expNHit,
				      _minPTCut,_maxChi2NDofCut);
  
    layerTrackFinder.findCandidateTracks(recoHitSet,expNHit,trackCandidateSet);
    //std::cout << "findTracks layer" << layer << std::endl; 
    //iffcf::duplicateTrackSetFilter(trackCandidateSet);
    ++expNHit;
  }

}
