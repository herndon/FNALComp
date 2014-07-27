#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include "Geometry/include/DetectorGeometry.hh"
#include "Tracking/include/TrackCandidateStrategy.hh"
#include "Algorithms/include/BuildTrack.hh"

fc::TrackCandidateStrategy::TrackCandidateStrategy(int debugLevel,const DetectorGeometry& detectorGeometry, double minCandPtCut):
  _debugLevel(debugLevel),
  _detectorGeometry(detectorGeometry),
  _minCandPTCut(minCandPtCut) {
}

void fc::TrackCandidateStrategy::findTrackCandidates(trackSet& trackCandidateSet, const HitSet& recoHitSet) const{

  std::vector<trackHitSet> trackHitCandidates;

  findHitCadidates(trackHitCandidates,recoHitSet);

  buildTrackCandidates(trackCandidateSet,trackHitCandidates,recoHitSet);
  

}


void fc::TrackCandidateStrategy::buildTrackCandidates(trackSet& trackCandidateSet,std::vector<trackHitSet>& trackHitCandidates,const HitSet& hitSet) const {

  for (std::vector<trackHitSet>::const_iterator trackHitCandidateIter = trackHitCandidates.begin(); trackHitCandidateIter != trackHitCandidates.end(); ++trackHitCandidateIter){
    Track trackCandidate(BuildTrack(hitSet,*trackHitCandidateIter,_detectorGeometry,_debugLevel));
    trackCandidateSet.push_back(trackCandidate);
  }

}

bool fc::TrackCandidateStrategy::goodCandidate(const Helix & helix) const {

  return (helix.getPT(_detectorGeometry.getBField()) >= _minCandPTCut);

}
