#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include <iostream>
#include "DetectorGeometry.hh"
#include "TrackFitAlgorithms.hh"
#include "HelixIntersections.hh"
#include "TrackFitMeasurements.hh"


TMatrixD fcf::expectedMeasurementVectorXZ(const fc::Helix& helix, int layer, const fc::DetectorGeometry & detectorGeometry) {

  TVector3 hitPosition;

  double phi = 0.0;

  fcf::intersectWithPlane(helix, hitPosition,detectorGeometry.getSensor(layer)._center,detectorGeometry.getSensor(layer)._normal,phi);

  TMatrixD expectedMeasurementVector(fc::DetectorGeometry::_mDim,1);


  expectedMeasurementVector.Zero();

  expectedMeasurementVector(0,0) = hitPosition.Perp()*hitPosition.Phi();
  expectedMeasurementVector(1,0) = hitPosition.Z();
  
  return expectedMeasurementVector;

}

TMatrixD fcf::expectedMeasurementDerivativedXZdHC(const fc::Helix& helix, int layer, const fc::DetectorGeometry & detectorGeometry) {

  TVector3 hitPosition;

  double phi = 0.0;

  // advances phi to the phi at crossing!

  fcf::intersectWithPlane(helix,hitPosition,detectorGeometry.getSensor(layer)._center,detectorGeometry.getSensor(layer)._normal,phi);

  // get normal
  // !!!!! could this be simpler and probably has to be in the correct coordinates. called dsdx
  TMatrixD detectorNormal(1,3);
  detectorNormal(0,0) = detectorGeometry.getSensor(layer)._normal.X();
  detectorNormal(0,1) = detectorGeometry.getSensor(layer)._normal.Y();
  detectorNormal(0,2) = detectorGeometry.getSensor(layer)._normal.Z();


  TMatrixD dxdHC(3,fc::Helix::_sDim); 
  dxdHC = fcf::calcDxDHC(helix,phi);
  TMatrixD dxdphi(3,1);
  dxdphi = fcf::calcDxDphi(helix,phi); 

  TMatrixD dphidHC(1,fc::Helix::_sDim);
  dphidHC = detectorNormal*dxdHC;
  TMatrix dsdphi(1,1);
  dsdphi = detectorNormal*dxdphi; 
  double denom = -dsdphi(0,0);
  dphidHC *= 1/denom;
  
  TMatrixD dxphidHC(3,fc::Helix::_sDim);
  dxphidHC = dxdphi*dphidHC + dxdHC;

 
  TMatrixD expectedMeasurementDerivative(fc::DetectorGeometry::_mDim,fc::Helix::_sDim);
  expectedMeasurementDerivative.Zero();

  expectedMeasurementDerivative=  fcf::calcDXZDHC(hitPosition,dxphidHC);
  
  return expectedMeasurementDerivative;

}

TMatrixD fcf::measurementVectorXZ(const TVector3 & hitPosition) {

  TMatrixD measurementVector(fc::DetectorGeometry::_mDim,1);
  measurementVector(0,0) = hitPosition.Perp()*std::atan2( hitPosition.y(),hitPosition.x());
  measurementVector(1,0) = hitPosition.z();

  return measurementVector;

}




TMatrixD fcf::expectedMeasurementVectorX(const fc::Helix& helix, int layer, const fc::DetectorGeometry & detectorGeometry) {

  TVector3 hitPosition;

  double phi = 0.0;

  fcf::intersectWithPlane(helix, hitPosition,detectorGeometry.getSensor(layer)._center,detectorGeometry.getSensor(layer)._normal,phi);

  TMatrixD expectedMeasurementVector(fc::DetectorGeometry::_mDim,1);


  expectedMeasurementVector.Zero();

  //  expectedMeasurementVector(0,0) = hitPosition.Perp()*hitPosition.Phi();
  expectedMeasurementVector(0,0) = hitPosition.X();
  
  return expectedMeasurementVector;

}

TMatrixD fcf::expectedMeasurementDerivativedXdHC(const fc::Helix& helix, int layer, const fc::DetectorGeometry & detectorGeometry) {

  TVector3 hitPosition;

  double phi = 0.0;

  // advances phi to the phi at crossing!

  fcf::intersectWithPlane(helix,hitPosition,detectorGeometry.getSensor(layer)._center,detectorGeometry.getSensor(layer)._normal,phi);

  // get normal
  // !!!!! could this be simpler and probably has to be in the correct coordinates. called dsdx
  TMatrixD detectorNormal(1,3);
  detectorNormal(0,0) = detectorGeometry.getSensor(layer)._normal.X();
  detectorNormal(0,1) = detectorGeometry.getSensor(layer)._normal.Y();
  detectorNormal(0,2) = detectorGeometry.getSensor(layer)._normal.Z();


  TMatrixD dxdHC(3,fc::Helix::_sDim); 
  dxdHC = fcf::calcDxDHC(helix,phi);
  TMatrixD dxdphi(3,1);
  dxdphi = fcf::calcDxDphi(helix,phi); 

  TMatrixD dphidHC(1,fc::Helix::_sDim);
  dphidHC = detectorNormal*dxdHC;
  TMatrix dsdphi(1,1);
  dsdphi = detectorNormal*dxdphi; 
  double denom = -dsdphi(0,0);
  dphidHC *= 1/denom;
  
  TMatrixD dxphidHC(3,fc::Helix::_sDim);
  dxphidHC = dxdphi*dphidHC + dxdHC;

 
  TMatrixD expectedMeasurementDerivative(fc::DetectorGeometry::_mDim,fc::Helix::_sDim);
  expectedMeasurementDerivative.Zero();

  expectedMeasurementDerivative=  fcf::calcDXDHC(hitPosition,dxphidHC);
  
  return expectedMeasurementDerivative;

}

TMatrixD fcf::measurementVectorX(const TVector3 & hitPosition) {

  // !!!!! not general, only works if the strip measurement direction is oriented in X

  TMatrixD measurementVector(fc::DetectorGeometry::_mDim,1);
  //measurementVector(0,0) = hitPosition.Perp()*std::atan2( hitPosition.y(),hitPosition.x());
  measurementVector(0,0) = hitPosition.X();

  return measurementVector;

}



