#ifndef Algorithms_InitializeHelix_hh
#define Algorithms_InitializeHelix_hh
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



namespace fc {

class Helix;
class DetectorGeometry;
class HitSet;
class Hit;



const Helix initializeHelix(const TVector3 & x1, const TVector3 & x2,
                            const TVector3 & x3,  const TVector3 & z1,
                            const DetectorGeometry & detectorGeometry);
void chooseHitsForInitialization(const HitSet & hitSet,
                                 const std::vector<int> & trackHitCandidate,
                                 const DetectorGeometry & detectorGeometry,
                                 int& outerXHit, int& middleXHit, int& outerZHit);
bool findZForInitialization(const HitSet & hitSet,
                            const std::vector<int> & trackHitCandidate,
                            const DetectorGeometry & detectorGeometry,TVector3 & z);
bool intersectStrips(const Hit & xHit, const Hit & sasHit,
                     const DetectorGeometry & detectorGeometry,TVector3 & z);

} // end namescape fc

#endif // Algorithms_InitializeHelix_hh

