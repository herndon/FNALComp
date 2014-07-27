#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include "DataObjects/include/Helix.hh"

fc::Helix::Helix():
   _helix(_sDim){
}

// Helix parameter initialization
fc::Helix::Helix(double dr, double phi0, double kappa, double dz, double tanL, double curvatureCInField):
  _helix(_sDim),
  _alpha(1/curvatureCInField){
 
  _helix(0) = dr;
  if (phi0 >= M_PI/2.0) _helix(1) = phi0;
  if (phi0 < M_PI/2.0) _helix(1) = phi0+M_PI;
  //if ((phi0+M_PI/2.0) < 0.0) _helix(1) = phi0+2.0*M_PI;
  _helix(2) = kappa;
  _helix(3) = dz;
  _helix(4) = tanL;

}
