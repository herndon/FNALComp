#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Tracking/include/TrackingFunctions.hh"
#include "Tracking/include/TrackingFilters.hh"

void fcf::simpleTrackSetFilter(const fc::DetectorGeometry& detectorGeometry,const TrackingSelector& trackSelector,fc::TrackSetContainer & trackCandidateSet){

  // Right now just checks the number of expected hits

  //moves tracks we do not want to the end of the container based on the goodCandidateTrack predicate and then we erase them
  auto newEndItr = std::remove_if( trackCandidateSet.begin(),  trackCandidateSet.end(),
				   [detectorGeometry,trackSelector](fc::Track const& track) { return !goodCandidateTrack(track,detectorGeometry,trackSelector); } );
  trackCandidateSet.erase(newEndItr, trackCandidateSet.end());



}


void fcf::contentionTrackSetFilter(fc::TrackSetContainer & trackCandidateSet){

  // !!!!! this may not be safe and needs to be revisited

  for (fc::TrackSetContainer::iterator trackIter = trackCandidateSet.begin(); trackIter != trackCandidateSet.end(); ++trackIter){
 
    fc::TrackSetContainer::iterator trackIter2 = trackIter;
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
