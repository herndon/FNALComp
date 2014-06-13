#include <iostream>
#include "Hit.hh"

fc::Hit::Hit(double* hitPosition, int trackNumber):
 _trackNumber(trackNumber){
 _hitPosition[0] = hitPosition[0];
 _hitPosition[1] = hitPosition[1];
 _hitPosition[2] = hitPosition[2];
}

fc::Hit::Hit(double * hitPosition):
 _trackNumber(-1){
 _hitPosition[0] = hitPosition[0];
 _hitPosition[1] = hitPosition[1];
 _hitPosition[2] = hitPosition[2];
}


//Copy constructor
fc::Hit::Hit(const Hit & hit):
  _trackNumber(hit._trackNumber){
  _hitPosition[0] = hit._hitPosition[0];
  _hitPosition[1] = hit._hitPosition[1];
  _hitPosition[2] = hit._hitPosition[2];
}
  


void fc::Hit::print(void) const{

  std::cout << "Hit position: " << _hitPosition[0] << " " <<  _hitPosition[1] << " " <<_hitPosition[2] << std::endl;
  std::cout << "Track number " << _trackNumber << std::endl;

}

