#ifndef InitializeHelix_hh
#define InitializeHelix_hh
//============================================================================
// InitializeHelix.hh
// header with class definition of the InitializeHelix from 3 points
// See <A HREF="doc/notes/Track.pdf">TrackF.pdf</A> for more information  !!!!! not done yet
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-13
//============================================================================

#include "TVector3.h"
#include <vector>

class Helix;
class DetectorGeometry;
class HitSet;


namespace fc {

  Helix initializeHelix(const TVector3 & x1, const TVector3 & x2, const TVector3 & x3, const DetectorGeometry & detectorGeometry);
  void chooseHitsForInitialization(const HitSet & hitSet, const std::vector<int> & trackHitCandidate, int& outerXHit, int& middleXHit, int& outerZHit);

  
} // end namescape fc

#endif // InitializeHelix_hh

