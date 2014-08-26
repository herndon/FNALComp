#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Helix.hh"
#include "Tracking/include/TrackFitAlgorithms.hh"
#include "Tracking/include/HelixIntersections.hh"
#include "Tracking/include/TrackFitMeasurements.hh"


const TMatrixD fc::expectedMeasurementVector1D(const Helix& helix, int layer,
        const DetectorGeometry & detectorGeometry) {

    TVector3 hitPosition;

    double phi = 0.0;

    intersectWithPlane(helix, detectorGeometry.sensor(layer).center(),
                       detectorGeometry.sensor(layer).normal(),phi,hitPosition);

    TMatrixD expectedMeasurementVector(_mDim,1);

    TVector3 measurementDirection = detectorGeometry.sensor(layer).measurementDirection();

    expectedMeasurementVector.Zero();

    expectedMeasurementVector(0,0) = hitPosition*measurementDirection;

    return expectedMeasurementVector;

}

const TMatrixD fc::expectedMeasurementDerivatived1DdHC(const Helix& helix,
        int layer, const DetectorGeometry & detectorGeometry) {

    TVector3 hitPosition;

    double phi = 0.0;

    // std::cout << "In expectedMeasurementDerivative1DdHC " << std::endl;

    // advances phi to the phi at crossing!

    intersectWithPlane(helix,detectorGeometry.sensor(layer).center(),
                       detectorGeometry.sensor(layer).normal(),phi,hitPosition);

    // get normal
    // !!!!! could this be simpler and probably has to be in the correct coordinates. called dsdx
    TMatrixD detectorNormal(1,3);
    detectorNormal(0,0) = detectorGeometry.sensor(layer).normal().X();
    detectorNormal(0,1) = detectorGeometry.sensor(layer).normal().Y();
    detectorNormal(0,2) = detectorGeometry.sensor(layer).normal().Z();


    TMatrixD dxdHC(3,_sDim);
    dxdHC = calcDxDHC(helix,phi);
    TMatrixD dxdphi(3,1);
    dxdphi = calcDxDphi(helix,phi);

    TMatrixD dphidHC(1,_sDim);
    dphidHC = detectorNormal*dxdHC;
    TMatrix dsdphi(1,1);
    dsdphi = detectorNormal*dxdphi;
    double denom = -dsdphi(0,0);
    dphidHC *= 1/denom;

    TMatrixD dxphidHC(3,_sDim);
    dxphidHC = dxdphi*dphidHC + dxdHC;

//   std::cout << "dxdHC" << std::endl;
//   dxdHC.Print();
//   std::cout << "dxdphi" << std::endl;
//   dxdphi.Print();
//   std::cout << "dphidHC" << std::endl;
//   dphidHC.Print();
//   std::cout << "dsdphi" << std::endl;
//   dsdphi.Print();
//   std::cout << "dxphidHC" << std::endl;
//   dxphidHC.Print();





    TMatrixD expectedMeasurementDerivative(_mDim,_sDim);

    TMatrixD measurementDirection(1,3);

    measurementDirection(0,0) =  detectorGeometry.sensor(layer).measurementDirection().X();
    measurementDirection(0,1) =  detectorGeometry.sensor(layer).measurementDirection().Y();
    measurementDirection(0,2) =  detectorGeometry.sensor(layer).measurementDirection().Z();


    expectedMeasurementDerivative.Zero();

    expectedMeasurementDerivative=  calcD1DDHC(hitPosition,dxphidHC,
                                    measurementDirection);

    return expectedMeasurementDerivative;

}

const TMatrixD fc::measurementVector1D(const TVector3 & hitPosition,int layer,
                                       const DetectorGeometry & detectorGeometry) {

    // !!!!! not general, only works if the strip measurement direction is oriented in X

    TMatrixD measurementVector(_mDim,1);

    TVector3 measurementDirection = detectorGeometry.sensor(layer).measurementDirection();

    measurementVector(0,0) = hitPosition*measurementDirection;

    return measurementVector;

}

double fc::expectedMeasurementUncertianty1D(const Helix & helix,
        const TMatrixD& covMatrix, int layer,
        const DetectorGeometry & detectorGeometry) {

    TMatrixD    expectedMeasurementDerivative (_mDim, _sDim);
    TMatrixD    expectedMeasurementDerivativeT (_sDim, _mDim);

    expectedMeasurementDerivative = expectedMeasurementDerivatived1DdHC(helix,layer,
                                    detectorGeometry);
    expectedMeasurementDerivativeT.Transpose(expectedMeasurementDerivative);

    return std::sqrt ((
                          expectedMeasurementDerivative*covMatrix*expectedMeasurementDerivativeT)(0,0));

}




