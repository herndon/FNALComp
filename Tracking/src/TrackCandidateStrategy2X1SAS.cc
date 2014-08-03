#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/HitSet.hh"
#include "Algorithms/include/InitializeHelix.hh"
#include "Algorithms/include/BuildTrack.hh"
#include "Tracking/include/TrackingSelectors.hh"
#include "Tracking/include/TrackCandidateStrategy2X1SAS.hh"


fc::TrackCandidateStrategy2X1SAS::TrackCandidateStrategy2X1SAS(int debugLevel,const DetectorGeometry& detectorGeometry,double minCandPTCut):
  _debugLevel(debugLevel),
  _detectorGeometry(detectorGeometry),
  _minCandPTCut(minCandPTCut) {
}


void fc::TrackCandidateStrategy2X1SAS::findTrackCandidates(const HitSet& recoHitSet,TrackSetContainer& trackCandidateSet) const{

  std::vector<trackHitSet> trackHitCandidates;

  findHitCadidates(recoHitSet,trackHitCandidates);

  for (auto const& trackHitCandidate: trackHitCandidates) {
    trackCandidateSet.push_back(std::move(buildTrack(recoHitSet,trackHitCandidate,_detectorGeometry,_debugLevel)));
  }

  // We could filter the candidates at this point

}


// !!!!! Could improve to use more than outer layers
// !!!!! control seed layers from config
void fc::TrackCandidateStrategy2X1SAS::findHitCadidates(const HitSet& hitSet,std::vector<fc::trackHitSet>& trackHitCandidates) const{
 
  fcf::TrackingSelector trackSelector;
  trackSelector._minPTCut = _minCandPTCut;

  int hitNumberO = 0;

  // Form 4-3,9 hit candidates
  for (auto const& hitO : hitSet.getHits()) {

    if (hitO.getLayer() == 4) {

      int hitNumberI = 0;
 
      for (auto const& hitI : hitSet.getHits()) {

	if (hitI.getLayer() == 3) {

	  int hitNumberOSAS = 0;

	  for (auto const&  hitOSAS : hitSet.getHits()) {

	    if (hitOSAS.getLayer() == 9) {

	      TVector3 zIntersection;
	      bool goodIntersection = intersectStrips(hitO,hitOSAS,_detectorGeometry,zIntersection);
	      if (goodIntersection) {

                TVector3 primaryVertex(0.0,0.0,0.0);
		Helix helix = initializeHelix(primaryVertex,hitO.getHitPosition(),hitI.getHitPosition(),zIntersection,_detectorGeometry);
		if (fcf::goodCandidateHelix(helix,_detectorGeometry,trackSelector)) {
		  //avoids a copy twice
		  trackHitCandidates.emplace_back(std::vector<int>{hitNumberO,hitNumberI,hitNumberOSAS});
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


