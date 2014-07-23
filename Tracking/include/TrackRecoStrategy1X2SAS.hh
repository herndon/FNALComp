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

class HitSet;

namespace fc {

  class TrackRecoStrategy1X2SAS : public TrackRecoStrategy{


  public:


    void findHitCadidates(std::vector<fc::trackHitMap>& trackHitCandidates,const HitSet& hitSet) override;

    void findTracks(fc::trackSet& trackCandidateSet,const HitSet & recoHitSet) override;




  };


} // end namescape fc

#endif // TrackRecoStrategy1X2SAS_hh

