#include <iostream>
#include "TVector3.h"
#include "DataObjects/include/GenHitSet.hh"


// Using default constructor


void fc::GenHitSet::insertGenHit(GenHit&& hit) {
    _genHitSet.push_back(std::move(hit));
}

void fc::GenHitSet::print(ostream& out) const{

  out << "GenHitSet" << std::endl;
 
 
  genHitSet::size_type numberGenHits =_genHitSet.size();

  int hitNumber = 0;

  out << "Number GenHits: " << numberGenHits << std::endl; 

  for (genHitSet::const_iterator hitIter =  _genHitSet.begin(); hitIter != _genHitSet.end(); ++hitIter,++hitNumber){
 
    out << "GenHit: " << hitNumber  << std::endl; 
    hitIter->print(out);
  } // end hit loop


}
