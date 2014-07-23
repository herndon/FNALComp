#ifndef TrackCandidateStrategy_hh
#define TrackCandidateStrategy_hh
//============================================================================
// TrackCandidateStrategy.hh
// header with function definitions of the TrackCandidateStrategy base class for 
//   track cadidate finding  stratgies
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-23
//============================================================================

#include<vector>
#include "DataObjects/include/TrackSet.hh"
#include "DataObjects/include/Track.hh"

class HitSet;

namespace fc {

  class TrackCandidateStrategy {

  private:

    int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

    // candidate track parameters

    double _minCandPTCut;


  public:
    TrackCandidateStrategy(int debugLevel,const DetectorGeometry detectorGeometry);
    TrackCandidateStrategy( const TrackCandidateStrategy&) = delete;
    ~TrackCandidateStrategy() = default;


    void findTrackCandidates(fc::TrackSet& trackCandidateSet, const HitSet& hitSet);

    virtual void findHitCadidates(std::vector<trackHitSet>& trackHitCandidates,const HitSet& hitSet)= 0;
    void buildTrackCandidates(TrackSet& trackCandidateSet,std::vector<fc::trackHitSet>& trackHitCandidates,const HitSet& hitSet);


  };


} // end namescape fc

#endif // TrackCandidateStrategy_hh

