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

#include "DataObjects/include/TrackSet.hh"

namespace fc {

class DetectorGeometry;
class HitSet;

  class TrackRecoStrategy2X1SAS {

 
  public:

 
    TrackRecoStrategy2X1SAS(int debugLevel,const DetectorGeometry& detectorGeometry,
			    double minPTCut,double maxChiNDofCut);
   
    void recoTracks(const HitSet& recoHitSet,TrackSetContainer& trackCandidateSet) const;

    void findTracks(const HitSet& recoHitSet,TrackSetContainer& trackCandidateSet) const;;
 

  private:
     int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

    double _minPTCut;
    double _maxChi2NDofCut;

  };


} // end namescape fc

#endif // Tracking_TrackRecoStrategy2X1SAS_hh

