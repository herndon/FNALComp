#include <iostream>
#include "TVector3.h"
#include "DataObjects/include/GenHitSet.hh"


// Using default constructor


void fc::GenHitSet::insertGenHit(GenHit&& hit) {
    _genHitSet.push_back(std::move(hit));
}

void fc::GenHitSet::print(std::ostream& out) const {

    out << "GenHitSet" << std::endl;

    GenHitSetContainer::size_type numberGenHits = genHits().size();

    int hitNumber = 0;

    out << "Number GenHits: " << numberGenHits << std::endl;

    for (auto const& hit : genHits()) {
        out << "GenHit: " << hitNumber  << std::endl;
        hit.print(out);
        ++hitNumber;
    } // end hit loop


}
