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
#include <map>
#include <fstream>
#include <iostream>
#include "DetectorGeometry.hh"
#include "Track.hh"

namespace fc {

  typedef std::map<int, Track> TrackMap; //!< Map of track number and Tracks

///
/// Class TrackSet: 
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laboratory 2014-06-06
/// For more information see <a href="../notes/Track.pdf">Track.pdf</a> 
///

class TrackSet {
private:


  TrackMap _trackMap;
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

  const TrackMap& getConstTrackMap(void) const { return _trackMap;};
  TrackMap& getTrackMap(void) {return _trackMap;};

  //Map pair and Track object data access by type of pair element
  int getTrackNumber(TrackMap::const_iterator iter) const { return iter->first;};
  //int getTrackNumber(TrackMap::iterator iter) { return iter->first;};
  const Track &getConstTrack(TrackMap::const_iterator iter) const { return iter->second;};
  Track & getTrack(TrackMap::iterator iter) { return iter->second;};
  const TLorentzVector & getConstLorentzVector(TrackMap::const_iterator iter) const { return iter->second.getConstLorentzVector();};
  //TLorentzVector & getLorentzVector(TrackMap::iterator iter) { return iter->second.getLorentzVector();};
  const TGeoHelix & getConstGeoHelix(TrackMap::const_iterator iter) const { return iter->second.getConstGeoHelix();};
  //TGeoHelix & getGeoHelix(TrackMap::iterator iter) { return iter->second.getGeoHelix();};

  void insertTrack(int, Track); //!< Insert track by tracknumber

  int getGenTracks(void) const {return _genTracks;}
  int getEventNumber(void)const {return _eventNumber;}
 
  void writeEvent(std::ofstream&) const; //!< Write all track information in the event to std::ofstream file
  void readEvent(std::ifstream&);  //!< Read all track information for on event from std::ifstream file

  void print(void) const;

  void clear(void);

};
} // end namespace fc

#endif // TrackSet_hh

