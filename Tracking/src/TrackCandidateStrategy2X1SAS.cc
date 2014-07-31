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


void fc::TrackCandidateStrategy2X1SAS::findTrackCandidates(trackSet& trackCandidateSet, const HitSet& recoHitSet) const{

  std::vector<trackHitSet> trackHitCandidates;

  findHitCadidates(trackHitCandidates,recoHitSet);

  for (auto const& trackHitCandidate: trackHitCandidates) {
    trackCandidateSet.push_back(buildTrack(recoHitSet,trackHitCandidate,_detectorGeometry,_debugLevel));
  }

  // We could filter the candidates at this point

}


// !!!!! Could improve to use more than outer layers
// !!!!! control seed layers from config
void fc::TrackCandidateStrategy2X1SAS::findHitCadidates(std::vector<fc::trackHitSet>& trackHitCandidates,const HitSet& hitSet) const{
 
  fcf::TrackingSelector trackSelector;
  trackSelector._minPTCut = _minCandPTCut;

  int hitNumberO = 0;

  // Form 4-3,9 hit candidates
  for (hitSet::const_iterator hitIterO = hitSet.getHits().begin(); hitIterO != hitSet.getHits().end(); ++hitIterO,++hitNumberO) {

    if (hitIterO->getLayer() == 4) {

      int hitNumberI = 0;
 
      for (hitSet::const_iterator hitIterI = hitSet.getHits().begin(); hitIterI != hitSet.getHits().end(); ++hitIterI,++hitNumberI) {

	if (hitIterI->getLayer() == 3) {

	  int hitNumberOSAS = 0;

	  for (hitSet::const_iterator hitIterOSAS = hitSet.getHits().begin(); hitIterOSAS != hitSet.getHits().end(); ++hitIterOSAS,++hitNumberOSAS) {

	    if (hitIterOSAS->getLayer() == 9) {

	      TVector3 zIntersection;
	      bool goodIntersection = intersectStrips(*hitIterO,*hitIterOSAS,zIntersection,_detectorGeometry);
	      if (goodIntersection) {

                TVector3 primaryVertex(0.0,0.0,0.0);
		Helix helix = initializeHelix(primaryVertex,hitIterO->getHitPosition(),hitIterI->getHitPosition(),zIntersection,_detectorGeometry);
		if (fcf::goodCandidateHelix(helix,_detectorGeometry,trackSelector)) {
		  //avoids a copy
		  trackHitCandidates.push_back( std::vector<int>{hitNumberO,hitNumberI,hitNumberOSAS} );
		}
	      }

	    }

	  }

	}

      }

    }

  }
}


