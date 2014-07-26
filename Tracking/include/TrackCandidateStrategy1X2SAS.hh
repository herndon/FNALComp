#ifndef TrackCandidateStrategy1X2SAS_hh
#define TrackCandidateStrategy1X2SAS_hh
//============================================================================
// TrackCandidateStategy1X2SAS.hh
// header with function definitions of the TrackCandidateStategy1X2SAS 
//   track candidate finding stratgy
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-23
//============================================================================

#include "Tracking/include/TrackCandidateStrategy.hh"


namespace fc {

class HitSet;

  class TrackCandidateStrategy1X2SAS : public TrackCandidateStrategy{

  public:

    TrackCandidateStrategy1X2SAS(int);


    void findHitCadidates(std::vector<fc::trackHitSet>& trackHitCandidates,const HitSet& hitSet,const DetectorGeometry &) const override;
    void filterTrackCandidates(trackSet& trackCandidateSet,const HitSet& hitSet) const override;

  };


} // end namescape fc

#endif // TrackCandidateStrategy1X2SAS_hh

