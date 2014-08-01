#include <iostream>
#include "TVector3.h"
#include "DataObjects/include/HitSet.hh"


// Using default constructor

// void fc::HitSet::insertHit(const Hit& hit) {
//     _hitSet.push_back(hit);
// }

void fc::HitSet::insertHit(Hit&& hit) {
    _hitSet.push_back(std::move(hit));
}

void fc::HitSet::print(ostream& out) const{

  out << "HitSet" << std::endl;
 
  hitSet::size_type numberHits =_hitSet.size();

  int hitNumber = 0;

  out << "Number Hits: " << numberHits << std::endl; 

  for (hitSet::const_iterator hitIter =  _hitSet.begin(); hitIter != _hitSet.end(); ++hitIter,++hitNumber){
 
    out << "Hit: " << hitNumber << std::endl; 
    hitIter->print(out);
  } // end hit loop


}
