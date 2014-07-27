#ifndef HitSet_hh
#define HitSet_hh
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

  typedef std::vector<Hit> hitSet;


///
/// Class HitSet: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-08
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a> 
///

class HitSet {
private:

  hitSet _hitSet;

public:

  HitSet() {};
  ~HitSet() {};


  const hitSet & getHits(void) const {return _hitSet;};


  void insertHit(const Hit&);

  void print(void) const;

};
} // end namespace fc
#endif // HitSet_hh

