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

void fc::Hit::print(ostream& out) const{

  out << "Hit position: " << _hitPosition.x() << " " <<  _hitPosition.y() << " " <<_hitPosition.z() << std::endl;
  out << "Layer" << _layer << std::endl;
  out << "Number strips: " << _numberStrips << std::endl;
  out << "Track number " << _trackNumber << std::endl;

}

