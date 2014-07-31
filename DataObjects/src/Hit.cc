#include <iostream>
#include "DataObjects/include/Hit.hh"

fc::Hit::Hit(const TVector3 & hitPosition, int layer, int trackNumber):
  _hitPosition(hitPosition),
  _layer(layer),
  _numberStrips(-1),
  _charge(-1),
  _goodHit(true),
  _resolution(0.0),
  _trackNumber(trackNumber){
}

fc::Hit::Hit(const TVector3 & hitPosition, int layer, int numberStrips, int charge, bool goodHit, double resolution):
  _hitPosition(hitPosition),
  _layer(layer),
  _numberStrips(numberStrips),
  _charge(charge),
  _goodHit(goodHit),
  _resolution(resolution),
  _trackNumber(-1){
}

void fc::Hit::print(ostream& out) const{

  if (_trackNumber > -1){
    out << "Hit position:  " << _hitPosition.x() << " " <<  _hitPosition.y() << " " <<_hitPosition.z() << std::endl;
    out << "Layer:         " << _layer << std::endl;
    out << "Number strips: " << _numberStrips << std::endl;
    out << "Track number   " << _trackNumber << std::endl;
  } else {
    out << "Hit position:  " << _hitPosition.x() << " " <<  _hitPosition.y() << " " <<_hitPosition.z() << std::endl;
    out << "Layer          " << _layer << std::endl;
    out << "Number strips: " << _numberStrips << std::endl;
    out << "Charge (ADC ): " << _charge << std::endl;
    out << "goodHit:       " << _goodHit << std::endl;
    out << "Resolution:    " << _resolution << std::endl;
  }



}

