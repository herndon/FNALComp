#ifndef StripSet_hh
#define StripSet_hh
//============================================================================
// StripSet.hh
// header with class definition of the StripSet
// A vector of map<int,int> layer_map's
// Map elements are int,int pairs of strip (Key) and acd (value) information 
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
//
// Planned upgrade, use detector geopmetry to access the number of layers.
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-05-02
//============================================================================
#include <map>
#include <fstream>
#include <iostream>
#include "DetectorGeometry.hh"

  typedef std::map<int, int> layerMap; //!< Map of strip number and acd counts for one layer

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

  layerMap _layerMapVector[DetectorGeometry::_nSensors];
  int _event;

public:

  StripSet() {};
  ~StripSet() {};

  const layerMap& getLayerMap(int layer);

  void insertStrip(int, int, int); //!< Insert strip by layer, strip number, adc count

  int getEvent(void){return _event;}
  void setEvent(int event) {_event = event;}

  void writeEvent(std::ofstream&); //!< Write all strip information for all sensors std::ofstream file for event _event
  void readEvent(std::ifstream&);  //!< Read all strip information for all sensors std::ifstream file for event _event

  void print(void);
  void printRawData(std::ifstream&);

  void clear(void);

};
#endif // StripSet_hh

