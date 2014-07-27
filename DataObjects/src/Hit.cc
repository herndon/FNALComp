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

void fc::Hit::print() const{

  std::cout << "Hit position: " << _hitPosition.x() << " " <<  _hitPosition.y() << " " <<_hitPosition.z() << std::endl;
  std::cout << "Layer" << _layer << std::endl;
  std::cout << "Number strips: " << _numberStrips << std::endl;
  std::cout << "Track number " << _trackNumber << std::endl;

}

