#ifndef Tracking_TrackRecoStrategy2X1SAS_hh
#define Tracking_TrackRecoStrategy2X1SAS_hh
//============================================================================
// TrackRecoStategy2X1SAS.hh
// header with function definitions of the TrackRecoStategy2X1SAS tracking stratgy
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-22
//============================================================================

#include "Tracking/include/TrackRecoStrategy.hh"

namespace fc {

class DetectorGeometry;
class HitSet;

  class TrackRecoStrategy2X1SAS : public TrackRecoStrategy{

 
  public:

 
    TrackRecoStrategy2X1SAS(int debugLevel,const DetectorGeometry& detectorGeometry,double minPTCut,double maxChiNDofCut);
  
  private:

   void findTracks(fc::trackSet& trackCandidateSet,const HitSet & recoHitSet) const override;

    double _minPTCut;
    double _maxChi2NDofCut;


  };


} // end namescape fc

#endif // Tracking_TrackRecoStrategy2X1SAS_hh

