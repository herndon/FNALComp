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

class HitSet;

namespace fc {

  class TrackCandidateStrategy1X2SAS : public TrackCandidateStrategy{

  public:

    void findHitCadidates(std::vector<fc::trackHitSet>& trackHitCandidates,const HitSet& hitSet) override;

  };


} // end namescape fc

#endif // TrackCandidateStrategy1X2SAS_hh

