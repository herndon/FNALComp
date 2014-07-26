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


namespace fc {

class HitSet;
class DetectorGeometry;

  class TrackCandidateStrategy {

  private:

   int _debugLevel;

    // candidate track parameters

    double _minCandPTCut;


  protected:
    TrackCandidateStrategy(int, int);
 
  public:
    TrackCandidateStrategy( const TrackCandidateStrategy&) = delete;
    ~TrackCandidateStrategy() = default;


    void findTrackCandidates(trackSet& trackCandidateSet, const HitSet& hitSet,const DetectorGeometry &) const;

    virtual void findHitCadidates(std::vector<trackHitSet>& trackHitCandidates,const HitSet& hitSet,const DetectorGeometry &) const = 0;
    void buildTrackCandidates(trackSet& trackCandidateSet,std::vector<fc::trackHitSet>& trackHitCandidates,const HitSet& hitSet,const DetectorGeometry &) const;
    virtual void filterTrackCandidates(trackSet& trackCandidateSet,const HitSet& hitSet) const = 0;

    // helper functions
    bool goodCandidate(const Helix & helix) const;


  };


} // end namescape fc

#endif // TrackCandidateStrategy_hh

