#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include <vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "Tracking/include/LayerTrackFinder.hh"
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

  std::vector<int> layers {8,2,1,0,7,6,5};
  int expNHit = 4;

  for (std::vector<int>::const_iterator layerIter = layers.begin();layerIter!=layers.end();++layerIter,++expNHit){
    LayerTrackFinder layerTrackFinder(getDebugLevel(),getDetectorGeometry(),*layerIter);
    layerTrackFinder.findCandidateTracks(trackCandidateSet,recoHitSet,expNHit);

  }

}
