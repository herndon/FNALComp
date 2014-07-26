#ifndef LayerTrackFinder_hh
#define LayerTrackFinder_hh
//============================================================================
// LayerTrackFinder.hh
// Find hits compatible with a track on a layer and make new Track candidates
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

  class LayerTrackFinder {

  private:

    int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

    int _layer;
    int _sensorType; 
 
 
    // intermediate tracking parameters
    double _minPTCut;
    double _maxTanLCut;
    double _maxDRCut;
    double _maxDZCut;
    double _maxChi2Cut;
    double _maxChi2NDofCut;
    bool _useExpHits;


  public:
    LayerTrackFinder(int debugLevel,const DetectorGeometry& detectorGeometry,int layer);
    ~LayerTrackFinder() = default;

    void findCandidateTracks(trackSet & trackCandidateSet,const HitSet & recoHitSet,unsigned int expNHit) const;

  private:
    void findSingleCandidateTracks(const Track &,trackSet& allnewTracks, const HitSet & recoHitSet) const;
    std::vector<int> findHits(const Track & track, const HitSet & recoHitSet) const;
    trackSet buildTrackCandidates(const Track &, const std::vector<int> &, const HitSet & recoHitSet) const;     
    std::vector<int> bestTrackCandidates(const trackSet &) const;
    void removeSeedTrack(trackSet&, const trackSet&) const;
    void layerTrackFilter(trackSet &,unsigned int expNHit) const;

    // Helper functions
    bool goodTrack(const Track&) const;



  };


} // end namescape fc

#endif // LayerTrackFinder_hh

