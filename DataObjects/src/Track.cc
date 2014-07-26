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


// Full parameter initialization
fc::Track::Track(const Helix& helix,const TMatrixD& covMatrix,double chi2,int nDof,const trackHitSet& trackHitCandidate):
  _helix(helix),
  _covMatrix(covMatrix),
  _chi2(chi2),
  _nDof(nDof),
  _trackHitSet(trackHitCandidate){

}


// !!!!! need to update streamer and replace
// Lorentz vector initialization
fc::Track::Track(const TLorentzVector & lorentzVector, int charge, const TVector3 & dr, int d0sign,const DetectorGeometry & detectorGeometry):
  _helix(dr.Perp()*d0sign,lorentzVector.Phi()+M_PI/2.0,-1.0*charge/lorentzVector.Pt(),dr.z(),lorentzVector.Pz()/ lorentzVector.Pt(),1.0/detectorGeometry.getCurvatureC()),
  _covMatrix(Helix::_sDim,Helix::_sDim),
  _chi2(0.0),
  _nDof(0) {

}




const TLorentzVector fc::Track::getLorentzVector(void) const{
  TLorentzVector lorentzVector;
  double pT = std::abs(1.0/getHelix().getKappa());
  double pZ = getHelix().getTanL()*pT;
  lorentzVector.SetPxPyPzE(pT*std::cos(getHelix().getPhi0()-M_PI/2.0),pT*std::sin(getHelix().getPhi0()-M_PI/2.0),pZ,std::sqrt(pT*pT+pZ*pZ));
  return lorentzVector;
}

// !!!!! revisit whether the copy constructor and assigment op are necessary.  The assignment op is to support delting in the TrackSet


fc::Track & fc::Track:: operator=(Track track){

  std::swap(_helix,track._helix);
  std::swap(_covMatrix,track._covMatrix);
  std::swap(_chi2,track._chi2);
  std::swap(_nDof,track._nDof);
  std::swap(_trackHitSet,track._trackHitSet);

  return *this;

}



void fc::Track::insertHit(int hitNumber){
  _trackHitSet.push_back(hitNumber);
}









void fc::Track::print(void) const{

  TLorentzVector lorentzVector = getLorentzVector();

  std::cout << "Charge " << getCharge() << std::endl;
  std::cout << "4 momentum " << lorentzVector.Px() << " " <<  lorentzVector.Py() << " " <<  lorentzVector.Pz() << " " <<  lorentzVector.E() << " " << std::endl;
  std::cout << "Track parameters:  pT " <<  lorentzVector.Pt() << " cot(theta) " << 1/getHelix().getTanL() << " phi0 " << getHelix().getPhi0()-M_PI/2.0 << " d0 " << getHelix().getDr() << " z0 " << getHelix().getDz() << std::endl;
  std::cout << "Helix paramters: kappa " << getHelix().getKappa() << " tan(Lambda) " << getHelix().getTanL() << " phi0 to d0 " << getHelix().getPhi0() << std::endl;  
  std::cout << "Reference Point " << 0.0 << " " << 0.0 << " " << 0.0 << std::endl;

  if (_nDof > 0){

    std::cout << "chi2 " << _chi2 << " ndof " << _nDof << std::endl;
    _covMatrix.Print();

    }

  trackHitSet::size_type numberHits =_trackHitSet.size();

  std::cout << "Number of hits " << numberHits << std::endl;
  
  std::cout << "Hit numbers: ";
  for (trackHitSet::const_iterator hitIter = _trackHitSet.begin(); hitIter != _trackHitSet.end(); ++hitIter){
    std::cout << *hitIter << " ";
  }
  std::cout << std::endl;



}
