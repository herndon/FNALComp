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
#include <fstream>
#include <iostream>
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
  int _eventNumber;

public:

  HitSet();
  HitSet(int eventNumber,bool genHits);
  ~HitSet() {};


  const std::vector<Hit> & getConstHitVector(void) const {return _hitVector;};
  std::vector<Hit>& getHitVector(void) {return _hitVector;};

  const Hit & getHit(int hitNumber) const {return _hitVector[hitNumber]; };

  void insertHit(Hit);

  int getEventNumber(void) const {return _eventNumber;}

  void writeEvent(std::ofstream&) const; //!< Write all hit information for all sensors std::ofstream file for event _event
  void readEvent(std::ifstream&);  //!< Read all hit information for all sensors std::ifstream file for event _event

  void print(void) const;

};
} // end namespace fc
#endif // HitSet_hh

