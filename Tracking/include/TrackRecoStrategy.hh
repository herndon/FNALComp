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

namespace fc {

  class DetectorGeometry;
  class HitSet;
  class Track;

  class TrackRecoStrategy {

  protected:

    int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

 

  protected:
    TrackRecoStrategy(int debugLevel,const DetectorGeometry & detectorGeometry);

  public: 
   TrackRecoStrategy( const TrackRecoStrategy&) = delete;
    ~TrackRecoStrategy() = default;

    void recoTracks(trackSet & trackCandidateSet, const HitSet& hitSet) const;

  protected:
    // Iterative track reconstruction
    virtual void findTracks(trackSet& trackCandidateSet,const HitSet & recoHitSet) const = 0;


    // get functions
    const DetectorGeometry & getDetectorGeometry() const {return _detectorGeometry;}
    int getDebugLevel() const {return _debugLevel;}

  private:
    void finalTrackFilter(fc::trackSet & trackCandidateSet) const;

    // Helper functions for final track filter
    int numberMatchedHits(const Track&, const Track&) const;
    bool betterOverlappingTrack(const Track&, const Track&) const;


  };


} // end namescape fc

#endif // TrackRecoStrategy_hh

