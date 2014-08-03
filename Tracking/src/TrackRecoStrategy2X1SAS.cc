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

    fcf::contentionTrackSetFilter(trackCandidateSet);


}


void fc::TrackRecoStrategy2X1SAS::findTracks(const HitSet & recoHitSet,
        TrackSetContainer& trackCandidateSet) const {

    std::vector<int> layers {8,2,1,0,7,6,5};
    int expNHit = 3;

    for (auto layer : layers) {
        LayerTrackFinder layerTrackFinder(_debugLevel,_detectorGeometry,layer,expNHit,
                                          _minPTCut,_maxChi2NDofCut);
        layerTrackFinder.findCandidateTracks(recoHitSet,expNHit,trackCandidateSet);
        ++expNHit;
    }

}
