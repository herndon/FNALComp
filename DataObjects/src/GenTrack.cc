#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "DataObjects/include/Helix.hh"
#include "DataObjects/include/GenTrack.hh"


// Lorentz vector initialization
fc::GenTrack::GenTrack(const TLorentzVector & lorentzVector, int charge,
                       const TVector3 & dr):
    _lorentzVector(lorentzVector),_charge(charge),_dr(dr) {
}


const fc::Helix fc::GenTrack::makeHelix(const TVector3& bField,
                                        double curvatureC) const {

    double dr = 1.0*_dr.Perp()*_dr.Y()/std::abs(_dr.Y());

    Helix helix(dr,_lorentzVector.Phi()+M_PI/2.0,-1.0*_charge/_lorentzVector.Pt(),
                _dr.Z(),_lorentzVector.Pz()/_lorentzVector.Pt(),curvatureC*bField.Z(),
                curvatureC);

    return helix;
}

// Only called in GenTrack print method.


void fc::GenTrack::print(ostream& out) const {

    // dummy field for makeHelix  Can not be used to retrieve the radius of curvature with needs the magentic field
    TVector3 bField(0.0,0.0,0.0);

    out << "Charge " << _charge << std::endl;
    out << "4 momentum " << _lorentzVector.Px() << " " <<  _lorentzVector.Py() <<
        " " <<  _lorentzVector.Pz() << " " <<  _lorentzVector.E() << " " << std::endl;
    out << "Track parameters:  pT " <<  _lorentzVector.Pt() << " cot(theta) " <<
        _lorentzVector.Pt()/_lorentzVector.Pz()  << " phi0 " << std::atan2(
            _lorentzVector.Py(),_lorentzVector.Px()) << " d0 " << makeHelix(bField,
                    0.0).getDr() << " z0 " << makeHelix(bField,0.0).getDz() << std::endl;
    out << "Helix paramters: kappa " << makeHelix(bField,
            0.0).getKappa() << " tan(Lambda) " << makeHelix(bField,
                    0.0).getTanL() << " phi0 to d0 " << makeHelix(bField,
                            0.0).getPhi0() << std::endl;

}
