#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "TVector3.h"
#include "DetectorGeometry.hh"
#include "TrackFitAlgorithms.hh"



TVector3 fcf::calcXAt(const fc::Helix& helix, double phi) {
  double csf0 = std::cos(helix.getPhi0());
   double snf0 = std::sin(helix.getPhi0());
   double snfd = std::sin(helix.getPhi0() + phi);
   double csfd = std::cos(helix.getPhi0() + phi);
   double rho  = helix.getAlpha()/helix.getKappa();


   double x    = helix.getDr() * csf0 + rho * (csf0 - csfd);
   double y    = helix.getDr() * snf0 + rho * (snf0 - snfd);
   double z    = helix.getDz()          - rho * helix.getTanL() * phi;

   return TVector3(x,y,z);
}

TMatrixD fcf::calcDxDphi(const fc::Helix& helix, double phi) {
  double radiusCurvature    = helix.getAlpha()/helix.getKappa();

   double snfd = std::sin(helix.getPhi0() + phi);
   double csfd = std::cos(helix.getPhi0() + phi);

   TMatrixD dxdphi(3,1);
   dxdphi(0,0) =  radiusCurvature * snfd;
   dxdphi(1,0) = -radiusCurvature * csfd;
   dxdphi(2,0) = -radiusCurvature * helix.getTanL();

   return dxdphi;
}



TMatrixD fcf::calcDxDHC(const fc::Helix& helix, double phi) {
  double radiusCurvature     = helix.getAlpha()/helix.getKappa();
   double rcpar = radiusCurvature/helix.getKappa();

   double snf0 = std::sin(helix.getPhi0());
   double csf0 = std::cos(helix.getPhi0());
   double snfd = std::sin(helix.getPhi0() + phi);
   double csfd = std::cos(helix.getPhi0() + phi);

   TMatrixD dxDHC(3,5);
   // @x/@a
   dxDHC(0,0) =  csf0;
   dxDHC(0,1) = -helix.getDr() * snf0 - radiusCurvature * (snf0 - snfd);
   dxDHC(0,2) = -rcpar * (csf0 - csfd);
   dxDHC(0,3) =  0;
   dxDHC(0,4) =  0;

   // @y/@a
   dxDHC(1,0) =  snf0;
   dxDHC(1,1) =  helix.getDr() * csf0 + radiusCurvature *(csf0 - csfd);
   dxDHC(1,2) = -rcpar * (snf0 - snfd);
   dxDHC(1,3) =  0;
   dxDHC(1,4) =  0;

   // @z/@a
   dxDHC(2,0) = 0;
   dxDHC(2,1) = 0;
   dxDHC(2,2) = rcpar * phi * helix.getTanL();
   dxDHC(2,3) = 1;
   dxDHC(2,4) = - radiusCurvature * phi;

   return dxDHC;
}


TMatrixD fcf::calcDXZDHC(const TVector3   & hitPosition,
			       const TMatrixD &dxphiadHC) {
  // Calculate
  //    dXZdHC = (@measVec/@a) = (r@phi/@HC, r@z/@HC)^t

  double r = hitPosition.Perp();


  // Set dX = (@h/@a) = (@d/@a, @z/@a)^t
  TMatrixD dXZdHC(fc::DetectorGeometry::_mDim,fc::Helix::_sDim);   

  for (Int_t i=0; i<fc::Helix::_sDim; i++) {
    dXZdHC(0,i) = - (hitPosition.Y() / r) * dxphiadHC(0,i) 
      + (hitPosition.X() / r) * dxphiadHC(1,i);
    dXZdHC(0,i) *= r;
    dXZdHC(1,i) =  dxphiadHC(2,i);
  
  }
  return dXZdHC;
}

TMatrixD fcf::calcDXDHC(const TVector3   & hitPosition,
			const TMatrixD &dxphiadHC,const TMatrixD& measurementDirection) {
  // Calculate
  //    dXdHC = (@measVec/@a) = (r@phi/@HC, r@z/@HC)^t

  //double r = hitPosition.Perp();


  // Set dX = (@h/@a) = (@d/@a, @z/@a)^t
  TMatrixD dXZdHC(fc::DetectorGeometry::_mDim,fc::Helix::_sDim);   

  for (Int_t i=0; i<fc::Helix::_sDim; i++) {
    //    dXZdHC(0,i) = - (hitPosition.Y() / r) * dxphiadHC(0,i) 
    //      + (hitPosition.X() / r) * dxphiadHC(1,i);
    //    dXZdHC(0,i) *= r;

    dXZdHC = measurementDirection*dxphiadHC;
     dXZdHC(0,i) = dxphiadHC(0,i);
    //dXZdHC(1,i) =  dxphiadHC(2,i);
  
  }
  return dXZdHC;
}





