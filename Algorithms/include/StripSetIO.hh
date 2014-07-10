#ifndef StripSetIO_hh
#define StripSetIO_hh
//============================================================================
// StripSetIO.hh
// header with class definition of the StripSetIO
// Input and output for the StripSet data object
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-04
//============================================================================
#include <fstream>
#include <iostream>
#include "DetectorGeometry.hh"

class StripSet;

namespace fc {


///
/// Class StripSetIO: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-07-04
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a> 
///

class StripSetIO {
private:

  static const int bitmask1 = 0x00FF;
  static const int bitmask2 = 0xFF00;
  static const int adcBitmask = 0x001F;
  static const int stripBitmask = 0xFFE0;

  const DetectorGeometry _detectorGeometry;

  int _version;

public:

  StripSetIO(const DetectorGeometry &);
  ~StripSetIO() {};

  void writeEvent(const StripSet &, std::ofstream&) const; //!< Write all strip information for all sensors std::ofstream file for event _event
  void readEvent(StripSet &, std::ifstream&);  //!< Read all strip information for all sensors std::ifstream file for event _event

  void printRawData(std::ifstream&) const;

};
} // end namespace

#endif // StripSetIO_hh

