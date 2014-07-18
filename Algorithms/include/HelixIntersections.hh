#ifndef HelixIntersections_hh
#define HelixIntersections_hh
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
#include "Geometry/include/DetectorGeometry.hh"
#include "DataObjects/include/Helix.hh"

class TVector3;
class DetectorGeometry;
class Helix;

namespace fcf {


  bool intersectWithLayer(const fc::Helix& helix, TVector3 & hitPosition, int layer, const fc::DetectorGeometry & detectorGeometry);
  bool intersectWithPlane(const fc::Helix& helix, TVector3 & hitPosition, const TVector3 & center, const TVector3 & normal,double & phi);

} // end namescape fcf

#endif // HelixIntersections_hh

