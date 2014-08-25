#include "DataObjects/include/Track.hh"
#include "DataObjects/include/TrackSet.hh"
#include "Tracking/include/TrackingFunctions.hh"
#include <iostream>

unsigned int fcf::numberMatchedHits(const fc::Track& track1, const fc::Track& track2 ) {

    unsigned int matchedHits = 0;
    for (auto const& hit : track1.trackHits()) {

        for (auto const& hit2 :  track2.trackHits()) {

            if (hit == hit2) ++matchedHits;

        }

    }

    return matchedHits;
}


bool fcf::betterOverlappingTrack(const fc::Track& track0,
                                 const fc::Track& track1) {

    if ((track0.trackHits().size() == track1.trackHits().size()
            && track0.chi2()/track0.nDof()> track1.chi2()/track1.nDof())||
            track0.trackHits().size() < track1.trackHits().size()) {
        return true;
    } else if ((track1.trackHits().size() == track0.trackHits().size()
                && track1.chi2()/track1.nDof()> track0.chi2()/track0.nDof())||
               track1.trackHits().size() < track0.trackHits().size()) {
        return false;
    }
    return false;
}
