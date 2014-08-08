#ifndef Tracking_LayerTrackFinder_hh
#define Tracking_LayerTrackFinder_hh
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

namespace fc {

class DetectorGeometry;
class HitSet;
class Track;

class LayerTrackFinder {

public:
    LayerTrackFinder(int debugLevel,const DetectorGeometry& detectorGeometry,
                     int layer,unsigned int nExpHits, double minPTCut,double maxChi2NDofCut);

    void findTracks(const HitSet & recoHitSet,unsigned int expNHit,
                             TrackSetContainer & trackSet) const;

private:
    void findTrack(const Track &, const HitSet & recoHitSet,
                                   TrackSetContainer& allnewTracks) const;
    std::vector<int> findHits(const Track & track, const HitSet & recoHitSet) const;
    TrackSetContainer buildTracks(const Track &, const std::vector<int> &,
                                           const HitSet & recoHitSet) const;
    std::vector<int> bestTracks(const TrackSetContainer & newTracks) const;
    void removeSeedTrack(const TrackSetContainer& oldTracks,
                         TrackSetContainer& newTracks) const;

    int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

    int _layer;
    int _sensorType;


    // intermediate tracking parameters
    unsigned int _nExpHits;
    double _minPTCut;
    double _maxChi2NDofCut;
};


} // end namescape fc

#endif // Tracking_LayerTrackFinder_hh

