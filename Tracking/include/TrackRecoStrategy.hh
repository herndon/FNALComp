#ifndef TrackRecoStrategy_hh
#define TrackRecoStrategy_hh
//============================================================================
// TrackRecoStrategy.hh
// header with function definitions of the TrackRecoStrategy base class for tracking stratgies
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-22
//============================================================================

#include<vector>
#include "DataObjects/include/TrackSet.hh"
#include "DataObjects/include/Track.hh"

namespace fc {

  class DetectorGeometry;
  class HitSet;

  class TrackRecoStrategy {

  private:

    int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

 
    // intermediate tracking parameters
    double _minPTCut;
    double _maxTanLCut;
    double _maxDRCut;
    double _maxDZCut;
    double _maxChi2Cut;
    double _maxChi2NDofCut;
    bool _useExpHits;



  protected:
    TrackRecoStrategy(int debugLevel,const DetectorGeometry & detectorGeometry):_debugLevel(debugLevel),_detectorGeometry(detectorGeometry) {};
  public: 
   TrackRecoStrategy( const TrackRecoStrategy&) = delete;
    ~TrackRecoStrategy() = default;

    void recoTracks(trackSet & trackCandidateSet, const HitSet& hitSet);


    // Iterative track reconstruction
    virtual void findTracks(trackSet& trackCandidateSet,const HitSet & recoHitSet) = 0;

    void findHitsOnLayer(trackSet & trackCandidateSet,const HitSet & recoHitSet,int layer);

    void intermediateTrackFilter(trackSet & trackCandidateSet,int expNHits);
    void finalTrackFilter(fc::trackSet & trackCandidateSet);

    // Helper functions
    bool goodTrack(const Track &) const;
    std::vector<int> bestTracks(const trackSet &) const;
    int numberMatchedHits(const Track&, const Track&) const;
    bool betterOverlappingTrack(const Track&, const Track&) const;



  };


} // end namescape fc

#endif // TrackRecoStrategy_hh

