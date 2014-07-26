#ifndef TrackSet_hh
#define TrackSet_hh
//============================================================================
// TrackSet.hh
// header with class definition of the TrackSet
// A vector of Tracks
// Map elements are int,Track pairs of track number (Key) and Track (value) information 
// See <A HREF="doc/notes/Track.pdf">Track.pdf</A> for more information
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-06
//============================================================================
#include <vector>
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Track.hh"

namespace fc {

  typedef std::vector<Track> trackSet;

///
/// Class TrackSet: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-06
/// For more information see <a href="../notes/Track.pdf">Track.pdf</a> 
///

class TrackSet {
private:


  trackSet _trackSet;

  // Detector information
  const DetectorGeometry & _detectorGeometry;

  double _curvatureC;

public:

  TrackSet(const DetectorGeometry & detectorGeometry);
  ~TrackSet() {};

  const trackSet & getTracks(void) const { return _trackSet;};

  void insertTrack(const Track &);

  void print(void) const;


};
} // end namespace fc

#endif // TrackSet_hh

