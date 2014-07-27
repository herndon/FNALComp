#ifndef TrackSetIO_hh
#define TrackSetIO_hh
//============================================================================
// TrackSetIO.hh
// header with class definition of the TrackSetIO
// TrackSet read and write algorithms
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

class DetectorGeometry;
class TrackSet;

///
/// Class TrackSetIO: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-13
/// For more information see <a href="../notes/Track.pdf">Track.pdf</a> 
///

class TrackSetIO {
private:

  int _version;

  // Detector information
  const DetectorGeometry & _detectorGeometry;

public:

  TrackSetIO(const DetectorGeometry & detectorGeometry);
  ~TrackSetIO() {};
 
  void writeEvent(const TrackSet &, std::ofstream&) const; //!< Write all track information in the event to std::ofstream file
  void readEvent(TrackSet &, std::ifstream&);  //!< Read all track information for on event from std::ifstream file

  void print(void) const;


};
} // end namespace fc

#endif // TrackSetIO_hh

