#ifndef Algorithms_TrackSetIO_hh
#define Algorithms_TrackSetIO_hh
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
public:

  explicit TrackSetIO(const DetectorGeometry & detectorGeometry);
 
  void writeEvent(const TrackSet &, std::ofstream&) const; //!< Write all track information in the event to std::ofstream file
  void readEvent(TrackSet &, std::ifstream&);  //!< Read all track information for on event from std::ifstream file

  void print() const;

private:

  int _version;

  // Detector information
  const DetectorGeometry & _detectorGeometry;
};
} // end namespace fc

#endif // Algorithms_TrackSetIO_hh

