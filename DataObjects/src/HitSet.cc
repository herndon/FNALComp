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

void fc::HitSet::print(ostream& out) const {

    out << "HitSet" << std::endl;

    HitSetContainer::size_type numberHits =_hitSet.size();

    int hitNumber = 0;

    out << "Number Hits: " << numberHits << std::endl;

    for (auto hit: getHits()) {
        out << "Hit: " << hitNumber << std::endl;
        hit.print(out);
        ++hitNumber;
    } // end hit loop

}
