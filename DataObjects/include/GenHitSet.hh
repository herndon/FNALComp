#ifndef DataObjects_GenHitSet_hh
#define DataObjects_GenHitSet_hh
//============================================================================
// GenHitSet.hh
// header with class definition of the GenHitSet
// A vector of GenHit objects
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-01
//============================================================================
#include <vector>
#include "DataObjects/include/GenHit.hh" 

namespace fc {

  typedef std::vector<GenHit> genHitSet;


///
/// Class GenHitSet: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-07-01
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a> 
///

class GenHitSet {
public:

  const genHitSet & getGenHits() const {return _genHitSet;};


  void insertGenHit(GenHit&&);

  void print(ostream& out) const;

private:

  genHitSet _genHitSet;
};
} // end namespace fc
#endif // DataObjects_GenHitSet_hh

