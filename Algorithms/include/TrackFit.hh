#ifndef Algorithms_TrackFit_hh
#define Algorithms_TrackFit_hh
//============================================================================
// TrackFit.hh
// header with class definition of the TrackFit for reconstructed tracks
// See <A HREF="doc/notes/Track.pdf">TrackF.pdf</A> for more information  !!!!! not done yet
//
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-08
//============================================================================

#include "TVector3.h"
#include "DataObjects/include/Track.hh"
#include <map>

namespace fc {

  class DetectorGeometry;
  class HitSet;
  class Helix;

  const fc::Helix fitToHelix(const Helix& initialHelix, const HitSet& hitSet, const TrackHitContainer & trackHits, const DetectorGeometry& detectorGeometry, TMatrixD &, double&, int&, int _debugLevel);
  const fc::Helix fitToHelixWithPV(const Helix& initialHelix, const HitSet& hitSet, const TrackHitContainer&  trackHits, const DetectorGeometry& detectorGeometry, TMatrixD& finalCovMatrix, double& finalChi2, int& finalNDof, int fitType, int _debugLevel);

} // end namescape fc

#endif // Algorithms_TrackFit_hh

