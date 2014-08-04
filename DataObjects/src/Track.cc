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
           detectorGeometry.getCurvatureCInField(detectorGeometry.getBField()),
           detectorGeometry.getCurvatureC()),
    _covMatrix(_sDim,_sDim),
    _chi2(0.0),
    _nDof(0) {

}


const TLorentzVector fc::Track::getLorentzVector() const {
    TLorentzVector lorentzVector;
    double pT = std::abs(1.0/getHelix().getKappa());
    double pZ = getHelix().getTanL()*pT;
    lorentzVector.SetPxPyPzE(pT*std::cos(getHelix().getPhi0()-M_PI/2.0),
                             pT*std::sin(getHelix().getPhi0()-M_PI/2.0),pZ,std::sqrt(pT*pT+pZ*pZ));
    return lorentzVector;
}

double fc::Track::getChi2Prob() const {

    // Root TMatch handles cases where chi2 or ndof = 0
    return TMath::Prob(_chi2,_nDof);

}


void fc::Track::print(ostream& out) const {

    TLorentzVector lorentzVector = getLorentzVector();

    out << "Charge " << getCharge() << std::endl;
    out << "4 momentum " << lorentzVector.Px() << " " <<  lorentzVector.Py() << " "
        <<  lorentzVector.Pz() << " " <<  lorentzVector.E() << " " << std::endl;
    out << "Track parameters:  pT " <<  lorentzVector.Pt() << " cot(theta) " <<
        1/getHelix().getTanL() << " phi0 " << getHelix().getPhi0()-M_PI/2.0 << " d0 " <<
        getHelix().getDr() << " z0 " << getHelix().getDz() << std::endl;
    out << "Helix paramters: kappa " << getHelix().getKappa() << " tan(Lambda) " <<
        getHelix().getTanL() << " phi0 to d0 " << getHelix().getPhi0() << std::endl;
    out << "Reference Point " << 0.0 << " " << 0.0 << " " << 0.0 << std::endl;

    if (_nDof > 0) {

        out << "chi2 " << _chi2 << " ndof " << _nDof << std::endl;
        _covMatrix.Print();

    }

    TrackHitContainer::size_type numberHits =_trackHits.size();

    out << "Number of hits " << numberHits << std::endl;

    out << "Hit numbers: ";
    for (auto const & hit: getHits()) {
        out <<  hit << " ";
    }
    out << std::endl;



}
