#define _USE_MATH_DEFINES
#include <limits>
#include<cmath>
#include "Helix.hh"

fc::Helix::Helix(){
  _helix = new TMatrixD(5,1);
}

// Helix parameter initialization
fc::Helix::Helix(double dr, double phi0, double kappa, double dz, double tanL){

  _helix = new TMatrixD(5,1);

  (*_helix)(0,0) = dr;
  (*_helix)(1,0) = phi0;
  (*_helix)(2,0) = kappa;
  (*_helix)(3,0) = dz;
  (*_helix)(4,0) = tanL;

}

// Copy constructor
fc::Helix::Helix(const Helix& helix){

  _helix = new TMatrixD(5,1);

  (*_helix) = helix.getHelix();

}




fc::Helix::~Helix(){

  delete _helix;

}
