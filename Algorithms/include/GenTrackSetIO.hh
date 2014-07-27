#ifndef GenTrackSetIO_hh
#define GenTrackSetIO_hh
//============================================================================
// GenTrackSetIO.hh
// header with class definition of the GenTrackSetIO
// GenTrackSet read and write algorithms
// See <A HREF="doc/notes/Track.pdf">Track.pdf</A> for more information !!!! no done yet
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-13
//============================================================================
#include <fstream>
#include <iostream>

namespace fc {

class GenTrackSet;


///
/// Class GenTrackSetIO: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-13
/// For more information see <a href="../notes/Track.pdf">Track.pdf</a> 
///

class GenTrackSetIO {
private:

  int _version;

public:

  GenTrackSetIO();
  ~GenTrackSetIO() {};
 
  void writeEvent(const GenTrackSet&, std::ofstream&) const; 
  //!< Write all track information in the event to std::ofstream file
 
  void readEvent(GenTrackSet &, std::ifstream&);  //!< Read all track information for on event from std::ifstream file

  void print(ostream& out) const;


};
} // end namespace fc

#endif // GenTrackSetIO_hh

