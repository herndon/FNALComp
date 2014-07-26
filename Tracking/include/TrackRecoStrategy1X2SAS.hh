#ifndef TrackRecoStrategy1X2SAS_hh
#define TrackRecoStrategy1X2SAS_hh
//============================================================================
// TrackRecoStategy1X2SAS.hh
// header with function definitions of the TrackRecoStategy1X2SAS tracking stratgy
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-22
//============================================================================

#include "Tracking/include/TrackRecoStrategy.hh"

namespace fc {

class HitSet;

  class TrackRecoStrategy1X2SAS : public TrackRecoStrategy{

 
  public:

 
    TrackRecoStrategy1X2SAS(int debugLevel,const DetectorGeometry& detectorGeometry,double minPTCut,double maxChiNDofCut);
  
  private:

   void findTracks(fc::trackSet& trackCandidateSet,const HitSet & recoHitSet) const override;

    double _minPTCut;
    double _maxChi2NDofCut;


  };


} // end namescape fc

#endif // TrackRecoStrategy1X2SAS_hh

