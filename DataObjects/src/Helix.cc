#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include "Helix.hh"

fc::Helix::Helix():
  _helix(_sDim){

}

// Helix parameter initialization
fc::Helix::Helix(double dr, double phi0, double kappa, double dz, double tanL):
  _helix(_sDim){
 
  _helix(0) = dr;
  _helix(1) = phi0;
  _helix(2) = kappa;
  _helix(3) = dz;
  _helix(4) = tanL;

}

// Copy constructor
fc::Helix::Helix(const Helix& helix):
  _helix(helix.getHelix()){

}
