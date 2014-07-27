#ifndef Tracking_TrackCandidateStrategy_hh
#define Tracking_TrackCandidateStrategy_hh
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


namespace fc {

class HitSet;
class DetectorGeometry;

  class TrackCandidateStrategy {
  public:
    TrackCandidateStrategy( const TrackCandidateStrategy&) = delete;

    void findTrackCandidates(trackSet& trackCandidateSet, const HitSet& hitSet) const;


  protected:

    TrackCandidateStrategy(int, const DetectorGeometry&,double);
 
    virtual void findHitCadidates(std::vector<trackHitSet>& trackHitCandidates,const HitSet& hitSet) const = 0;
    virtual void filterTrackCandidates(trackSet& trackCandidateSet,const HitSet& hitSet) const = 0;

    // helper functions
    bool goodCandidate(const Helix & helix) const;


   int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;
  
    // candidate track parameters
    double _minCandPTCut;

  private: 
   void buildTrackCandidates(trackSet& trackCandidateSet,std::vector<fc::trackHitSet>& trackHitCandidates,const HitSet& hitSet) const;


  };


} // end namescape fc

#endif // Tracking_TrackCandidateStrategy_hh

