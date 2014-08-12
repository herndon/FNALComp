#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include <vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "Tracking/include/LayerTrackFinder.hh"
#include "Services/include/Config.hh"
#include "Tracking/include/TrackingFilters.hh"
#include "Tracking/include/TrackRecoStrategy2X1SAS.hh"

fc::TrackRecoStrategy2X1SAS::TrackRecoStrategy2X1SAS(int debugLevel,
        const DetectorGeometry& detectorGeometry,const Config& config):
    _debugLevel(debugLevel),
    _detectorGeometry(detectorGeometry),
    _minPTCut(config.getMinPTCut()),
    _dRCut(config.dRCut()),
    _dZCut(config.dZCut()),
    _maxChi2NDofCut(config.getMaxChi2NDofCut()) {
}

void fc::TrackRecoStrategy2X1SAS::recoTracks(const HitSet& recoHitSet,
        TrackSetContainer& trackSet) const {


    // !!!!! trackCandidateSet type def to a track list since we are doing may insertions and deletions?

    findTracks(recoHitSet,trackSet);

    int numberHitsForMatching =5;
    fcf::contentionTrackSetFilter(trackSet,numberHitsForMatching);


}


void fc::TrackRecoStrategy2X1SAS::findTracks(const HitSet & recoHitSet,
        TrackSetContainer& trackSet) const {

  std::vector<int> layers {9,4,8,3,2,1,0,7,6,5};
    unsigned int expNHit = 0;

    for (auto layer : layers) {

        LayerTrackFinder layerTrackFinder(_debugLevel,_detectorGeometry,layer,expNHit,
                                          _minPTCut,_dRCut,_dZCut,_maxChi2NDofCut);
  
	layerTrackFinder.findTracks(recoHitSet,expNHit,trackSet);

        ++expNHit;
    }

}
