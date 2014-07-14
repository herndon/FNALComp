#ifndef TrackSet_hh
#define TrackSet_hh
//============================================================================
// TrackSet.hh
// header with class definition of the TrackSet
// A map map<int,Track>
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


///
/// Class TrackSet: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-06
/// For more information see <a href="../notes/Track.pdf">Track.pdf</a> 
///

class TrackSet {
private:


  std::vector<Track> _trackVector;
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

  const std::vector<Track>& getConstTrackVector(void) const { return _trackVector;};
  std::vector<Track>& getTrackVector(void) {return _trackVector;};

  const Track& getConstTrack(int trackNumber) const { return _trackVector[trackNumber];};
  Track & getTrack(int trackNumber) { return _trackVector[trackNumber];};

  void insertTrack(Track);

  int getVersion() const {return _version;}
  int getGenTracks() const {return _genTracks;}
  int getEventNumber()const {return _eventNumber;}
 
  void setGenTracks(bool genTracks) {_genTracks = genTracks;}

  void print(void) const;


};
} // end namespace fc

#endif // TrackSet_hh

