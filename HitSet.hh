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
#include <map>
#include <fstream>
#include <iostream>
#include "DetectorGeometry.hh"
#include "Hit.hh"

namespace fc {

  typedef std::map<int, Hit> layerHitMap; //!< Map of strip number and acd counts for one layer

///
/// Class HitSet: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-08
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a> 
///

class HitSet {
private:

  layerHitMap _layerHitMapVector[DetectorGeometry::_nSensors];
  int _version;
  bool _genHits;
  int _eventNumber;

public:

  HitSet();
  HitSet(int eventNumber,bool genHits);
  ~HitSet() {};


  const layerHitMap& getConstLayerHitMap(int layer) const;
  layerHitMap& getLayerHitMap(int layer);


  //Map pair access by type of pair element
  int getHitNumber(layerHitMap::const_iterator iter) const {return iter->first;};
  //int getHitNumber(layerHitMap::iterator iter) {return iter->first;};
  const Hit & getConstHit(layerHitMap::const_iterator iter) const {return iter->second;}
  Hit & getHit(layerHitMap::iterator iter) {return iter->second;}

  void insertHit(int, Hit, int); //!< Insert by hit number, Hit and layer

  int getEventNumber(void) const {return _eventNumber;}

  void writeEvent(std::ofstream&) const; //!< Write all hit information for all sensors std::ofstream file for event _event
  void readEvent(std::ifstream&);  //!< Read all hit information for all sensors std::ifstream file for event _event

  void print(void) const;

  void clear(void);

};
} // end namespace fc
#endif // HitSet_hh

