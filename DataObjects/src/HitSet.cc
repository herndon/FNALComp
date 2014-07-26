#include <iostream>
#include "TVector3.h"
#include "DataObjects/include/HitSet.hh"


// Using default constructor

void fc::HitSet::insertHit(const Hit& hit) {
    _hitSet.push_back(hit);
}

void fc::HitSet::print(void) const{

  std::cout << "HitSet" << std::endl;
 
 
  hitSet::size_type numberHits =_hitSet.size();

  int hitNumber = 0;

  std::cout << "Number Hits: " << numberHits << std::endl; 

  for (hitSet::const_iterator hitIter =  _hitSet.begin(); hitIter != _hitSet.end(); ++hitIter,++hitNumber){
 
    std::cout << "Hit: " << hitNumber << " associated with track " << hitIter->getTrackNumber() << std::endl; 
    hitIter->print();
  } // end hit loop


}
