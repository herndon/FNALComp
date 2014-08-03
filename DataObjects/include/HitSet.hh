#ifndef DataObjects_HitSet_hh
#define DataObjects_HitSet_hh
//============================================================================
// HitSet.hh
// header with class definition of the HitSet
// A vector of Hit objects
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-08
//============================================================================
#include <vector>
#include "DataObjects/include/Hit.hh"

namespace fc {

typedef std::vector<Hit> HitSetContainer;


///
/// Class HitSet:
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-08
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a>
///

class HitSet {
public:

    const HitSetContainer & getHits() const {
        return _hitSet;
    };


    //void insertHit(const Hit&);
    void insertHit(Hit&&);

    void print(ostream& out) const;

private:

    HitSetContainer _hitSet;
};
} // end namespace fc
#endif // DataObjects_HitSet_hh

