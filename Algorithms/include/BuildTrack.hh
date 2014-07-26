#ifndef BuildTrack_hh
#define BuildTrack_hh
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

#include "DataObjects/include/Track.hh"

class DetectorGeometry;
class HitSet;

namespace fc {

  const Track BuildTrack(const HitSet & hitSet, const std::vector<int> & trackHitCandidate, const DetectorGeometry & detectorGeometry, int debugLevel);

} // end namescape fc

#endif // BuildTrack_hh

