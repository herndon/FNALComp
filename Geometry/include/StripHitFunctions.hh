#ifndef StripHitFunctions_hh
#define StripHitFunctions_hh
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

#include "TVector3.h"
#include "Geometry/include/DetectorGeometry.hh"

namespace fcf {


  // From Global hits position to strip number position
  double calclateLocalFromGlobalPostion(const TVector3 &, int,const fc::DetectorGeometry &);
  double calculateStripFromLocalPosition(double, int, const fc::DetectorGeometry &);


  // From strips number position to global Hit position
  double calculateLoalFromStripPosition(double, int, const fc::DetectorGeometry &);
  const TVector3 calculateGlobalFromLocalPosition(double, int, const fc::DetectorGeometry &);


  
} // end namescape fcf

#endif // StripHitFunctions_hh

