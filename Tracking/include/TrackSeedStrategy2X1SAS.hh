#ifndef Tracking_TrackCandidateStrategy2X1SAS_hh
#define Tracking_TrackSeedStrategy2X1SAS_hh
//============================================================================
// TrackSeedStategy2X1SAS.hh
// header with function definitions of the TrackSeedStategy2X1SAS
//   track candidate finding stratgy
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-07-23
//============================================================================

#include "DataObjects/include/TrackSet.hh"


namespace fc {

class HitSet;
class DetectorGeometry;

class TrackSeedStrategy2X1SAS {

public:

    TrackSeedStrategy2X1SAS(int,const DetectorGeometry&,double);

    void findHitCadidates(const HitSet& hitSet,
                          std::vector<fc::TrackHitContainer>& trackHitSeeds) const;

    void findTrackSeeds(const HitSet& hitSet,
                             FastTrackSetContainer& trackSeedSet) const;



private:

    int _debugLevel;

    // Detector information
    const DetectorGeometry & _detectorGeometry;

    // candidate track parameters
    double _minCandPTCut;




};


} // end namescape fc

#endif // Tracking_TrackSeedStrategy2X1SAS_hh

