#define _USE_MATH_DEFINES
#include<cmath>
#include "Geometry/include/StripHitFunctions.hh"

// From Global hits position to strip number position
double fcf::calclateLocalFromGlobalPostion(const TVector3& hitPosition,
        int layer,const fc::DetectorGeometry& detectorGeometry) {
    return (hitPosition-detectorGeometry.sensor(layer)._center)
           *detectorGeometry.sensor(layer)._measurementDirection;
}

double fcf::calculateStripFromLocalPosition(double localHitPosition, int layer,
        const fc::DetectorGeometry& detectorGeometry) {
    return localHitPosition/detectorGeometry.sensor(layer)._stripPitch + ((
                detectorGeometry.sensor(layer)._nStrips/2.0) - 0.5);
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
    return (stripHitPosition - ((detectorGeometry.sensor(
                                     layer)._nStrips/2.0)-0.5)) * detectorGeometry.sensor(layer)._stripPitch;
}

const TVector3 fcf::calculateGlobalFromLocalPosition(double localHitPosition,
        int layer, const fc::DetectorGeometry& detectorGeometry) {
    TVector3 hitPosition = detectorGeometry.sensor(
                               layer)._measurementDirection*localHitPosition + detectorGeometry.sensor(
                               layer)._center;
    return hitPosition;
}


bool fcf::isValidStrip(int layer, int strip,
                       const fc::DetectorGeometry& detectorGeometry) {

    return (strip >=0 && strip<detectorGeometry.sensor(layer)._nStrips);

}

bool fcf::isValidHit(int layer, const TVector3& hitPosition,
                     const fc::DetectorGeometry& detectorGeometry) {

    TVector3 perpDir = detectorGeometry.sensor(layer)._normal.Cross((
                           detectorGeometry.sensor(layer)._measurementDirection));
    return ((std::abs((hitPosition-detectorGeometry.sensor(
                           layer)._center)*detectorGeometry.sensor(layer)._measurementDirection) <
             detectorGeometry.sensor(layer)._nStrips*detectorGeometry.sensor(
                 layer)._stripPitch/2.0) &&
            (std::abs((hitPosition-detectorGeometry.sensor(layer)._center)*perpDir)
             <detectorGeometry.sensor(layer)._perpSize/2.0));

}
