#define _USE_MATH_DEFINES
#include<cmath>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Helix.hh"
#include "DataObjects/include/HitSet.hh"
#include "Algorithms/include/InitializeHelix.hh"
#include <iostream>

const fc::Helix fc::initializeHelix(const TVector3 & x1, const TVector3 & x2,
                                    const TVector3 & x3, const TVector3 & z1,
                                    const DetectorGeometry & detectorGeometry) {


    // Use three Cartesion points to determine a helix parameters in x,y
    // Pivot point is set to x1.  Have to be set to 0,0,0 or primary vertex if we want to dr and dz to distances to that point
    // Calculation is in 2D


    // Calculate vector between the points in xy to determing rphi coordinates
    TVector3 x12 = x2 - x1;
    TVector3 x13 = x3 - x1;
    TVector3 x23 = x3 - x2;
    TVector3 z(0., 0., 1.);

    x12.SetZ(0.);
    x13.SetZ(0.);
    x23.SetZ(0.);

    // Magnitudes and unit vectors to simplify angle calculates
    double x12Mag = x12.Mag();
    x12 = x12.Unit();
    double x13Mag = x13.Mag();
    x13 = x13.Unit();
    double x23Mag = x23.Mag();
    x23 = x23.Unit();


    // Find radius of curvature
    double sinPhi23 = x12.Cross(x13).Z();
    double cosPhi23 = 0.5 * (x13Mag/x12Mag + (1. - x23Mag/x12Mag)*
                             (x12Mag + x23Mag)/x13Mag);
    double phi23 = std::atan2(sinPhi23, cosPhi23);
    if (sinPhi23 == 0.0) sinPhi23 = 1.0e-10;
    double radiusCurvature  = -0.5 * x23Mag / sinPhi23;


    // !!!!! Since we are using the pV  hit positions based on integer strips you occationally you can get a bad curvature and tanL
    if (std::abs(radiusCurvature) > 125.0) radiusCurvature =
            125.0*radiusCurvature/std::abs(radiusCurvature);
    double tanL = (z1.Z() - x1.Z()) / (radiusCurvature * 2.0 * ((z1.Y()-x1.Y())/
                                       (x3.Y()-x2.Y())) * phi23);
    if (std::abs(tanL) > 0.2) tanL = 0.0;

    // Find center of curvature
    TVector3 centerCurvature = 0.5 * (x2 + x3) + radiusCurvature * cosPhi23 *
                               x23.Cross(z);

//   helix(0) = 0.0;
//   helix(1) = std::atan2(radiusCurvature * (centerCurvature.Y() - x1.Y()), radiusCurvature * (centerCurvature.X() - x1.X()));
//   helix(2) = _alpha / radiusCurvature;
//   helix(3) = 0.0;
//   helix(4) =  (x2.Z() - x3.Z()) / (radiusCurvature * 2 * phi23);


    Helix helix(0.0, std::atan2(radiusCurvature * (centerCurvature.Y() - x1.Y()),
                                radiusCurvature * (centerCurvature.X() - x1.X())),
                -1.0 /(radiusCurvature*detectorGeometry.getCurvatureCInField(
                           detectorGeometry.getBField())),
                0.0,tanL,detectorGeometry.getCurvatureCInField(detectorGeometry.getBField()),
                detectorGeometry.getCurvatureC());
    // Note that phi23 has to be scaled up to the phi between z1 and x1


    return helix;

}

