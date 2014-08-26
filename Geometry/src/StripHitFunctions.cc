#define _USE_MATH_DEFINES
#include<cmath>
#include "Geometry/include/StripHitFunctions.hh"

// From Global hits position to strip number position
double fcf::calclateLocalFromGlobalPostion(const TVector3& hitPosition,
        int layer,const fc::DetectorGeometry& detectorGeometry) {
  return (hitPosition-detectorGeometry.sensor(layer).center())
    *detectorGeometry.sensor(layer).measurementDirection();
}

double fcf::calculateStripFromLocalPosition(double localHitPosition, int layer,
        const fc::DetectorGeometry& detectorGeometry) {
  return localHitPosition/detectorGeometry.sensor(layer).stripPitch() + ((detectorGeometry.sensor(layer).nStrips()/2.0) - 0.5);
}

// Find strip nubmer position
double fcf::calculateStripHitPositionFromCluster(int initialStrip,
        const std::vector<int> & stripAdcs) {

    double stripHitPosition = 0.0;
    double stripPosition = 0.0;
    double adcSum = 0.0;

    for (auto stripAdc: stripAdcs) {

        stripHitPosition = stripHitPosition + stripPosition*stripAdc;
        adcSum = adcSum + stripAdc;
        stripPosition =  stripPosition + 1.0;

    } // end strip loop

    stripHitPosition = stripHitPosition/adcSum;

    stripHitPosition = initialStrip + stripHitPosition;

    return stripHitPosition;

}



// From strips number position to global Hit position
double fcf::calculateLocalFromStripPosition(double stripHitPosition, int layer,
        const fc::DetectorGeometry& detectorGeometry) {
    return (stripHitPosition - ((detectorGeometry.sensor(layer).nStrips()/2.0)-0.5)) * detectorGeometry.sensor(layer).stripPitch();
}

const TVector3 fcf::calculateGlobalFromLocalPosition(double localHitPosition,
        int layer, const fc::DetectorGeometry& detectorGeometry) {
    TVector3 hitPosition = detectorGeometry.sensor(layer).measurementDirection()*localHitPosition + detectorGeometry.sensor(layer).center();
    return hitPosition;
}


bool fcf::isValidStrip(int layer, unsigned int strip,
                       const fc::DetectorGeometry& detectorGeometry) {

  return (strip >=0 && strip<detectorGeometry.sensor(layer).nStrips());

}

bool fcf::isValidHit(int layer, const TVector3& hitPosition,
                     const fc::DetectorGeometry& detectorGeometry) {

  TVector3 perpDir = detectorGeometry.sensor(layer).perpDirection();
    return ((std::abs((hitPosition-detectorGeometry.sensor(layer).center())*detectorGeometry.sensor(layer).measurementDirection()) <
             detectorGeometry.sensor(layer).measurementSize()/2.0) &&
            (std::abs((hitPosition-detectorGeometry.sensor(layer).center())*perpDir)
             <detectorGeometry.sensor(layer).perpSize()/2.0));

}
