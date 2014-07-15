#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "HitSet.hh"
#include "DetectorGeometry.hh"
#include "Track.hh"
#include "Helix.hh"
#include "TrackFit.hh"
#include "InitializeHelix.hh"


// Helix parameter initialization
fc::Track::Track(double kappa, double dr, double dz, double phi0, double tanL, const DetectorGeometry & detectorGeometry):
  _helix(dr,phi0+M_PI/2.0,-1.0*kappa,dz,tanL,1.0/detectorGeometry.getCurvatureC()),
  _covMatrix(NULL),
  _detectorGeometry(detectorGeometry),
  _alpha(1.0/detectorGeometry.getCurvatureC()){



}

// Lorentz vector initialization
fc::Track::Track(const TLorentzVector & lorentzVector, int charge, const TVector3 & dr, const DetectorGeometry & detectorGeometry):
  _helix(dr.Perp(),lorentzVector.Phi()+M_PI/2.0,-1.0*charge/lorentzVector.Pt(),dr.z(),lorentzVector.Pz()/ lorentzVector.Pt(),1.0/detectorGeometry.getCurvatureC()),
  _covMatrix(NULL),
  _detectorGeometry(detectorGeometry),
  _alpha(1.0/detectorGeometry.getCurvatureC()) {

}


fc::Track::Track(const HitSet & myHitSet, const std::vector<int> & trackHitCandidate, const TVector3 & primaryVertex, const DetectorGeometry & detectorGeometry, int debugLevel):
  _covMatrix(NULL),
  _detectorGeometry(detectorGeometry),
  _alpha(1.0/_detectorGeometry.getCurvatureC()){


  // !!!!! change this to just take a hit map and HitSet for input
  // !!!!! Move calculation to function that can be called my either 3 hit or more hit function


    insertHit(trackHitCandidate[0],4);
    insertHit(trackHitCandidate[1],3);
    insertHit(trackHitCandidate[2],2);
    insertHit(trackHitCandidate[3],1);
    insertHit(trackHitCandidate[4],0);


    TVector3 x1 = primaryVertex;
    TVector3 x2 = myHitSet.getHit(trackHitCandidate[2]).getHitPosition();
    TVector3 x3 = myHitSet.getHit(trackHitCandidate[0]).getHitPosition();



    Helix initialHelix = initializeHelix(x1,x2,x3,detectorGeometry);
    initialHelix.setAlpha(1.0/_detectorGeometry.getCurvatureC());
    _helix = initialHelix;

    //TrackFit testTrackFit1(initialHelix,detectorGeometry);

    //testTrackFit1.insertHit(trackHitCandidate[0],4);
    //testTrackFit1.insertHit(trackHitCandidate[1],3);
    //testTrackFit1.insertHit(trackHitCandidate[2],2);
    //testTrackFit1.insertHit(trackHitCandidate[3],1);
    //testTrackFit1.insertHit(trackHitCandidate[4],0);


     std::cout << "Track before fit" << std::endl;
      //print(); 
      print();


      _helix = FitToHelix(initialHelix,myHitSet,_trackHitMap,detectorGeometry,2);

      //_helix.setHelix(testTrackFit1.getHelix().getHelix());
      //_helix.setAlpha(1.0/_detectorGeometry.getCurvatureC());


  // Set Initial covariance matrix
  if (_covMatrix) { delete _covMatrix;}
  _covMatrix = new TMatrixD(Helix::_sDim,Helix::_sDim);

  _covMatrix->Zero();
  for (int ii = 0; ii < Helix::_sDim; ++ii){
    (*_covMatrix)(ii,ii) = 1.0e4;
  }



}



TLorentzVector fc::Track::getLorentzVector(void) const{
  TLorentzVector lorentzVector;
  double pT = std::abs(1.0/getHelix().getKappa());
  double pZ = getHelix().getTanL()*pT;
  lorentzVector.SetPxPyPzE(pT*std::cos(getHelix().getPhi0()-M_PI/2.0),pT*std::sin(getHelix().getPhi0()-M_PI/2.0),pZ,std::sqrt(pT*pT+pZ*pZ));
  return lorentzVector;
}



// Copy constructor
fc::Track::Track(const Track & track):
  _helix(track._helix),
  _covMatrix(track._covMatrix),
  _detectorGeometry(track._detectorGeometry),
  _alpha(track._alpha) {

  trackHitMap inputTrackHitMap = track.getTrackHitMap();

  for (trackHitMap::const_iterator trackHitMapIter = inputTrackHitMap.begin(); trackHitMapIter != inputTrackHitMap.end(); ++trackHitMapIter){    insertHit(trackHitMapIter->first,trackHitMapIter->second);
  }

}


void fc::Track::insertHit(int hitNumber, int layer){
   _trackHitMap.insert(trackHitMap::value_type(hitNumber,layer));
}









void fc::Track::print(void) const{

  TLorentzVector lorentzVector = getLorentzVector();

  std::cout << "Charge " << getCharge() << std::endl;
  std::cout << "4 momentum " << lorentzVector.Px() << " " <<  lorentzVector.Py() << " " <<  lorentzVector.Pz() << " " <<  lorentzVector.E() << " " << std::endl;
  std::cout << "Track parameters:  pT " <<  lorentzVector.Pt() << " cot(theta) " << 1/getHelix().getTanL() << " phi0 " << getHelix().getPhi0()-M_PI/2.0 << " d0 " << getHelix().getDr() << " z0 " << getHelix().getDz() << std::endl;
  std::cout << "Helix paramters: kappa " << getHelix().getKappa() << " tan(Lambda) " << getHelix().getTanL() << " phi0 to d0 " << getHelix().getPhi0() << std::endl;  
  std::cout << "Pivot Point " << 0.0 << " " << 0.0 << " " << 0.0 << std::endl;


  trackHitMap::size_type numberHits =_trackHitMap.size();

  std::cout << "Number of hits " << numberHits << std::endl;
  
  std::cout << "Hit layers, numbers: ";
  for (trackHitMap::const_iterator trackHitMapIter = _trackHitMap.begin(); trackHitMapIter != _trackHitMap.end(); ++trackHitMapIter){
    std::cout << trackHitMapIter->second << ", " << trackHitMapIter->first << ": ";
  }
  std::cout << std::endl;



}
