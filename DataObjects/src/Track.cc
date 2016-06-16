#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "TMath.h"
#include "DataObjects/include/HitSet.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Helix.hh"
#include "DataObjects/include/Track.hh"

// Full parameter initialization
fc::Track::Track(const Helix& helix,const TMatrixD& covMatrix,double chi2,
                 int nDof,const TrackHitContainer& trackHitCandidate):
    _helix(helix),
    _covMatrix(covMatrix),
    _chi2(chi2),
    _nDof(nDof),
    _trackHits(trackHitCandidate) {

}


// !!!!! need to update streamer and replace
// Lorentz vector initialization
fc::Track::Track(const TLorentzVector & lorentzVector, int charge,
                 const TVector3 & dr, int d0sign,const DetectorGeometry & detectorGeometry):
    _helix(dr.Perp()*d0sign,lorentzVector.Phi()+M_PI/2.0,
           -1.0*charge/lorentzVector.Pt(),dr.z(),lorentzVector.Pz()/ lorentzVector.Pt(),
           detectorGeometry.curvatureCInField(detectorGeometry.bField()),
           detectorGeometry.curvatureC()),
    _covMatrix(_sDim,_sDim),
    _chi2(0.0),
    _nDof(0) {

}


const TLorentzVector fc::Track::lorentzVector() const {
    TLorentzVector lorentzVector;
    double pT = std::abs(1.0/kappa());
    double pZ = tanL()*pT;
    double trackPhi0  = phi0()-M_PI/2.0;

    lorentzVector.SetPxPyPzE(pT*std::cos(trackPhi0),
                             pT*std::sin(trackPhi0),pZ,std::sqrt(pT*pT+pZ*pZ));
    return lorentzVector;
}

double fc::Track::chi2Prob() const {

    // Root TMatch handles cases where chi2 or ndof = 0
  return TMath::Prob(chi2(),nDof());

}


void fc::Track::print(std::ostream& out) const {

    TLorentzVector lorentzVec = lorentzVector();

    out << "Charge " << charge() << std::endl;
    out << "4 momentum " << lorentzVec.Px() << " " <<  lorentzVec.Py() << " "
        <<  lorentzVec.Pz() << " " <<  lorentzVec.E() << " " << std::endl;
    out << "pT " <<  lorentzVec.Pt() << " cot(theta) " <<
      1/tanL() << " phi0 " << std::atan2(lorentzVec.Py(),lorentzVec.Px()) << std::endl; 
   out << "Helix parameters: kappa" << kappa() << " tan(Lambda) " << tanL() << std::endl;
    out << "phi0 to d0 " << phi0() << " d0 " << dR() << " z0 " << dZ() << std::endl;
    out << "Vertex : " << dR()*std::cos(phi0()) << " " << dR()*std::sin(phi0()) << " " << dZ() << std::endl;


    out << "Reference Point " << 0.0 << " " << 0.0 << " " << 0.0 << std::endl;



    if (_nDof > 0) {

      out << "chi2 " << chi2() << " ndof " << nDof() << std::endl;
      //covMatrix().Print();

    }

    TrackHitContainer::size_type numberHits = trackHits().size();

    out << "Number of hits " << numberHits << std::endl;

    out << "Hit numbers: ";
    for (auto const & hit: trackHits()) {
        out <<  hit << " ";
    }
    out << std::endl;



}
