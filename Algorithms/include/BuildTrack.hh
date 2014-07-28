#ifndef Algorithms_BuildTrack_hh
#define Algorithms_BuildTrack_hh
//============================================================================
// BuildTrack.hh
// header with class definition of the BuildTrack building a track from hits
// See <A HREF="doc/notes/Track.pdf">Track.pdf</A> for more information  !!!!! not done yet
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-08
//============================================================================



namespace fc {

  class DetectorGeometry;
  class HitSet;
  class Track;

  const Track buildTrack(const HitSet & hitSet, const std::vector<int> & trackHitCandidate, const DetectorGeometry & detectorGeometry, int debugLevel);

} // end namescape fc

#endif // Algorithms_BuildTrack_hh

