#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include "Tracking/include/TrackCandidateStrategy.hh"
#include "DataObjects/include/Track.hh"

fc::TrackCandidateStrategy::TrackCandidateStrategy(int debugLevel,const DetectorGeometry detectorGeometry):

  _debugLevel(debugLevel),
  _detectorGeometry(detectorGeometry),
  _minCandPTCut(1.0) {
}



void fc::TrackCandidateStrategy::findTrackCandidates(TrackSet& trackCandidateSet, const HitSet& recoHitSet){

  std::vector<trackHitSet> trackHitCandidates;

  findHitCadidates(trackHitCandidates,recoHitSet);

  buildTrackCandidates(trackCandidateSet,trackHitCandidates,recoHitSet);
  

}


void fc::TrackCandidateStrategy::buildTrackCandidates(fc::TrackSet& trackCandidateSet,std::vector<trackHitSet>& trackHitCandidates,const HitSet& hitSet){

  // !!!!! repalce with build track call
  TVector3 primaryVertex(0.0,0.0,0.0);
  // !!!!! until Track constructor is updated
  std::vector<int> temp;
  for (std::vector<trackHitSet>::const_iterator trackHitCandidateIter = trackHitCandidates.begin(); trackHitCandidateIter != trackHitCandidates.end(); ++trackHitCandidateIter){
    //Track trackCandidate(hitSet,temp,primaryVertex,_detectorGeometry,_debugLevel);
    //trackCandidateSet.push_back(trackCandidate);
  }

}

