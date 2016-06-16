#include <iostream>
#include "DataObjects/include/Hit.hh"


fc::Hit::Hit(const TVector3 & hitPosition, int layer, int numberStrips,
             int charge, bool goodHit, double resolution):
    _hitPosition(hitPosition),
    _layer(layer),
    _numberStrips(numberStrips),
    _charge(charge),
    _goodHit(goodHit),
    _resolution(resolution) {
}

void fc::Hit::print(std::ostream& out) const {

  out << "Hit position:  " << position().x() << " " <<  position().y() << " " << position().z() << std::endl;
  out << "Layer          " << layer() << std::endl;
  out << "Number strips: " << numberStrips() << std::endl;
  out << "Charge (ADC ): " << charge() << std::endl;
  out << "goodHit:       " << goodHit() << std::endl;
  out << "Resolution:    " << resolution() << std::endl;



}

