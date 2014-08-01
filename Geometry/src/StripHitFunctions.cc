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

// Find strip nubmer position
double fcf::calculateStripHitPositionFromCluster(int initialStrip,const std::vector<int> & stripAdcs){

  double stripHitPosition = 0.0;
  double stripPosition = 0.0;
  double adcSum = 0.0;

  for (std::vector<int>::const_iterator stripAdcIter = stripAdcs.begin(); stripAdcIter != stripAdcs.end(); ++stripAdcIter){

    stripHitPosition = stripHitPosition + stripPosition*(*stripAdcIter);
    adcSum = adcSum + (*stripAdcIter);
    stripPosition =  stripPosition + 1.0;
    
  } // end strip loop

  stripHitPosition = stripHitPosition/adcSum;

  stripHitPosition = initialStrip + stripHitPosition;

  return stripHitPosition;

}



  // From strips number position to global Hit position
double fcf::calculateLocalFromStripPosition(double stripHitPosition, int layer, const fc::DetectorGeometry& detectorGeometry){
  return (stripHitPosition - ((detectorGeometry.getSensor(layer)._nStrips/2.0)-0.5)) * detectorGeometry.getSensor(layer)._stripPitch;
}

const TVector3 fcf::calculateGlobalFromLocalPosition(double localHitPosition, int layer, const fc::DetectorGeometry& detectorGeometry){
  TVector3 hitPosition = detectorGeometry.getSensor(layer)._measurementDirection*localHitPosition + detectorGeometry.getSensor(layer)._center;
  return hitPosition;
}


bool fcf::isValidStrip(int layer, int strip, const fc::DetectorGeometry& detectorGeometry){

  return (strip >=0 && strip<detectorGeometry.getSensor(layer)._nStrips); 

}

bool fcf::isValidHit(int layer, const TVector3& hitPosition, const fc::DetectorGeometry& detectorGeometry){

  TVector3 perpDir = detectorGeometry.getSensor(layer)._normal.Cross((detectorGeometry.getSensor(layer)._measurementDirection));
  return ((std::abs((hitPosition-detectorGeometry.getSensor(layer)._center)*detectorGeometry.getSensor(layer)._measurementDirection) <
	   detectorGeometry.getSensor(layer)._nStrips*detectorGeometry.getSensor(layer)._stripPitch/2.0) &&
	  (std::abs((hitPosition-detectorGeometry.getSensor(layer)._center)*perpDir)<detectorGeometry.getSensor(layer)._perpSize/2.0));

}
