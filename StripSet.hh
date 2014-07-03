#ifndef StripSet_hh
#define StripSet_hh
//============================================================================
// StripSet.hh
// header with class definition of the StripSet
// A vector of map<int,int> layer_map's
// Map elements are int,int pairs of strip number (Key) and acd (value) information 
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-05-02
//============================================================================
#include <map>
#include <array>
#include <fstream>
#include <iostream>
#include "DetectorGeometry.hh"

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

  static const int bitmask1 = 0x00FF;
  static const int bitmask2 = 0xFF00;
  static const int adcBitmask = 0x001F;
  static const int stripBitmask = 0xFFE0;

  int _eventNumber;
  bool _genStrips;
  int _version; // version 1

  std::array<layerStripMap,DetectorGeometry::_nSensors> _layerStripMapArray;

public:

  StripSet();
  StripSet(int eventNumber,bool genStrips);
  ~StripSet() {};

  const layerStripMap& getLayerStripMap(int layer) const;

  int getStripNumber(layerStripMap::const_iterator iter) const {return iter->first;};
  //int getStripNumber(layerStripMap::iterator iter) {return iter->first;};
  int getStripAdc(layerStripMap::const_iterator iter) const {return iter->second;};
  //int getStripAdc(layerStripMap::iterator iter) {return iter->second;};

  int getEventNumber(void) const {return _eventNumber;}

  void insertStrip(int, int, int); //!< Insert strip by layer, strip number, adc count

  void writeEvent(std::ofstream&) const; //!< Write all strip information for all sensors std::ofstream file for event _event
  void readEvent(std::ifstream&);  //!< Read all strip information for all sensors std::ifstream file for event _event

  void print(void) const;
  void printRawData(std::ifstream&) const;

  void clear(void);

};
} // end namespace

#endif // StripSet_hh

