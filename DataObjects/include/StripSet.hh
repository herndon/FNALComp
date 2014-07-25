#ifndef StripSet_hh
#define StripSet_hh
//============================================================================
// StripSet.hh
// header with class definition of the StripSet
// An array of map<int,int> layer_map's
// Map elements are int,int pairs of strip number (Key) and acd (value) information 
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-05-02
//============================================================================
#include <map>
#include <array>
#include "Geometry/include/DetectorGeometry.hh"

namespace fc {

  // Map used to allow keying by strip number
  typedef std::map<int, int> layerStripMap; //!< Map of strip number and acd counts for one layer
  

///
/// Class StripSet: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-05-02
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a> 
///

class StripSet {
private:

  std::array<layerStripMap,DetectorGeometry::_nSensors> _layerStripMapArray;

public:

  StripSet() {};
  ~StripSet() {};

  const layerStripMap& getLayerStripMap(int layer) const;

  int getStripNumber(layerStripMap::const_iterator iter) const {return iter->first;};
  int getStripAdc(layerStripMap::const_iterator iter) const {return iter->second;};

  void insertStrip(int, int, int); //!< Insert strip by layer, strip number, adc count

  void print(void) const;

};
} // end namespace

#endif // StripSet_hh

