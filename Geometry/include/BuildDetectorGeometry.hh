#ifndef Geometry_BuildDetectorGeometry_hh
#define Geometry_BuildDetectorGeometry_hh
//============================================================================
// BuildDetectorGeometry.hh
// header with functions to build the DetectorGeometry from a file
// For more information see <a href="doc/notes/detectorGeometry.pdf">detectorGeometry.pdf</a>
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-26
//============================================================================
#include <fstream>
#include <iostream>

namespace fc {

  class DetectorGeometry;

  const DetectorGeometry buildDetectorGeometry(std::ifstream & detectorgeometryfile);

} // end namespace fc

#endif // Geometry_BuildDetectorGeometry_hh
