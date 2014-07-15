#ifndef TrackFit_hh
#define TrackFit_hh
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
#include "Helix.hh"
#include <map>

class DetectorGeometry;
class HitSet;
class Helix;


namespace fc {



  Helix FitToHelix(Helix& initialHelix, const HitSet& hitSet, const std::map<int, int>& _trackHitMap, const DetectorGeometry& detectorGeometry,int _debugLevel);


} // end namescape fc

#endif // TrackFit_hh

