#define _USE_MATH_DEFINES
#include<cmath>
#include <iostream>
#include "DataObjects/include/HitSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/Helix.hh"
#include "Algorithms/include/TrackFit.hh"
#include "Algorithms/include/InitializeHelix.hh"
#include "Algorithms/include/BuildTrack.hh"



const fc::Track fc::buildTrack(const HitSet & hitSet, const std::vector<int> & trackHitCandidate, const DetectorGeometry & detectorGeometry, int debugLevel){

  int numberXHits = 0;
  int numberSASHits = 0;
  int numberZHits = 0;

  // !!!!! Geometry needs to understand types of sensors for use here
  int layer;
  for (std::vector<int>::const_iterator trackHitCandidateIter = trackHitCandidate.begin(); trackHitCandidateIter != trackHitCandidate.end(); ++trackHitCandidateIter){
    layer = hitSet.getHits()[*trackHitCandidateIter].getLayer();
    if (layer >= 0 && layer <= 4) ++numberXHits;
    if (layer==9||layer==8) ++numberSASHits;
    if (layer>=5 && layer <=7) ++numberZHits;
  }

  // Choose Hits to initialize Helix.  
  int outerXHit = -1;
  int middleXHit = -1;
  int outerZHit = -1;

  chooseHitsForInitialization(hitSet, trackHitCandidate,outerXHit, middleXHit, outerZHit,detectorGeometry);


  // Primary vertex used to help find seed tracks
  TVector3 x1(0.0,0.0,0.0);
  TVector3 x2 = hitSet.getHits()[middleXHit].getHitPosition();
  TVector3 x3 = hitSet.getHits()[outerXHit].getHitPosition();

  TVector3 z1;
  findZForInitialization(hitSet,trackHitCandidate,z1,detectorGeometry);



  Helix initialHelix = initializeHelix(x1,x2,x3,z1,detectorGeometry);

  double chi2=0.0;
  int nDof=0;
  TMatrixD covMatrix(Helix::_sDim,Helix::_sDim); 

  int fitType = 0;
  if (numberXHits <3 ) fitType += 1;
  if ((numberSASHits+numberZHits)<2) fitType += 2;

  if (fitType >0){
    Helix helix = fitToHelixWithPV(initialHelix,hitSet,trackHitCandidate,detectorGeometry,covMatrix,chi2,nDof,fitType,2);
    return Track(helix,covMatrix,chi2,nDof,trackHitCandidate);
  } else {
    Helix helix = fitToHelix(initialHelix,hitSet,trackHitCandidate,detectorGeometry,covMatrix,chi2,nDof,2);
    return Track(helix,covMatrix,chi2,nDof,trackHitCandidate);
  }

}

