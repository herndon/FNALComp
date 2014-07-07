#ifndef HitSetIO_hh
#define HitSetIO_hh
//============================================================================
// HitSetIO.hh
// header with class definition of the HitSetIO
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

class HitSet;

namespace fc {


///
/// Class HitSetIO: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-08
/// For more information see <a href="../notes/dataFormat.pdf">dataFormat.pdf</a> 
///

class HitSetIO {
private:

  int _version;

public:

  HitSetIO();
  ~HitSetIO() {};


  void writeEvent(const HitSet &, std::ofstream&) const; //!< Write all hit information for all sensors std::ofstream file for event _event
  void readEvent(HitSet &, std::ifstream&);  //!< Read all hit information for all sensors std::ifstream file for event _event


};
} // end namespace fc
#endif // HitSetIO_hh

