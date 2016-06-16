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
  // Standard def of phi0 and dR
   double phi0ToDr = std::atan2(_lorentzVector.Py(),_lorentzVector.Px()) -_charge*M_PI/2.0;
   double dr = std::abs(_dr.Perp());
   if (std::abs(phi0ToDr - std::atan2(_dr.Y(),_dr.X())) > M_PI/4.0) dr = -dr;

   // The way the track parameterization understand phi0toDr
   phi0ToDr = std::atan2(_lorentzVector.Py(),_lorentzVector.Px()) + M_PI/2.0;

    Helix helix(dr,phi0ToDr,1.0*_charge/_lorentzVector.Pt(),
                _dr.Z(),_lorentzVector.Pz()/_lorentzVector.Pt(),curvatureC*bField.Z(),
                curvatureC);

    return helix;
}

// Only called in GenTrack print method.


void fc::GenTrack::print(std::ostream& out) const {

    // dummy field for makeHelix  Can not be used to retrieve the radius of curvature with needs the magentic field
    TVector3 bField(0.0,0.0,0.0);

    out << "Charge " << charge() << std::endl;
    out << "4 momentum " << lorentzVector().Px() << " " <<  lorentzVector().Py() <<
      " " <<  lorentzVector().Pz() << " " <<  lorentzVector().E() << " " << std::endl;
    out << "pT " <<  _lorentzVector.Pt() << " cot(theta) " <<
      lorentzVector().Pt()/lorentzVector().Pz()  << " phi0 " << std::atan2(lorentzVector().Py(),lorentzVector().Px()) << std::endl;
    out << "Helix parameters: kappa" << makeHelix(bField,0.0).kappa() << " tan(Lambda) " << makeHelix(bField,0.0).tanL() << std::endl;

    out << "phi0 to d0 " << makeHelix(bField,0.0).phi0() << " d0 " << makeHelix(bField,0.0).dR() << " z0 " << makeHelix(bField,0.0).dZ() << std::endl;
    out << "Vertex : " << dR().X() << " " << dR().Y() << " " << dR().Z() << std::endl;
 

}
