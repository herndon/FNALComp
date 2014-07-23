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


  // !!!!! type def a track list since we are doing may insertions and deletions
  trackSet trackCandidateSet;

  findTracks(trackCandidateSet,recoHitSet);

  finalTrackEvaluation(recoTrackSet, trackCandidateSet);

}




void  fc::TrackRecoStrategy::findHitsOnLayer(fc::trackSet & trackCandidateSet,const HitSet & recoHitSet,int layer){

}

void fc::TrackRecoStrategy::intermediateTrackEvalutation(fc::trackSet & trackCandidateSet){



}

void fc::TrackRecoStrategy::finalTrackEvaluation(TrackSet & trackSet, fc::trackSet & trackCandidateSet){



}

