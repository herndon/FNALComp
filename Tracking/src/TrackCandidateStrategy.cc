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

  for (auto const& trackHitCandidate: trackHitCandidates) {
    trackCandidateSet.push_back(buildTrack(hitSet,trackHitCandidate,_detectorGeometry,_debugLevel));
  }
}

bool fc::TrackCandidateStrategy::goodCandidate(const Helix & helix) const {

  return (helix.getPT(_detectorGeometry.getBField()) >= _minCandPTCut);

}
