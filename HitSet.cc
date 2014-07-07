#include "DetectorGeometry.hh"
#include "Exception.hh"
#include "TVector3.h"
#include "HitSet.hh"


fc::HitSet::HitSet():
  _version(1){
}


fc::HitSet::HitSet(bool genHits):
  _version(1),
  _genHits(genHits){
}


void fc::HitSet::insertHit(Hit hit) {
  if (hit.getLayer() >= 0 && hit.getLayer() < DetectorGeometry::_nSensors) {
    _hitVector.push_back(hit);
    return;
  } else {
    throw Exception("HitSet::insertHit: Out of bounds layer");
  }
  
}

void fc::HitSet::print(void) const{

  std::cout << "Hit Set version: " << _version << std::endl;
  std::cout << "Gen Hits: " << _genHits << std::endl;

 
  std::vector<Hit>::size_type numberHits =_hitVector.size();

  int hitNumber = 0;

  std::cout << "Number Hits: " << numberHits << std::endl; 

  for (std::vector<Hit>::const_iterator hitIter =  _hitVector.begin(); hitIter != _hitVector.end(); ++hitIter,++hitNumber){
 
    std::cout << "Hit: " << hitNumber << " associated with track " << hitIter->getTrackNumber() << std::endl; 
    hitIter->print();
  } // end hit loop


}
