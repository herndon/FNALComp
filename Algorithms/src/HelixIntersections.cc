#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "TMatrixD.h"
#include "DataObjects/include/Helix.hh"
#include "Geometry/include/DetectorGeometry.hh"
#include "Algorithms/include/TrackFitAlgorithms.hh"
#include "Algorithms/include/HelixIntersections.hh"

bool fc::intersectWithLayer(const Helix& helix, int layer,
                            const DetectorGeometry & detectorGeometry,TVector3 & hitPosition) {

    double phi = 0.0;

    return intersectWithPlane(helix,detectorGeometry.sensor(layer).center(),
                              detectorGeometry.sensor(layer).normal(),phi,hitPosition);
}


bool fc::intersectWithPlane(const Helix& helix, const TVector3 & center,
                            const TVector3 & normal,double& phi,TVector3 & hitPosition) {


    const int       maxcount   = 100;
    const double    initlambda = 1.e-10;
    const double    lambdaincr = 10.;
    const double    lambdadecr = 0.1;

    hitPosition = calcXAt(helix,phi);


    double  eps = 1.0e-8;
    double  lastphi =  phi;
    double  lasts   =  99999;
    double  lambda  =  initlambda;

    TVector3  lastHitPosition  =  hitPosition;
    int     count   =  0;

    double s;

    while (1) {
        if (count > maxcount) {
// 	s       = lasts;
// 	phi     = lastphi;
// 	hitPosition      = lastHitPosition;
// 	std::cerr << "TVSurface::CalcXingPointWith:"
// 		  << "   --- Loop count limit reached ---------- " << std::endl
// 		  << "   phi    : " << phi    << std::endl
// 		  << "   x      : " << hitPosition.X() << " "
// 		  << hitPosition.Y() << " "
// 		  << hitPosition.Z() << std::endl
// 		  << "   s      : " << s      << std::endl
// 		  << "   lambda : " << lambda << std::endl;
            return false;
        }
        // put in debug
// 	 std::cerr << "TVSurface::CalcXingPointWith:"
// 		   << "   phi    : " << lastphi    << std::endl
//                    << "   x      : " << lastHitPosition.X() << " "
//                                 << lastHitPosition.Y() << " "
// 		   << lastHitPosition.Z() << std::endl
// 		   << "   s      : " << lasts      << std::endl
// 		   << "   lambda : " << lambda << std::endl;
// 	 print();
        ++count;
        s  = (hitPosition-center)*normal;
        if (std::abs(s) < eps) break;
        if (std::abs(s) < std::abs(lasts)) {
            lasts   = s;
            lastphi = phi;
            lastHitPosition  = hitPosition;
            lambda *= lambdadecr;
        } else {
            s       = lasts;
            phi     = lastphi;
            hitPosition      = lastHitPosition;
            lambda *= lambdaincr;
        }
        TMatrixD dsdx(1,3);
        dsdx(0,0) = normal.X();
        dsdx(0,1) = normal.Y();
        dsdx(0,2) = normal.Z();

        TMatrixD dxdphi = calcDxDphi(helix,phi);
        TMatrixD dsdphi = dsdx * dxdphi;
        double denom = (1 + lambda) * dsdphi(0,0);
        phi -= s / denom;
        hitPosition   = calcXAt(helix,phi);
    }
    return true;

}


