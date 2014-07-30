#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "Algorithms/include/TrackFitMeasurements.hh"
#include "Algorithms/include/BuildTrack.hh"
#include "Tracking/include/TrackingSelectors.hh"
#include "Tracking/include/TrackingFilters.hh"
#include "Tracking/include/LayerTrackFinder.hh"


fc::LayerTrackFinder::LayerTrackFinder(int debugLevel,const DetectorGeometry& detectorGeometry,int layer,int nExpHits,double minPTCut, double maxChi2NDofCut):
  _debugLevel(debugLevel),
  _detectorGeometry(detectorGeometry),
  _layer(layer),
  _nExpHits(nExpHits),
  _minPTCut(minPTCut),
  _maxChi2NDofCut(maxChi2NDofCut){
}

void fc::LayerTrackFinder::findCandidateTracks(trackSet& trackCandidateSet, const HitSet& recoHitSet,unsigned int expNHit) const{

  trackSet allNewTracks;

  for (auto& track :  trackCandidateSet) {

    findSingleCandidateTracks(track,allNewTracks,recoHitSet);  

  }

  trackCandidateSet.reserve(trackCandidateSet.size()+allNewTracks.size());
  for (auto& track: allNewTracks) {
    trackCandidateSet.push_back(std::move(track));
  }

  fcf::TrackingSelector trackSelector;
  trackSelector._nHitCut = _nExpHits;

  fcf::simpleTrackSetFilter(trackCandidateSet,_detectorGeometry,trackSelector);
  //layerTrackFilter(trackCandidateSet,expNHit);

}

void fc::LayerTrackFinder::findSingleCandidateTracks(const Track& track , trackSet& allNewTracks, const HitSet& recoHitSet)const {



  std::vector<int> hits = findHits(track,recoHitSet);
  trackSet newTracks = buildTrackCandidates(track, hits, recoHitSet);
  std::vector<int> bestTracks = bestTrackCandidates(newTracks);
  //We may want to decide whether to remove the seed track
  //removeSeedTrack(trackCandidateSet,trackSet);

  for (auto trackNumber: bestTracks){
    allNewTracks.push_back(newTracks[trackNumber]);
  }

}

std::vector<int>  fc::LayerTrackFinder::findHits(const Track & track , const HitSet & recoHitSet) const{

  std::vector<int> hits;

  bool hasLayerHit = false;
  for (auto hitIndex : track.getHits()) {
    if (recoHitSet.getHits()[hitIndex].getLayer() == _layer){
      hasLayerHit = true;
    }
  }
  if (hasLayerHit) return hits;

  double maxResidual = 5.0*expectedMeasurementUncertianty1D(track.getHelix(), track.getCovMatrix(), _layer, _detectorGeometry);
  int hitNumber = 0;

  for (auto hitIter = recoHitSet.getHits().begin(); hitIter != recoHitSet.getHits().end(); ++hitIter,++hitNumber){

    if (hitIter->getLayer()==_layer){

      if (std::abs(expectedMeasurementVector1D(track.getHelix(),_layer,_detectorGeometry)(0,0) 
		   - measurementVector1D(hitIter->getHitPosition(),_layer,_detectorGeometry)(0,0)) 
	  < maxResidual) {

	hits.push_back(hitNumber);

      }

    }

  } // End loop over hits

  return hits;

}

std::vector<int> fc::LayerTrackFinder::bestTrackCandidates(const trackSet & tracks) const{

  std::vector<int> trackList;

  // keep the two best and all before the fit is constrined
  double bestChi2 = 999.0;
  double secondBestChi2 = 999.0;
  int bestTrack = -1;
  int secondBestTrack = -1;
  int trackN = 0;


  for (trackSet::const_iterator trackIter = tracks.begin(); trackIter != tracks.end(); ++trackIter,++trackN){
    if (trackIter->getNDof() > 0 && trackIter->getChi2() < bestChi2) {
      secondBestChi2 = bestChi2;
      secondBestTrack = bestTrack;
      bestChi2 = trackIter->getChi2();
      bestTrack = trackN;
    } else if (trackIter->getNDof() > 0 && trackIter->getChi2() < secondBestChi2){
      secondBestChi2 = trackIter->getChi2();
      secondBestTrack = trackN;
    }

  }

  trackN = 0;

  // Keep all if not yet constrainted since we can't compare chi2/ndof
  for (trackSet::const_iterator trackIter = tracks.begin(); trackIter != tracks.end(); ++trackIter,++trackN){
    if (trackIter->getNDof() <= 0) trackList.push_back(trackN);
  }
  if (bestTrack > -1)  trackList.push_back(bestTrack);
  if (secondBestTrack > -1)  trackList.push_back(secondBestTrack);

  // 	if (trackSetIter->getNDof()> 0 && secondBestTrack > -1 && secondBestChi2/foundTracks[secondBestTrack].getNDof() <  trackSetIter->getChi2()/trackSetIter->getNDof()) {
  // 	  trackCandidateSet.getTracks().erase(trackSetIter);
  //           trackSetIter--;
  // 	}



  return trackList;

}



fc::trackSet fc::LayerTrackFinder::buildTrackCandidates(const Track & track, const std::vector<int> & hits, const HitSet & recoHitSet) const{

  fcf::TrackingSelector trackSelector;
  trackSelector._minPTCut = _minPTCut;
  trackSelector._maxChi2NDofCut = _maxChi2NDofCut;
  trackSelector._nHitCut = _nExpHits;
  trackSelector._useFiducialDRCut = true;
  trackSelector._useFiducialDZCut = true;


  trackSet newTracks;
  for (auto hitNumber : hits) {
    trackHitSet trackHitCandidate = track.getHits();
    trackHitCandidate.push_back(hitNumber);

    Track newTrack(buildTrack(recoHitSet,trackHitCandidate,_detectorGeometry,_debugLevel));

    if (fcf::goodCandidateTrack(newTrack,_detectorGeometry,trackSelector)) newTracks.push_back(std::move(newTrack));
  }
  return newTracks;

}



