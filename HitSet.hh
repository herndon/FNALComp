#ifndef HitSet_hh
#define HitSet_hh
//============================================================================
// HitSet.hh
// header with class definition of the HitSet
// A vector of map<int,Hit> layer_map's
// Map elements are int,Hit pairs of hitnubmer (Key) and Hit information 
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-08
//============================================================================
#include <vector>
#include "Hit.hh"

namespace fc {


///
/// Class HitSet: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-08
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a> 
///

class HitSet {
private:

  std::vector<Hit> _hitVector;
  int _version;
  bool _genHits;

public:

  HitSet();
  HitSet(bool genHits);
  ~HitSet() {};


  const std::vector<Hit> & getHitVector(void) const {return _hitVector;};

  const Hit & getHit(int hitNumber) const {return _hitVector[hitNumber]; };

  bool getGenHits(void) const {return _genHits;}
  int getVersion(void) const {return _version;}

  void setGenHits(bool genHits) {_genHits = genHits;}

  void insertHit(Hit);

  void print(void) const;

};
} // end namespace fc
#endif // HitSet_hh

