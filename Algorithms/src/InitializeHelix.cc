#define _USE_MATH_DEFINES
#include<cmath>
#include "DetectorGeometry.hh"
#include "Helix.hh"
#include "InitializeHelix.hh"

fc::Helix fc::initializeHelix(const TVector3 & x1, const TVector3 & x2, const TVector3 & x3, const DetectorGeometry & detectorGeometry){


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

  // Find center of curvature
  TVector3 centerCurvature = 0.5 * (x2 + x3) + radiusCurvature * cosPhi23 * x23.Cross(z);
 
//   helix(0) = 0.0;
//   helix(1) = std::atan2(radiusCurvature * (centerCurvature.Y() - x1.Y()), radiusCurvature * (centerCurvature.X() - x1.X()));
//   helix(2) = _alpha / radiusCurvature;
//   helix(3) = 0.0;
//   helix(4) =  (x2.Z() - x3.Z()) / (radiusCurvature * 2 * phi23);

  Helix helix(0.0, std::atan2(radiusCurvature * (centerCurvature.Y() - x1.Y()), 
	      radiusCurvature * (centerCurvature.X() - x1.X())), 1.0 /(radiusCurvature*detectorGeometry.getCurvatureC()),
	      0.0,(x2.Z() - x3.Z()) / (radiusCurvature * 2 * phi23),1.0/detectorGeometry.getCurvatureC());
 
  return helix;

}


