#define _USE_MATH_DEFINES
#include<cmath>
#include "DetectorGeometry.hh"
#include "Helix.hh"
#include "HitSet.hh"
#include "InitializeHelix.hh"

fc::Helix fc::initializeHelix(const TVector3 & x1, const TVector3 & x2, const TVector3 & x3, const TVector3 & z1, const DetectorGeometry & detectorGeometry){


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
  double cosPhi23 = 0.5 * (x13Mag/x12Mag + (1. - x23Mag/x12Mag)*(x12Mag + x23Mag)/x13Mag);
  double phi23 = std::atan2(sinPhi23, cosPhi23);
  double radiusCurvature  = -0.5 * x23Mag / sinPhi23;

  std::cout << "radiusCurvature " << radiusCurvature << " x23Mag " << x23Mag << " sinPhi23 " << sinPhi23 << std::endl;
  x1.Print();
  x2.Print();
  x3.Print();

  // Since we are using the pV to see the track occationally you can get a bad curvature
  if (radiusCurvature > 125.0) radiusCurvature = 125.0;

  // Find center of curvature
  TVector3 centerCurvature = 0.5 * (x2 + x3) + radiusCurvature * cosPhi23 * x23.Cross(z);
 
//   helix(0) = 0.0;
//   helix(1) = std::atan2(radiusCurvature * (centerCurvature.Y() - x1.Y()), radiusCurvature * (centerCurvature.X() - x1.X()));
//   helix(2) = _alpha / radiusCurvature;
//   helix(3) = 0.0;
//   helix(4) =  (x2.Z() - x3.Z()) / (radiusCurvature * 2 * phi23);

  Helix helix(0.0, std::atan2(radiusCurvature * (centerCurvature.Y() - x1.Y()), 
	      radiusCurvature * (centerCurvature.X() - x1.X())), -1.0 /(radiusCurvature*detectorGeometry.getCurvatureC()),
	      0.0,(z1.Z() - x1.Z()) / (radiusCurvature * 2.0 * ((z1.Y()-x1.Y())/(x3.Y()-x2.Y())) * phi23),1.0/detectorGeometry.getCurvatureC());
  // Note that phi23 has to be scaled up to the phi between z1 and x1

 
  return helix;

}

void fc::chooseHitsForInitialization(const HitSet & hitSet, const std::vector<int> & trackHitCandidate, int& outerXHit, int& middleXHit, int& outerZHit){

  int outerXLayer = -1;
  int outerZLayer = DetectorGeometry::_nXSensors-1;
  int middleXLayer = -1;

  for (std::vector<int>::const_iterator trackHitCandidateIter = trackHitCandidate.begin(); trackHitCandidateIter != trackHitCandidate.end(); ++trackHitCandidateIter){

    if (hitSet.getHits()[*trackHitCandidateIter].getLayer() > outerXLayer && hitSet.getHits()[*trackHitCandidateIter].getLayer() < DetectorGeometry::_nXSensors) {
      outerXLayer = hitSet.getHits()[*trackHitCandidateIter].getLayer();
      outerXHit = *trackHitCandidateIter;
    }
    if (hitSet.getHits()[*trackHitCandidateIter].getLayer() > outerZLayer) {
      outerZLayer = hitSet.getHits()[*trackHitCandidateIter].getLayer();
      outerZHit = *trackHitCandidateIter;
    }

    // !!!!! Figuring out a way to make this general is difficult.
    if (  hitSet.getHits()[*trackHitCandidateIter].getLayer() == 2) {
      middleXLayer = hitSet.getHits()[*trackHitCandidateIter].getLayer();
      middleXHit = *trackHitCandidateIter;
    } else if (middleXLayer !=2 && hitSet.getHits()[*trackHitCandidateIter].getLayer() == 1) {
      middleXLayer = hitSet.getHits()[*trackHitCandidateIter].getLayer();
      middleXHit = *trackHitCandidateIter;
    } else if (middleXLayer !=2 && middleXLayer !=1 && hitSet.getHits()[*trackHitCandidateIter].getLayer() == 3) {
      middleXLayer = hitSet.getHits()[*trackHitCandidateIter].getLayer();
      middleXHit = *trackHitCandidateIter;
    } else if (middleXLayer !=2 && middleXLayer !=1 && middleXLayer !=3 && hitSet.getHits()[*trackHitCandidateIter].getLayer() == 0) {
      middleXLayer = hitSet.getHits()[*trackHitCandidateIter].getLayer();
      middleXHit = *trackHitCandidateIter;
    }
 
  }






}
