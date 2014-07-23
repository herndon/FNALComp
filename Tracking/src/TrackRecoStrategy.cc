#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include "Tracking/include/TrackRecoStrategy.hh"
#include "DataObjects/include/Track.hh"

fc::TrackRecoStrategy::TrackRecoStrategy(int debugLevel,const DetectorGeometry detectorGeometry):

  _debugLevel(debugLevel),
  _detectorGeometry(detectorGeometry),
  _minPTCut(1.0) {
}


void fc::TrackRecoStrategy::recoTracks(TrackSet & recoTrackSet, const HitSet& recoHitSet){

  trackSet trackCandidateSet;

  findTrackCandidates(trackCandidateSet, recoHitSet);

  findTracks(trackCandidateSet,recoHitSet);

  finalTrackEvaluation(recoTrackSet, trackCandidateSet);

}

void fc::TrackRecoStrategy::findTrackCandidates(trackSet& trackCandidateSet, const HitSet& recoHitSet){

  std::vector<trackHitMap> trackHitCandidates;

  findHitCadidates(trackHitCandidates,recoHitSet);

  buildTrackCandidates(trackCandidateSet,trackHitCandidates,recoHitSet);
  

}


void fc::TrackRecoStrategy::buildTrackCandidates(fc::trackSet& trackCandidateSet,std::vector<trackHitMap>& trackHitCandidates,const HitSet& hitSet){

  // !!!!! repalce with build track call
  TVector3 primaryVertex(0.0,0.0,0.0);
  // !!!!! until Track constructor is updated
  std::vector<int> temp;
  for (std::vector<trackHitMap>::const_iterator trackHitCandidateIter = trackHitCandidates.begin(); trackHitCandidateIter != trackHitCandidates.end(); ++trackHitCandidateIter){
    //Track trackCandidate(hitSet,temp,primaryVertex,_detectorGeometry,_debugLevel);
    //trackCandidateSet.push_back(trackCandidate);
  }

}

void  fc::TrackRecoStrategy::findHitsOnLayer(fc::trackSet & trackCandidateSet,const HitSet & recoHitSet,int layer){

}

void fc::TrackRecoStrategy::intermediateTrackEvalutation(fc::trackSet & trackCandidateSet){



}

void fc::TrackRecoStrategy::finalTrackEvaluation(TrackSet & trackSet, fc::trackSet & trackCandidateSet){



}

