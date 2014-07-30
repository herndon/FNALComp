#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include <vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "Algorithms/include/TrackFitMeasurements.hh"
#include "Tracking/include/TrackingFunctions.hh"
#include "Tracking/include/TrackingFilters.hh"
#include "Tracking/include/TrackRecoStrategy.hh"

fc::TrackRecoStrategy::TrackRecoStrategy(int debugLevel,const DetectorGeometry & detectorGeometry)
  :_debugLevel(debugLevel),
   _detectorGeometry(detectorGeometry){
}



void fc::TrackRecoStrategy::recoTracks(trackSet & trackCandidateSet, const HitSet& recoHitSet) const{


  // !!!!! trackCandidateSet type def to a track list since we are doing may insertions and deletions?
 
  findTracks(trackCandidateSet,recoHitSet);

  fcf::contentionTrackSetFilter(trackCandidateSet);


}

