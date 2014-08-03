#ifndef Geometry_StripHitFunctions_hh
#define Geometry_StripHitFunctions_hh
//============================================================================
// StripHitFunctions.hh
// header with function definitions for converting between, global, local coordinates, and strip positions
// See <A HREF="doc/notes/Geometry.pdf">Geometry.pdf</A> for more information  !!!!! not done yet
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-27
//============================================================================

#include <vector>
#include "TVector3.h"
#include "Geometry/include/DetectorGeometry.hh"

namespace fcf {


  // From Global hits position to strip number position
  double calclateLocalFromGlobalPostion(const TVector3& hitPosition, int layer,const fc::DetectorGeometry &);
  double calculateStripFromLocalPosition(double localHitPosition, int layer, const fc::DetectorGeometry &);


  // Calculate strip position from cluster
  double calculateStripHitPositionFromCluster(int initialStrip,const std::vector<int> & stripAdcs);


  // From strips number position to global Hit position
  double calculateLocalFromStripPosition(double stripHitPosition, int layer, const fc::DetectorGeometry &);
  const TVector3 calculateGlobalFromLocalPosition(double, int, const fc::DetectorGeometry &);

  // Find if hits and strips are in a valid location on a sensor
  bool isValidStrip(int layer, int strip, const fc::DetectorGeometry &);
  bool isValidHit(int layer, const TVector3& hitPosition, const fc::DetectorGeometry &);


  
} // end namescape fcf

#endif // Geometry_StripHitFunctions_hh

