#ifndef Tracking_TrackMatching_hh
#define Tracking_TrackMatching_hh
//============================================================================
// TrackMatching.hh
// header with track matching utilities
//
// Author Matt Herndon, University of Wisconsin,
// 2014-08-15
//============================================================================


class DetectorGeoemtry;
class Track;
class GenTrack;
class TrackSet;
class GenTrackSet;


namespace fcf {


// Track selection filters

  const fc::Track & matchTrack(const fc::GenTrack & genTrack,
			       const fc::TrackSet& trackSet, const fc::DetectorGeometry& detectorGeometry,
			       bool& matchedTrackLoose, bool& matchedTrackXYLoose,
			       bool& matchedTrackTight, bool& matchedTrackXYTight);
  const fc::GenTrack & matchTrack(const fc::Track & track,
			       const fc::GenTrackSet& genTrackSet, const fc::DetectorGeometry& detectorGeometry,
			       bool& matchedTrackLoose, bool& matchedTrackXYLoose,
			       bool& matchedTrackTight, bool& matchedTrackXYTight);
  const fc::Track & matchTrack(const fc::Track & track,
			       const fc::TrackSet& trackSet,
			       bool& matchedTrack, bool& matchedTrackXY);




  bool goodMatch(const fc::GenTrack & genTrack,const fc::Track& track,double sigma,const fc::DetectorGeometry& detectorGeometry);
  bool goodMatchXY(const fc::GenTrack & genTrack,const fc::Track& track,double sigma,const fc::DetectorGeometry& detectorGeometry);
  bool goodMatch(const fc::Track & track,const fc::Track& track1);
  bool goodMatchXY(const fc::Track & track,const fc::Track& track1);



  TVectorD deltaHP(const fc::GenTrack & genTrack,const fc::Track& track,const fc::DetectorGeometry& detectorGeometry);
  TVectorD deltaHP(const fc::Track & track,const fc::Track& track1);



  double deltaTrackHPs(const fc::GenTrack & genTrack,const fc::Track& track,const fc::DetectorGeometry& detectorGeometry);
  double deltaTrackHPs(const fc::Track & track0,const fc::Track& track1);



} // end namescape fcf

#endif // Tracking_TrackMatching_hh

