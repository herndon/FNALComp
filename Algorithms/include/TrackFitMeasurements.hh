#ifndef TrackFitMeasurements_hh
#define TrackFitMeasurements_hh
//============================================================================
// TrackFitMeasurements.hh
// header with funciton definitions for Measurements and Measurement derivatives
// in local detector coordinates
// See <A HREF="doc/notes/Track.pdf">TrackF.pdf</A> for more information  !!!!! not done yet
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-14
//============================================================================

#include "TMatrixD.h"

class TVector3;

namespace fc {

  class Helix;
  class DetectorGeometry;

  const TMatrixD expectedMeasurementVector1D(const fc::Helix& helix, int layer, const fc::DetectorGeometry & detectorGeometry);
  const TMatrixD expectedMeasurementDerivatived1DdHC(const fc::Helix& helix,int layer, const fc::DetectorGeometry & detectorGeometry);
  const TMatrixD measurementVector1D(const TVector3 & hitPosition,int layer, const fc::DetectorGeometry & detectorGeometry);

  double expectedMeasurementUncertianty1D(const fc::Helix & helix, const TMatrixD& covMatrix, int layer,const fc::DetectorGeometry & detectorGeometry);  


} // end namescape fc

#endif // TrackFitMeasurements_hh

