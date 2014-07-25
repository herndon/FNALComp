#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "DataObjects/include/GenTrack.hh"


// Lorentz vector initialization
fc::GenTrack::GenTrack(const TLorentzVector & lorentzVector, int charge, const TVector3 & dr):
  _lorentzVector(lorentzVector),_charge(charge),_dr(dr){
}

void fc::GenTrack::print(void) const{

  std::cout << "Charge " << _charge << std::endl;
  std::cout << "4 momentum " << _lorentzVector.Px() << " " <<  _lorentzVector.Py() << " " <<  _lorentzVector.Pz() << " " <<  _lorentzVector.E() << " " << std::endl;


}

// To remind me how to convert when it gets time to intersect the gen track with a plane

  //  _helix(dr.Perp()*d0sign,lorentzVector.Phi()+M_PI/2.0,-1.0*charge/lorentzVector.Pt(),dr.z(),lorentzVector.Pz()/ lorentzVector.Pt(),1.0/detectorGeometry.getCurvatureC())
  //  std::cout << "Track parameters:  pT " <<  _lorentzVector.Pt() << " cot(theta) " <<  _lorentzVector.Pt()/_lorentzVector.Pz() << " phi0 " << std::atan2(_lorentzVector.Py(),_lorentzVector.Px()) << " d0 " << -_dr.Perp()*_dr.Y()/std::abs(_dr.Y()) << " z0 " << _dr.Z() << std::endl;


  //  std::cout << "Helix paramters: kappa " << -1.0*_charge/_lorentzVector.Pt() << " tan(Lambda) " << _lorentzVector.Pz()/_lorentzVector.Pt() << " phi0 to d0 " << _lorentzVector.Phi()+M_PI/2.0 << std::endl;  
  //std::cout << "Reference Point " << 0.0 << " " << 0.0 << " " << 0.0 << std::endl;
