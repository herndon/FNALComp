#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include <vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "Algorithms/include/TrackFitMeasurements.hh"
#include "Tracking/include/LayerTrackFinder.hh"


void fc::LayerTrackFinder::findCandidateTracks(trackSet & trackCandidateSet, const HitSet& recoHitSet) const{

  trackSet allNewTracks;

  for (trackSet::iterator trackIter = trackCandidateSet.begin(); trackIter != trackCandidateSet.end(); ++trackIter){

    findSingleCandidateTracks(*trackIter,allNewTracks,recoHitSet);  

  }

  for (trackSet::iterator trackIter = allNewTracks.begin(); trackIter!= allNewTracks.end(); ++trackIter){
    trackCandidateSet.push_back(*trackIter);
  }


  layerTrackFilter(trackCandidateSet);

}

void fc::LayerTrackFinder::findSingleCandidateTracks(const Track& track , trackSet& allNewTracks, const HitSet& recoHitSet)const {



  std::vector<int> hits = findHits(track,recoHitSet);
  trackSet newTracks = buildTrackCandidates(track, hits, recoHitSet);
  std::vector<int> bestTracks = bestTrackCandidates(newTracks);
  //We may want to decide whether to remove the seed track
  //removeSeedTrack(trackCandidateSet,trackSet);

  for (std::vector<int>::const_iterator trackNumberIter = bestTracks.begin(); trackNumberIter < bestTracks.end();++trackNumberIter){
    allNewTracks.push_back(newTracks[*trackNumberIter]);
  }

}

std::vector<int>  fc::LayerTrackFinder::findHits(const Track & track , const HitSet & recoHitSet) const{

  std::vector<int> hits;

  bool hasLayerHit = false;
  for (trackHitSet::const_iterator hitIter = track.getHits().begin(); hitIter!=track.getHits().end(); ++hitIter){
    if (recoHitSet.getHits()[*hitIter].getLayer() == _layer){
      hasLayerHit = true;
    }
  }
  if (hasLayerHit) return hits;

  double maxResidual = 5.0*expectedMeasurementUncertianty1D(track.getHelix(), track.getCovMatrix(), _layer, _detectorGeometry);
  int hitNumber = 0;

  for (hitSet::const_iterator hitIter = recoHitSet.getHits().begin(); hitIter != recoHitSet.getHits().end(); ++hitIter,++hitNumber){

    if (hitIter->getLayer()==_layer){

      if (std::abs(expectedMeasurementVectorX(track.getHelix(),_layer,_detectorGeometry)(0,0) 
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

  trackSet newTracks;
  for (std::vector<int>::const_iterator hitNumberIter = hits.begin();hitNumberIter!=hits.end();++hitNumberIter){
    trackHitSet trackHitCandidate = track.getHits();
    trackHitCandidate.push_back(*hitNumberIter);

    Track newTrack(recoHitSet,trackHitCandidate,_detectorGeometry, _debugLevel);

    // !!!!! replace with good candidate call

    if (goodTrack(newTrack)) newTracks.push_back(newTrack);
  }
  return newTracks;

}



void fc::LayerTrackFinder::layerTrackFilter(fc::trackSet & trackCandidateSet) const{

  // Right now just checks the number of expected hits

  for (trackSet::iterator trackIter = trackCandidateSet.begin(); trackIter != trackCandidateSet.end(); ++trackIter){

    if (trackIter->getHits().size() < _expNHits-1) {
      trackCandidateSet.erase(trackIter);
      trackIter--;
    }
  }
}

bool fc::LayerTrackFinder::goodTrack(const Track& track) const{

  return  track.getHelix().getPT() > 1.0 && 
    track.getChi2()< 100.0 && 
    std::abs(track.getHelix().getDr()) < 0.05 && 
    std::abs(track.getHelix().getDz()) < 0.05;
}
