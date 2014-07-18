#include <iostream>
#include "DataObjects/include/Hit.hh"

fc::Hit::Hit(const TVector3 & hitPosition, int layer, int numberStrips, int trackNumber):
  _hitPosition(hitPosition),
  _layer(layer),
  _numberStrips(numberStrips),
  _trackNumber(trackNumber){
}

fc::Hit::Hit(const TVector3 & hitPosition, int layer, int numberStrips):
  _hitPosition(hitPosition),
  _layer(layer),
  _numberStrips(numberStrips),
  _trackNumber(-1){
}


//Copy constructor
fc::Hit::Hit(const Hit & hit):
  _hitPosition(hit._hitPosition),
  _layer(hit._layer),
  _numberStrips(hit._numberStrips),
  _trackNumber(hit._trackNumber){
}
  


void fc::Hit::print(void) const{

  std::cout << "Hit position: " << _hitPosition.x() << " " <<  _hitPosition.y() << " " <<_hitPosition.z() << std::endl;
  std::cout << "Number strips: " << _numberStrips << std::endl;
  std::cout << "Track number " << _trackNumber << std::endl;

}

