#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Tracking/include/TrackingFunctions.hh"
#include "Tracking/include/TrackingFilters.hh"
#include <set>
#include <iostream>

void fcf::simpleTrackSetFilter(const fc::DetectorGeometry& detectorGeometry,
                               const TrackingSelector& trackSelector,
                               fc::FastTrackSetContainer & trackSet) {

    // Right now just checks the number of expected hits

    //moves tracks we do not want to the end of the container based on the goodCandidateTrack predicate and then we erase them
    auto newEndItr = std::remove_if( trackSet.begin(),
                                     trackSet.end(),
    [detectorGeometry,trackSelector](fc::Track const& track) {
        return !goodTrack(track,detectorGeometry,trackSelector);
    } );
    trackSet.erase(newEndItr, trackSet.end());



}


void fcf::duplicateTrackSetFilter(fc::FastTrackSetContainer & trackCandidateSet) {


  std::set<int> deleteSet;

  int trackNumber=0;
  int trackNumber2=0;

  for (fc::FastTrackSetContainer::iterator trackIter = trackCandidateSet.begin();
       trackIter != trackCandidateSet.end(); ++trackIter,++trackNumber) {

    fc::FastTrackSetContainer::iterator trackIter2 = trackIter;
    trackNumber2 = trackNumber+1; 
    ++trackIter2;

    for ( ; trackIter2 != trackCandidateSet.end(); ++trackIter2,++trackNumber2) {

      // Find number of matching hits
      unsigned int matchedHits = fcf::numberMatchedHits(*trackIter,*trackIter2);
      if (matchedHits == trackIter->getHits().size() && matchedHits == trackIter2->getHits().size()) deleteSet.insert(trackNumber2);

    }
  }

  //std::cout << "Remove matched tracks " << deleteSet.size() << std::endl;
  int ii=0;
    auto newEndItr = std::remove_if( trackCandidateSet.begin(),
                                     trackCandidateSet.end(),
				     [&ii,deleteSet](fc::Track const& track) {
                                       ++ii;
				       return deleteSet.find(ii-1) != deleteSet.end() ;
    } );
    trackCandidateSet.erase(newEndItr, trackCandidateSet.end());

}



void fcf::contentionTrackSetFilter(fc::FastTrackSetContainer & trackCandidateSet,unsigned int numberToMatch) {

  // !!!!! this may not be safe and needs to be revisited

  std::set<int> deleteSet;

  int trackNumber=0;
  int trackNumber2=0;

for (fc::FastTrackSetContainer::iterator trackIter = trackCandidateSet.begin();
       trackIter != trackCandidateSet.end(); ++trackIter,++trackNumber) {

    fc::FastTrackSetContainer::iterator trackIter2 = trackIter;
    trackNumber2 = trackNumber+1; 
    ++trackIter2;

    for ( ; trackIter2 != trackCandidateSet.end(); ++trackIter2,++trackNumber2) {

      // Find number of matching hits
      unsigned int matchedHits = fcf::numberMatchedHits(*trackIter,*trackIter2);

      // Remove tracks with less hits or with a worse chi2/ndof

      if (matchedHits>=numberToMatch) {

	if (fcf::betterOverlappingTrack(*trackIter,*trackIter2)) {
	  deleteSet.insert(trackNumber);

	  //trackIter--;
	  break;
	} else {
	  deleteSet.insert(trackNumber2);
	}

      }
    }

  }

  int ii=0;
    auto newEndItr = std::remove_if( trackCandidateSet.begin(),
                                     trackCandidateSet.end(),
				     [&ii,deleteSet](fc::Track const& track) {
                                       ++ii;
				       return deleteSet.find(ii-1) != deleteSet.end() ;
    } );
    trackCandidateSet.erase(newEndItr, trackCandidateSet.end());

}

std::vector<int> fcf::bestTracksFilter(const fc::FastTrackSetContainer & tracks) {

    std::vector<int> trackList;

    // keep the two best and all before the fit is constrined
    double bestChi2 = 999.0;
    double secondBestChi2 = 999.0;
    int bestTrack = -1;
    int secondBestTrack = -1;
    int trackNumber = 0;


    for (auto const& track : tracks) {
        if (track.getNDof() > 0 && track.getChi2() < bestChi2) {
            secondBestChi2 = bestChi2;
            secondBestTrack = bestTrack;
            bestChi2 = track.getChi2();
            bestTrack = trackNumber;
        } else if (track.getNDof() > 0 && track.getChi2() < secondBestChi2) {
            secondBestChi2 = track.getChi2();
            secondBestTrack = trackNumber;
        }
        ++trackNumber;
    }

    trackNumber = 0;

    // Keep all if not yet constrainted since we can't compare chi2/ndof
    for (auto const& track : tracks) {
        if (track.getNDof() <= 0) trackList.push_back(trackNumber);
        ++trackNumber;
    }

    // Tested keeping all or just the best track.  Keeping top 2 works best
    if (bestTrack > -1)  trackList.push_back(bestTrack);
    if (secondBestTrack > -1)  trackList.push_back(secondBestTrack);





    return trackList;

}
