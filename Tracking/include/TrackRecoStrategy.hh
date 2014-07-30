#ifndef Tracking_TrackRecoStrategy_hh
#define Tracking_TrackRecoStrategy_hh
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

  public: 
   TrackRecoStrategy( const TrackRecoStrategy&) = delete;

    void recoTracks(trackSet & trackCandidateSet, const HitSet& hitSet) const;

  protected:

    int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

    TrackRecoStrategy(int debugLevel,const DetectorGeometry & detectorGeometry);

    // Iterative track reconstruction
    virtual void findTracks(trackSet& trackCandidateSet,const HitSet & recoHitSet) const = 0;


    // get functions
    const DetectorGeometry & getDetectorGeometry() const {return _detectorGeometry;}
    int getDebugLevel() const {return _debugLevel;}


  };


} // end namescape fc

#endif // Tracking_TrackRecoStrategy_hh

