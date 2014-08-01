#include <iostream>
#include "DataObjects/include/GenHit.hh"

fc::GenHit::GenHit(const TVector3 & hitPosition, int layer, int trackNumber):
  _hitPosition(hitPosition),
  _layer(layer),
  _trackNumber(trackNumber){
}


void fc::GenHit::print(ostream& out) const{

  out << "GenHit position:  " << _hitPosition.x() << " " <<  _hitPosition.y() << " " <<_hitPosition.z() << std::endl;
  out << "Layer:         " << _layer << std::endl;
  out << "Track number   " << _trackNumber << std::endl;

}

