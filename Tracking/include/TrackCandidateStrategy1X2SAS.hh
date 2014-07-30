#ifndef Tracking_TrackCandidateStrategy1X2SAS_hh
#define Tracking_TrackCandidateStrategy1X2SAS_hh
//============================================================================
// TrackCandidateStategy1X2SAS.hh
// header with function definitions of the TrackCandidateStategy1X2SAS 
//   track candidate finding stratgy
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-23
//============================================================================

#include "DataObjects/include/TrackSet.hh"


namespace fc {

class HitSet;
class DetectorGeometry;

  class TrackCandidateStrategy1X2SAS {

  public:

    TrackCandidateStrategy1X2SAS(int,const DetectorGeometry&,double);

    void findHitCadidates(std::vector<fc::trackHitSet>& trackHitCandidates,const HitSet& hitSet) const;

    void findTrackCandidates(trackSet& trackCandidateSet, const HitSet& hitSet) const;



  private:
 
   int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;
  
    // candidate track parameters
    double _minCandPTCut;




  };


} // end namescape fc

#endif // Tracking_TrackCandidateStrategy1X2SAS_hh

