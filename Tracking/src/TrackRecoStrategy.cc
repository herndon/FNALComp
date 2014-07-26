#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include <vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "Algorithms/include/TrackFitMeasurements.hh"
#include "Tracking/include/TrackRecoStrategy.hh"

fc::TrackRecoStrategy::TrackRecoStrategy(int debugLevel,const DetectorGeometry & detectorGeometry)
  :_debugLevel(debugLevel),
   _detectorGeometry(detectorGeometry){
}



void fc::TrackRecoStrategy::recoTracks(trackSet & trackCandidateSet, const HitSet& recoHitSet) const{


  // !!!!! trackCandidateSet type def to a track list since we are doing may insertions and deletions?
 
  findTracks(trackCandidateSet,recoHitSet);

  finalTrackFilter(trackCandidateSet);

}


void fc::TrackRecoStrategy::finalTrackFilter(fc::trackSet & trackCandidateSet) const{


  for (trackSet::iterator trackIter = trackCandidateSet.begin(); trackIter != trackCandidateSet.end(); ++trackIter){
 
    trackSet::iterator trackIter2 = trackIter;
    trackIter2++;

    for ( ; trackIter2 != trackCandidateSet.end(); ++trackIter2){

      // Find number of matching hits
      int matchedHits = numberMatchedHits(*trackIter,*trackIter2); 

      // Remove tracks with less hits or with a worse chi2/ndof

      if (matchedHits>4) {

	if (betterOverlappingTrack(*trackIter,*trackIter2)){
	  trackCandidateSet.erase(trackIter);
	  trackIter--;
	  break;
	}else{
	  trackCandidateSet.erase(trackIter2);
	  trackIter2--;
	}
 
      }
    }
  }
}

int fc::TrackRecoStrategy::numberMatchedHits(const Track& track1, const Track& track2 ) const{

  int matchedHits = 0;
  for (trackHitSet::const_iterator hitIter = track1.getHits().begin(); hitIter!=track1.getHits().end(); ++hitIter){
 
    for (trackHitSet::const_iterator hitIter2 =  track2.getHits().begin(); hitIter2!=track2.getHits().end(); ++hitIter2){
 
      if (*hitIter == *hitIter2) ++matchedHits;
    }

  }

  return matchedHits;

}


bool fc::TrackRecoStrategy::betterOverlappingTrack(const Track& track0, const Track& track1) const{

  if ((track0.getHits().size() == track1.getHits().size() && track0.getChi2()/track0.getNDof()> track1.getChi2()/track1.getNDof())||
      track0.getHits().size() < track1.getHits().size()) {
    return true;
  } else if ((track1.getHits().size() == track0.getHits().size() && track1.getChi2()/track1.getNDof()> track0.getChi2()/track0.getNDof())||
	     track1.getHits().size() < track0.getHits().size()){
    return false;
  } 
  std::cout << "Should not be here" << std::endl;
  return false;
}
