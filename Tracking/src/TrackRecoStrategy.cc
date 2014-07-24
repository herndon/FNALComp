#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include <vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/HitSet.hh"
#include "DataObjects/include/Track.hh"
#include "Algorithms/include/TrackFitMeasurements.hh"
#include "Tracking/include/TrackRecoStrategy.hh"


void fc::TrackRecoStrategy::recoTracks(trackSet & trackCandidateSet, const HitSet& recoHitSet){


  // !!!!! trackCandidateSet type def to a track list since we are doing may insertions and deletions?
 
  findTracks(trackCandidateSet,recoHitSet);

  finalTrackFilter(trackCandidateSet);

}




void  fc::TrackRecoStrategy::findHitsOnLayer(fc::trackSet & trackCandidateSet,const HitSet & recoHitSet,int layer){

  trackSet allNewTracks;

  for (trackSet::iterator trackIter = trackCandidateSet.begin(); trackIter!= trackCandidateSet.end(); ++trackIter){

   trackSet newTracks;

    bool hasLayerHit = false;
    for (trackHitSet::const_iterator hitIter = trackIter->getHits().begin(); hitIter!=trackIter->getHits().end(); ++hitIter){
      if (recoHitSet.getHits()[*hitIter].getLayer() == layer){
	hasLayerHit = true;
      }
    }
    if (hasLayerHit) continue;


    double maxResidual = 5.0*expectedMeasurementUncertianty1D(trackIter->getHelix(), trackIter->getCovMatrix(), layer, _detectorGeometry);

    int hitNumber = 0;
    for (hitSet::const_iterator hitIter = recoHitSet.getHits().begin(); hitIter != recoHitSet.getHits().end(); ++hitIter,++hitNumber){

      if (hitIter->getLayer()==layer){

	if (std::abs(expectedMeasurementVectorX(trackIter->getHelix(),hitIter->getLayer(),_detectorGeometry)(0,0) 
		   - measurementVector1D(hitIter->getHitPosition(),hitIter->getLayer(),_detectorGeometry)(0,0)) 
	    < maxResidual) {

	  trackHitSet trackHitCandidate = trackIter->getHits();
       	  trackHitCandidate.push_back(hitNumber);

	  Track newTrack(recoHitSet,trackHitCandidate,_detectorGeometry, _debugLevel);

	  // !!!!! replace with good candidate call

	  if (goodTrack(newTrack)) newTracks.push_back(newTrack);

	}

      }

    } // End loop over hits

    // Find best candidates
    std::vector<int> reducedTrackList = bestTracks(newTracks);

    for (std::vector<int>::const_iterator trackNumberIter = reducedTrackList.begin(); trackNumberIter < reducedTrackList.end();++trackNumberIter){
      allNewTracks.push_back(newTracks[*trackNumberIter]);
    }
				  
    // !!!!! consider whether to drop old candidate if worst than all new ones

 
  } // End loop over old track candidates

    for (trackSet::iterator trackIter = allNewTracks.begin(); trackIter!= allNewTracks.end(); ++trackIter){
      trackCandidateSet.push_back(*trackIter);
    }

}

bool fc::TrackRecoStrategy::goodTrack(const Track& track) const{

  return  track.getHelix().getPT() > 1.0 && 
    track.getChi2()< 100.0 && 
    std::abs(track.getHelix().getDr()) < 0.05 && 
    std::abs(track.getHelix().getDz()) < 0.05;
}

std::vector<int> fc::TrackRecoStrategy::bestTracks(const trackSet & tracks) const{

  // keep the two best and all before the fit is constrined
  double bestChi2 = 999.0;
  double secondBestChi2 = 999.0;
  int bestTrack = -1;
  int secondBestTrack = -1;
  int trackN = 0;
  std::vector<int> trackList;

  // !!!!! Check if all/none/some are constrained

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



void fc::TrackRecoStrategy::intermediateTrackFilter(fc::trackSet & trackCandidateSet,int expNHits){

  // Right now just checks the number of expected hits

  for (trackSet::iterator trackIter = trackCandidateSet.begin(); trackIter != trackCandidateSet.end(); ++trackIter){

    if (trackIter->getHits().size() < expNHits-1) {
      trackCandidateSet.erase(trackIter);
      trackIter--;
    }

  }
}

void fc::TrackRecoStrategy::finalTrackFilter(fc::trackSet & trackCandidateSet){


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
