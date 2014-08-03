#ifndef Algorithms_HelixIntersections_hh
#define Algorithms_HelixIntersections_hh
//============================================================================
// HelixIntersections.hh
// header with function definitions for HelixIntersections
// See <A HREF="doc/notes/Track.pdf">TrackF.pdf</A> for more information  !!!!! not done yet
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-04
//============================================================================

#include "TVector3.h"

namespace fc {

class DetectorGeometry;
class Helix;

bool intersectWithLayer(const Helix& helix, int layer,
                        const fc::DetectorGeometry & detectorGeometry,TVector3 & hitPosition);
bool intersectWithPlane(const Helix& helix, const TVector3 & center,
                        const TVector3 & normal,double & phi,TVector3 & hitPosition);

} // end namescape fc

#endif // Algorithms_HelixIntersections_hh

