#ifndef Algorithms_HitSetIO_hh
#define Algorithms_HitSetIO_hh
//============================================================================
// HitSetIO.hh
// header with class definition of the HitSetIO
// A vector of Hit objects
// See <A HREF="doc/notes/dataFormat.pdf">dataFormat.pdf</A> for more information
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-08
//============================================================================
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

public:
  HitSetIO();

  void writeEvent(const HitSet &, std::ofstream&) const; //!< Write all hit information for all sensors std::ofstream file for event _event
  void readEvent(HitSet &, std::ifstream&);  //!< Read all hit information for all sensors std::ifstream file for event _event

private:

  int _version;
};
} // end namespace fc
#endif // Algorithms_HitSetIO_hh

