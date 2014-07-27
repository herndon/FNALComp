#define _USE_MATH_DEFINES
#include<cmath>
#include "Geometry/include/StripHitFunctions.hh"

  // From Global hits position to strip number position
double fcf::calclateLocalFromGlobalPostion(const TVector3& hitPosition, int layer,const fc::DetectorGeometry& detectorGeometry){
  return (hitPosition-detectorGeometry.getSensor(layer)._center)*detectorGeometry.getSensor(layer)._measurementDirection;
}

double fcf::calculateStripFromLocalPosition(double localHitPosition, int layer, const fc::DetectorGeometry& detectorGeometry){
  return localHitPosition/detectorGeometry.getSensor(layer)._stripPitch + ((detectorGeometry.getSensor(layer)._nStrips/2.0) - 0.5);
}

  // From strips number position to global Hit position
double fcf::calculateLoalFromStripPosition(double stripHitPosition, int layer, const fc::DetectorGeometry& detectorGeometry){
  return (stripHitPosition - ((detectorGeometry.getSensor(layer)._nStrips/2.0)-0.5)) * detectorGeometry.getSensor(layer)._stripPitch;
}

const TVector3 fcf::calculateGlobalFromLocalPosition(double localHitPosition, int layer, const fc::DetectorGeometry& detectorGeometry){
  TVector3 hitPosition = detectorGeometry.getSensor(layer)._measurementDirection*localHitPosition + detectorGeometry.getSensor(layer)._center;
  return hitPosition;
}
