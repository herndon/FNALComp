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
#include "DetectorGeometry.hh"
#include "Track.hh"

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
  int _version;
  bool _genTracks;
  int _eventNumber;

  // Detector information
  const DetectorGeometry & _myDetectorGeometry;

  double _curvatureC;

public:

  TrackSet(const DetectorGeometry & myDetectorGeometry);
  TrackSet(int,bool,const DetectorGeometry & myDetectorGeometry);
  ~TrackSet() {};

  const trackSet & getTracks(void) const { return _trackSet;};
  trackSet& getTracks(void) {return _trackSet;};

  void insertTrack(Track);

  int getVersion() const {return _version;}
  int getGenTracks() const {return _genTracks;}
  int getEventNumber()const {return _eventNumber;}
 
  void setGenTracks(bool genTracks) {_genTracks = genTracks;}

  void print(void) const;


};
} // end namespace fc

#endif // TrackSet_hh

