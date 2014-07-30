#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Tracking/include/TrackingFunctions.hh"
#include "Tracking/include/TrackingFilters.hh"

void fcf::simpleTrackSetFilter(fc::trackSet & trackCandidateSet,const fc::DetectorGeometry& detectorGeometry,const TrackingSelector& trackSelector ){

  // Right now just checks the number of expected hits

  for (fc::trackSet::iterator trackIter = trackCandidateSet.begin(); trackIter != trackCandidateSet.end(); ++trackIter){

    if (!goodCandidateTrack(*trackIter,detectorGeometry,trackSelector)) {
      trackCandidateSet.erase(trackIter);
      trackIter--;
    }
  }
}


void fcf::contentionTrackSetFilter(fc::trackSet & trackCandidateSet){


  for (fc::trackSet::iterator trackIter = trackCandidateSet.begin(); trackIter != trackCandidateSet.end(); ++trackIter){
 
    fc::trackSet::iterator trackIter2 = trackIter;
    ++trackIter2;

    for ( ; trackIter2 != trackCandidateSet.end(); ++trackIter2){

      // Find number of matching hits
      int matchedHits = fcf::numberMatchedHits(*trackIter,*trackIter2); 

      // Remove tracks with less hits or with a worse chi2/ndof

      if (matchedHits>4) {

	if (fcf::betterOverlappingTrack(*trackIter,*trackIter2)){
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
