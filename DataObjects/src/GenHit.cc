#include <iostream>
#include "DataObjects/include/GenHit.hh"

fc::GenHit::GenHit(const TVector3 & position, int layer, int trackNumber):
    _position(position),
    _layer(layer),
    _trackNumber(trackNumber) {
}


void fc::GenHit::print(std::ostream& out) const {

  out << "GenHit position:  " << position().x() << " " <<  position().y() << " " << position().z() << std::endl;
  out << "Layer:         " << layer() << std::endl;
  out << "Track number   " << trackNumber() << std::endl;

}

