#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include "Helix.hh"

fc::Helix::Helix():
  _helix(new TVectorD(_sDim)){

}

// Helix parameter initialization
fc::Helix::Helix(double dr, double phi0, double kappa, double dz, double tanL):
  _helix(new TVectorD(_sDim)){
 
  (*_helix)(0) = dr;
  (*_helix)(1) = phi0;
  (*_helix)(2) = kappa;
  (*_helix)(3) = dz;
  (*_helix)(4) = tanL;

}

// Copy constructor
fc::Helix::Helix(const Helix& helix):
  _helix(new TVectorD(helix.getHelix())){

  //*_helix = helix.getHelix();

}
