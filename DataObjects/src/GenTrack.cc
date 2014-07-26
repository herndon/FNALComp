#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "DataObjects/include/Helix.hh"
#include "DataObjects/include/GenTrack.hh"


// Lorentz vector initialization
fc::GenTrack::GenTrack(const TLorentzVector & lorentzVector, int charge, const TVector3 & dr):
  _lorentzVector(lorentzVector),_charge(charge),_dr(dr){
}


const fc::Helix fc::GenTrack::makeHelix(double alpha) const{

  double dr = 1.0*_dr.Perp()*_dr.Y()/std::abs(_dr.Y());

  Helix helix(dr,_lorentzVector.Phi()+M_PI/2.0,-1.0*_charge/_lorentzVector.Pt(),_dr.Z(),_lorentzVector.Pz()/_lorentzVector.Pt(),alpha);

  return helix;
}

const fc::Helix fc::GenTrack::makeHelix() const{
  return makeHelix(0);
}


void fc::GenTrack::print(void) const{

  std::cout << "Charge " << _charge << std::endl;
  std::cout << "4 momentum " << _lorentzVector.Px() << " " <<  _lorentzVector.Py() << " " <<  _lorentzVector.Pz() << " " <<  _lorentzVector.E() << " " << std::endl;
  std::cout << "Track parameters:  pT " <<  _lorentzVector.Pt() << " cot(theta) " << _lorentzVector.Pt()/_lorentzVector.Pz()  << " phi0 " << std::atan2(_lorentzVector.Py(),_lorentzVector.Px()) << " d0 " << makeHelix().getDr() << " z0 " << makeHelix().getDz() << std::endl;
  std::cout << "Helix paramters: kappa " << makeHelix().getKappa() << " tan(Lambda) " << makeHelix().getTanL() << " phi0 to d0 " << makeHelix().getPhi0() << std::endl;  

}
