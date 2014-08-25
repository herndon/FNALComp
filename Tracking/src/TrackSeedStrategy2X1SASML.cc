#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/HitSet.hh"
#include "Algorithms/include/InitializeHelix.hh"
#include "Algorithms/include/BuildTrack.hh"
#include "Tracking/include/TrackingSelectors.hh"
#include "Tracking/include/TrackSeedStrategy2X1SASML.hh"


fc::TrackSeedStrategy2X1SASML::TrackSeedStrategy2X1SASML(int debugLevel,
        const DetectorGeometry& detectorGeometry,double minCandPTCut):
    _debugLevel(debugLevel),
    _detectorGeometry(detectorGeometry),
    _minCandPTCut(minCandPTCut) {
}


void fc::TrackSeedStrategy2X1SASML::findTrackSeeds(
    const HitSet& recoHitSet,FastTrackSetContainer& trackSeedSet) const {

    std::vector<TrackHitContainer> trackHitSeeds;

    findHitCadidates(recoHitSet,trackHitSeeds);

    for (auto const& trackHitSeed: trackHitSeeds) {
        trackSeedSet.push_back(std::move(buildTrack(recoHitSet,trackHitSeed,
                                              _detectorGeometry,_debugLevel)));
    }

    // We could filter the candidates at this point

}



void fc::TrackSeedStrategy2X1SASML::findHitCadidates(const HitSet& hitSet,
							  std::vector<fc::TrackHitContainer>& trackHitSeeds) const {

  fcf::TrackingSelector trackSelector = {_minCandPTCut,0,1000000.0,false,false};


  std::vector<std::vector<int>> hitPatterns;

  hitPatterns.emplace_back(std::vector<int>{4,3,9});
  hitPatterns.emplace_back(std::vector<int>{3,4,8});
  hitPatterns.emplace_back(std::vector<int>{4,1,9});
  hitPatterns.emplace_back(std::vector<int>{3,1,8});
 

  for (auto pattern : hitPatterns) {
    //trackSelector._minPTCut = _minCandPTCut;

    int hitNumberO = 0;

  // Form hitPatern combinations

  for (auto const& hitO : hitSet.hits()) {

    if (hitO.layer() == pattern[0]) {

      int hitNumberI = 0;

      for (auto const& hitI : hitSet.hits()) {

	if (hitI.layer() == pattern[1]) {

	  int hitNumberOSAS = 0;

	  for (auto const&  hitOSAS : hitSet.hits()) {

	    if (hitOSAS.layer() == pattern[2]) {

	      TVector3 zIntersection;
	      bool goodIntersection = intersectStrips(hitO,hitOSAS,_detectorGeometry,
						      zIntersection);
		//intersectStrips(hitI,hitOSAS,_detectorGeometry,
		//		zIntersection);
	      if (goodIntersection) {

		TVector3 primaryVertex(0.0,0.0,0.0);
		Helix helix = initializeHelix(primaryVertex,hitO.position(),
					      hitI.position(),zIntersection,_detectorGeometry);
		if (fcf::goodSeedHelix(helix,_detectorGeometry,trackSelector)) {
		  //avoids a copy twice
		  trackHitSeeds.emplace_back(std::vector<int> {hitNumberO,hitNumberI,hitNumberOSAS});
		}
	      }

	    }
	    ++hitNumberOSAS;
	  }

	}
	++hitNumberI;
      }

    }
    ++hitNumberO;
  }
  }

}


