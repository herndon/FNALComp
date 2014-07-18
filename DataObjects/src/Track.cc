#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "DataObjects/include/HitSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Track.hh"
#include "DataObjects/include/Helix.hh"
#include "Algorithms/include/TrackFit.hh"
#include "Algorithms/include/InitializeHelix.hh"


// Helix parameter initialization
fc::Track::Track(double kappa, double dr, double dz, double phi0, double tanL, const DetectorGeometry & detectorGeometry):
  _helix(dr,phi0+M_PI/2.0,-1.0*kappa,dz,tanL,1.0/detectorGeometry.getCurvatureC()),
  _covMatrix(Helix::_sDim,Helix::_sDim),
  _chi2(0.0),
  _nDof(0),
  _detectorGeometry(detectorGeometry),
  _alpha(1.0/detectorGeometry.getCurvatureC()){



}

// Lorentz vector initialization
fc::Track::Track(const TLorentzVector & lorentzVector, int charge, const TVector3 & dr, int d0sign,const DetectorGeometry & detectorGeometry):
  _helix(dr.Perp()*d0sign,lorentzVector.Phi()+M_PI/2.0,-1.0*charge/lorentzVector.Pt(),dr.z(),lorentzVector.Pz()/ lorentzVector.Pt(),1.0/detectorGeometry.getCurvatureC()),
  _covMatrix(Helix::_sDim,Helix::_sDim),
  _chi2(0.0),
  _nDof(0),
  _detectorGeometry(detectorGeometry),
  _alpha(1.0/detectorGeometry.getCurvatureC()) {

}


fc::Track::Track(const HitSet & hitSet, const std::vector<int> & trackHitCandidate, const TVector3 & primaryVertex, const DetectorGeometry & detectorGeometry, int debugLevel):
  _covMatrix(Helix::_sDim,Helix::_sDim),
  _chi2(0.0),
  _nDof(0),
  _detectorGeometry(detectorGeometry),
  _alpha(1.0/_detectorGeometry.getCurvatureC()){


  // !!!!! change this to just take a hit map and HitSet for input
  // !!!!! Move calculation to function that can be called my either 3 hit or more hit function


  for (std::vector<int>::const_iterator trackHitCandidateIter = trackHitCandidate.begin(); trackHitCandidateIter != trackHitCandidate.end(); ++trackHitCandidateIter){
    insertHit(*trackHitCandidateIter,hitSet.getHits()[*trackHitCandidateIter].getLayer());
  }

  // Choose Hits to initialize Helix.  
  int outerXHit = -1;
  int middleXHit = -1;
  int outerZHit = -1;

  chooseHitsForInitialization(hitSet, trackHitCandidate,outerXHit, middleXHit, outerZHit);


    TVector3 x1 = primaryVertex;
    TVector3 x2 = hitSet.getHits()[middleXHit].getHitPosition();
    TVector3 x3 = hitSet.getHits()[outerXHit].getHitPosition();
    TVector3 z1 = hitSet.getHits()[outerZHit].getHitPosition();



    Helix initialHelix = initializeHelix(x1,x2,x3,z1,detectorGeometry);
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

  double chi2=0.0;
  int nDof=0;

  _helix = FitToHelix(initialHelix,hitSet,_trackHitMap,detectorGeometry,_covMatrix,chi2,nDof,2);
  _chi2 = chi2;
  _nDof = nDof;
  std::cout << "Track chi2 " << _chi2 << " " << _nDof << std::endl;

      //_helix.setHelix(testTrackFit1.getHelix().getHelix());
      //_helix.setAlpha(1.0/_detectorGeometry.getCurvatureC());

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
  _chi2(track._chi2),
  _nDof(track._nDof),
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

  if (_nDof > 0){

    std::cout << "chi2 " << _chi2 << " ndof " << _nDof << std::endl;
    _covMatrix.Print();

    }

  trackHitMap::size_type numberHits =_trackHitMap.size();

  std::cout << "Number of hits " << numberHits << std::endl;
  
  std::cout << "Hit layers, numbers: ";
  for (trackHitMap::const_iterator trackHitMapIter = _trackHitMap.begin(); trackHitMapIter != _trackHitMap.end(); ++trackHitMapIter){
    std::cout << trackHitMapIter->second << ", " << trackHitMapIter->first << ": ";
  }
  std::cout << std::endl;



}
