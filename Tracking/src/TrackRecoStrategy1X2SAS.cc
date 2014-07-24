#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include "Tracking/include/TrackRecoStrategy1X2SAS.hh"

fc::TrackRecoStrategy1X2SAS::TrackRecoStrategy1X2SAS(int debugLevel,const DetectorGeometry& detectorGeometry):
  TrackRecoStrategy(debugLevel,detectorGeometry) {
}


//   int _debugLevel;

//     // Detector information
//     const DetectorGeometry & _detectorGeometry;

 
//     // intermediate tracking parameters
//     double _minPTCut;
//     double _maxTanLCut;
//     double _maxDRCut;
//     double _maxDZCut;
//     double _maxChi2Cut;
//     double _maxChi2NDofCut;
//     bool _useExpHits;


void fc::TrackRecoStrategy1X2SAS::findTracks(fc::trackSet& trackCandidateSet,const HitSet & recoHitSet){

  findHitsOnLayer(trackCandidateSet,recoHitSet,8);
  intermediateTrackFilter(trackCandidateSet,4);
   

  findHitsOnLayer(trackCandidateSet,recoHitSet,2);
  intermediateTrackFilter(trackCandidateSet,5);
  findHitsOnLayer(trackCandidateSet,recoHitSet,1);
  intermediateTrackFilter(trackCandidateSet,6);
  findHitsOnLayer(trackCandidateSet,recoHitSet,0);
  intermediateTrackFilter(trackCandidateSet,7);
 

  findHitsOnLayer(trackCandidateSet,recoHitSet,7);
  intermediateTrackFilter(trackCandidateSet,8);
  findHitsOnLayer(trackCandidateSet,recoHitSet,6);
  intermediateTrackFilter(trackCandidateSet,9);
  findHitsOnLayer(trackCandidateSet,recoHitSet,5);
  intermediateTrackFilter(trackCandidateSet,10);
 

}
