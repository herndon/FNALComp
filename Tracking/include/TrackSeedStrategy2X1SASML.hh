#ifndef Tracking_TrackSeedStrategy2X1SASML_hh
#define Tracking_TrackSeedStrategy2X1SASML_hh
//============================================================================
// TrackSeedStategy2X1SASML.hh
// header with function definitions of the TrackSeedStategy2X1SASML
//   track candidate finding stratgy
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-08-04
//============================================================================

#include "DataObjects/include/TrackSet.hh"


namespace fc {

class HitSet;
class DetectorGeometry;

class TrackSeedStrategy2X1SASML {

public:

    TrackSeedStrategy2X1SASML(int,const DetectorGeometry&,double);

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

#endif // Tracking_TrackSeedStrategy2X1SASML_hh