void fc::chooseHitsForInitialization(const HitSet & hitSet,
                                     const std::vector<int> & trackHitCandidate,
                                     const DetectorGeometry& detectorGeometry, int& outerXHit, int& middleXHit,
                                     int& outerZHit) {

    int outerXLayer = -1;
    int outerZLayer = detectorGeometry.getNXSensors()-1;
    int middleXLayer = -1;


    for (auto const& hit : trackHitCandidate) {


        if (hitSet.getHits()[hit].getLayer() > outerXLayer
                && hitSet.getHits()[hit].getLayer() < detectorGeometry.getNXSensors()) {
            outerXLayer = hitSet.getHits()[hit].getLayer();
            outerXHit = hit;
        }

        if (hitSet.getHits()[hit].getLayer() > outerZLayer) {
            outerZLayer = hitSet.getHits()[hit].getLayer();
            outerZHit = hit;
        }

    }

    for (auto const& hit : trackHitCandidate) {


        // !!!!! Figuring out a way to make this general is difficult.
        if (  hitSet.getHits()[hit].getLayer() == 2
                && hitSet.getHits()[hit].getLayer() != outerXLayer) {
            middleXLayer = hitSet.getHits()[hit].getLayer();
            middleXHit = hit;
        } else if (middleXLayer !=2 && hitSet.getHits()[hit].getLayer() == 1
                   && hitSet.getHits()[hit].getLayer() != outerXLayer) {
            middleXLayer = hitSet.getHits()[hit].getLayer();
            middleXHit = hit;
        } else if (middleXLayer !=2 && middleXLayer !=1
                   && hitSet.getHits()[hit].getLayer() == 3
                   && hitSet.getHits()[hit].getLayer() != outerXLayer) {
            middleXLayer = hitSet.getHits()[hit].getLayer();
            middleXHit = hit;
        } else if (middleXLayer !=2 && middleXLayer !=1 && middleXLayer !=3
                   && hitSet.getHits()[hit].getLayer() == 0
                   && hitSet.getHits()[hit].getLayer() != outerXLayer) {
            middleXLayer = hitSet.getHits()[hit].getLayer();
            middleXHit = hit;
        }

    }

}

bool fc::findZForInitialization(const HitSet & hitSet,
                                const std::vector<int> & trackHitCandidate,
                                const DetectorGeometry & detectorGeometry,TVector3& zPosition) {

    int layer4XHit = -1;
    int layer4SASHit = -1;
    int layer3XHit = -1;
    int layer3SASHit = -1;



    for (auto const& hit : trackHitCandidate) {
        if (hitSet.getHits()[hit].getLayer() == 4) layer4XHit = hit;
        if (hitSet.getHits()[hit].getLayer() == 9) layer4SASHit = hit;
        if (hitSet.getHits()[hit].getLayer() == 3) layer3XHit = hit;
        if (hitSet.getHits()[hit].getLayer() == 8) layer3SASHit = hit;
    }


    if (layer4XHit != -1 &&  layer4SASHit != -1) {
        if (intersectStrips(hitSet.getHits()[layer4XHit],hitSet.getHits()[layer4SASHit],
                            detectorGeometry,zPosition)) return true;
    }
    if (layer3XHit != -1 &&  layer3SASHit != -1) {
        if (intersectStrips(hitSet.getHits()[layer3XHit],hitSet.getHits()[layer3SASHit],
                            detectorGeometry,zPosition)) return true;
    }
    return false;

}


bool fc::intersectStrips(const Hit & xHit, const Hit & sasHit,
                         const DetectorGeometry & detectorGeometry,TVector3& z) {

    // Note, in the SAS geometry the strips will always intersect so there is no need to check for non intersecting cases

    TVector3 xPos = xHit.getHitPosition();
    TVector3 xDir = detectorGeometry.getSensor(xHit.getLayer())._normal.Cross(
                        detectorGeometry.getSensor(xHit.getLayer())._measurementDirection);
    TVector3 sasPos = sasHit.getHitPosition();
    TVector3 sasDir = detectorGeometry.getSensor(sasHit.getLayer())._normal.Cross(
                          detectorGeometry.getSensor(sasHit.getLayer())._measurementDirection);

    TVector3 num = (sasPos-xPos).Cross(sasDir);
    TVector3 denom = xDir.Cross(sasDir);

    double disX = (num.Mag()/denom.Mag()) * (num*denom/(num.Mag()*denom.Mag()));

    z = xPos + xDir*disX;

    double localZ = z.Dot(xDir);
    if (std::abs(localZ) < (detectorGeometry.getSensor(
                                xHit.getLayer())._nStrips*detectorGeometry.getSensor(
                                xHit.getLayer())._stripPitch/2.0)) return true;
    return false;
}




